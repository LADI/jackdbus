/* -*- Mode: C ; c-basic-offset: 4 -*- */
/*
 * SPDX-FileCopyrightText: Copyright © 2009 Grame
 * SPDX-FileCopyrightText: Copyright © 2012-2025 Nedko Arnaudov
 * SPDX-License-Identifier: GPL-2.0-only
 */
/* PAX SOUND SERVERIS, A simple device reservation scheme with D-Bus */
/*
 * This implementation, compreated to 2009-2012 version of the jack2 code:
 *  * Adds device give up and take back callbacks
 *  * Caller of *_reservation_init() is to call *_reservation_uninit()
 *  * D-Bus connection is singleton
 *  * *_reservation_loop() is dropped
 *  * Reserved devices are kept in a list instead of array
 *  * Modified reserve.[ch] files in are used
 *    * Releasing device name ownership is decoupled from releasing reference to rd_device.
 *    * Acquiring device via rd_acquire() is alowed to reuse already created rd_device.
 *    * NameOwnerChanged signals are now monitored and when device is available for reaquire,
 *      a rd_available_cb_t callback is called device name ownership is reaquired.
 *    * userdata is supplied via rd_acquire(), when object is created (instead of rd_set_userdata)
 */

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "reserve.h"
#include "device_reservation.h"
#include "jack/control.h"
#include "list.h"

/* string like "Audio2", "Midi5" or "Video3" */
#define MAX_DEVICE_NAME 63

struct reserved_device
{
    struct list_head siblings;
    char device_name[MAX_DEVICE_NAME+1];
    rd_device * reserved_device;
};

static DBusConnection * g_dbus_connection;
static LIST_HEAD(g_device_reservation_list);
static void * g_device_reservation_ctx;

int device_reservation_init(DBusConnection * connection, void * ctx)
{
    g_dbus_connection = connection;
    g_device_reservation_ctx = ctx;
    jack_info("PAX SOUND SERVERIS module initialized");
    return 0;
}

int device_reservation_uninit(void)
{
    struct list_head * node_ptr;
    struct reserved_device * device_ptr;

    while (!list_empty(&g_device_reservation_list))
    {
        node_ptr = g_device_reservation_list.prev;
        device_ptr = list_entry(node_ptr, struct reserved_device, siblings);
        list_del(node_ptr);
        rd_release(device_ptr->reserved_device, 0);
        free(device_ptr);
    }
    g_dbus_connection = NULL;
    jack_info("PAX SOUND SERVERIS module uninitialized");
    return 0;
}

int
device_reservation_request_cb(
    void *userdata,
    rd_device *d,
    int forced)
{
    struct reserved_device * ptr = userdata;

//    jack_info("device_reservation_request_cb");
    assert(ptr->reserved_device == d);

    device_reservation_on_takeover(g_device_reservation_ctx, ptr->device_name);

    (void)forced;
    return 1;                   /* device was released */
}

void
device_reservation_available_cb(
    void *userdata,
    rd_device *d)
{
    struct reserved_device * ptr = userdata;

    assert(ptr->reserved_device == d);

    device_reservation_on_giveback(g_device_reservation_ctx, ptr->device_name);
}

bool device_reservation_acquire(const char * device_name)
{
    DBusError error;   
    int ret;
    struct reserved_device * device_ptr;

    if (g_dbus_connection == NULL)
    {
        jack_error("device reservation requires dbus connection");
        assert(false);
        return false;
    }

    device_ptr = malloc(sizeof(struct reserved_device));
    if (device_ptr == NULL)
    {
        jack_error("OOM in device_reservation_acquire()");
        return false;
    }

    strncpy(device_ptr->device_name, device_name, MAX_DEVICE_NAME);
    if (strcmp(device_ptr->device_name, device_name) != 0)
    {
        jack_error("Ignoring reservation for device with too long name");
        return false;
    }

    dbus_error_init(&error);

    ret = rd_acquire(
	&device_ptr->reserved_device,
	g_dbus_connection,
	device_name,
	"Jack audio server (jackdbus)",
	NULL,
	INT32_MAX - 1000, /* value lower than INT32_MAX
			     allows other process to take over */
	device_ptr,
	device_reservation_request_cb,         /* request device release callback */
	device_reservation_available_cb,       /* device available callback */
	&error);
    if (ret < 0)
    {
        jack_error("Failed to acquire device name : %s error : %s", device_name, (error.message ? error.message : strerror(-ret)));
        dbus_error_free(&error);
        return false;
    }

    list_add_tail(&device_ptr->siblings, &g_device_reservation_list);

    jack_info("Acquire audio card %s", device_name);
    return true;
}

void device_reservation_release(const char * device_name)
{
    struct list_head * node_ptr;
    struct reserved_device * device_ptr;

    // Look for corresponding reserved device
    list_for_each(node_ptr, &g_device_reservation_list)
    {
        device_ptr = list_entry(node_ptr, struct reserved_device, siblings);
        if (strcmp(device_ptr->device_name, device_name) == 0)
        {
            jack_info("Released audio card %s", device_name);
            rd_release(device_ptr->reserved_device, 1);
            return;
        }
    }

    jack_error("Audio card %s not found!!", device_name);
}
