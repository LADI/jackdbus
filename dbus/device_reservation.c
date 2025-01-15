/*
    Copyright (C) 2009 Grame
    Copyright (C) 2012-2025 Nedko Arnaudov
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

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

#define DEVICE_MAX 2

/* string like "Audio2", "Midi5" or "Video3" */
#define MAX_DEVICE_NAME 63

struct reserved_audio_device {
     void * ctx;
     char device_name[MAX_DEVICE_NAME+1];
     rd_device * reserved_device;

};

static DBusConnection* gConnection = NULL;
static struct reserved_audio_device gReservedDevice[DEVICE_MAX];
static int gReserveCount = 0;
static void * g_device_reservation_ctx;

int device_reservation_init(DBusConnection* connection, void * ctx)
{
#if 1                           /* jackdbus */
    gConnection = connection;
    g_device_reservation_ctx = ctx;
#else
    DBusError error;
    dbus_error_init(&error);

    if (!(gConnection = dbus_bus_get(DBUS_BUS_SESSION, &error))) {
        jack_error("Failed to connect to session bus for device reservation: %s\n", error.message);
        return -1;
    }
#endif
    jack_info("audio_reservation_init");
    return 0;
}

int device_reservation_finish(void)
{
    int i;

    if (gConnection) {
#if 1                           /* jackdbus */
	for (i = 0; i < DEVICE_MAX; i++) {
	    if (gReservedDevice[i].reserved_device) 
	        rd_release(gReservedDevice[i].reserved_device, 0);
	}
#else
        dbus_connection_unref(gConnection);
#endif
        gConnection = NULL;
        jack_info("audio_reservation_finish");
    }
    return 0;
}

int
device_reservation_request_cb(
    void *userdata,
    rd_device *d,
    int forced)
{
    struct reserved_audio_device * ptr = userdata;

//    jack_info("device_reservation_request_cb");
    assert(ptr->reserved_device == d);

    device_reservation_on_takeover(ptr->ctx, ptr->device_name);

//    rd_release(ptr->reserved_device);
//    memset(ptr, 0, sizeof(struct reserved_audio_device));

    return 1;			/* device was released */
}

void
device_reservation_available_cb(
    void *userdata,
    rd_device *d)
{
    struct reserved_audio_device * ptr = userdata;

    assert(ptr->reserved_device == d);

    device_reservation_on_giveback(ptr->ctx, ptr->device_name);
}

bool device_reservation_acquire(const char * device_name)
{
    DBusError error;   
    int ret;

#if 0 /* user calling device_reservation_init() is to call device_reservation_finish() */
    // Open DBus connection first time
    if (gReserveCount == 0) {
        if (device_reservation_init() != 0) {
            return false;
        }
    }
#else
    if (!gConnection) {
        jack_error("device reservation requires dbus connection");
        assert(false);
        return false;
    }
#endif

    assert(gReserveCount < DEVICE_MAX);

    if (gReserveCount != 0) {
        /* reuse device entry if name matches */
        if (gReserveCount != 1 || strcmp(gReservedDevice[0].device_name, device_name)) {
            jack_error("Ignoring reservation for more than one device (acquire)");
            return false;
        }
        gReserveCount = 0;
    }

    strncpy(gReservedDevice[gReserveCount].device_name, device_name, MAX_DEVICE_NAME);
    if (strcmp(gReservedDevice[gReserveCount].device_name, device_name) != 0)
    {
        jack_error("Ignoring reservation for device with too long name");
        return false;
    }

    dbus_error_init(&error);

    if ((ret= rd_acquire(
                 &gReservedDevice[gReserveCount].reserved_device,
                 gConnection,
                 device_name,
                 "Jack audio server (jackdbus)",
                 NULL,
                 INT32_MAX - 1000, /* value lower than INT32_MAX
				      allows other process to take over */
                 gReservedDevice + gReserveCount,
                 device_reservation_request_cb,		/* request device release callback */
                 device_reservation_available_cb,	/* device available callback */
                 &error)) < 0) {

        jack_error("Failed to acquire device name : %s error : %s", device_name, (error.message ? error.message : strerror(-ret)));
        dbus_error_free(&error);
        return false;
    }

    gReservedDevice[gReserveCount].ctx = g_device_reservation_ctx;
//    rd_set_userdata(gReservedDevice[gReserveCount].reserved_device, gReservedDevice + gReserveCount);

    gReserveCount++;

    jack_info("Acquire audio card %s", device_name);
    return true;
}

void device_reservation_release(const char * device_name)
{
    int i;

    // Look for corresponding reserved device
    for (i = 0; i < DEVICE_MAX; i++) {
 	if (strcmp(gReservedDevice[i].device_name, device_name) == 0)  
	    break;
    }
   
    if (i < DEVICE_MAX) {
        jack_info("Released audio card %s", device_name);
        rd_release(gReservedDevice[i].reserved_device, 1);
    } else {
        jack_error("Audio card %s not found!!", device_name);
    }

//    gReserveCount--;

#if 0 /* user calling device_reservation_init() is to call device_reservation_finish() */
    // Close DBus connection last time
    if (gReserveCount == 0)
        device_reservation_finish();
#endif
}

void device_reservation_loop(void)
{
    if (gConnection != NULL) {
       dbus_connection_read_write_dispatch (gConnection, 200);
    }
}

