/*
    Copyright (C) 2009 Grame
    Copyright (C) 2024-2025 Nedko Arnaudov
    
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

#ifndef __device_reservation__
#define __device_reservation__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int device_reservation_init(DBusConnection * connection, void * ctx);
int device_reservation_uninit(void);

bool device_reservation_acquire(const char * device_name);
void device_reservation_release(const char * device_name);

/* callbacks */
void device_reservation_on_takeover(void * ctx, const char * device_name);
void device_reservation_on_giveback(void * ctx, const char * device_name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
