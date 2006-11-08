/*
Copyright (C) 2006 Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "jack.h"
#include "JackExports.h"
#include <dlfcn.h>

#ifdef __cplusplus
extern "C"
{
#endif

    EXPORT int jack_client_name_size (void);
    EXPORT char* jack_get_client_name (jack_client_t *client);
    EXPORT int jack_internal_client_new (const char *client_name,
                                         const char *load_name,
                                         const char *load_init);
    EXPORT jack_client_t* my_jack_internal_client_new(const char* client_name);
    EXPORT void jack_internal_client_close (const char *client_name);
    EXPORT void my_jack_internal_client_close (jack_client_t* client);
    EXPORT int jack_is_realtime (jack_client_t *client);
    EXPORT void jack_on_shutdown (jack_client_t *client,
                                  void (*function)(void *arg), void *arg);
    EXPORT int jack_set_process_callback (jack_client_t *client,
                                          JackProcessCallback process_callback,
                                          void *arg);
    EXPORT int jack_set_thread_init_callback (jack_client_t *client,
            JackThreadInitCallback thread_init_callback,
            void *arg);
    EXPORT int jack_set_freewheel_callback (jack_client_t *client,
                                            JackFreewheelCallback freewheel_callback,
                                            void *arg);
    EXPORT int jack_set_freewheel(jack_client_t* client, int onoff);
    EXPORT int jack_set_buffer_size (jack_client_t *client, jack_nframes_t nframes);
    EXPORT int jack_set_buffer_size_callback (jack_client_t *client,
            JackBufferSizeCallback bufsize_callback,
            void *arg);
    EXPORT int jack_set_sample_rate_callback (jack_client_t *client,
            JackSampleRateCallback srate_callback,
            void *arg);
    EXPORT int jack_set_port_registration_callback (jack_client_t *,
            JackPortRegistrationCallback
            registration_callback, void *arg);
    EXPORT int jack_set_graph_order_callback (jack_client_t *,
            JackGraphOrderCallback graph_callback,
            void *);
    EXPORT int jack_set_xrun_callback (jack_client_t *,
                                       JackXRunCallback xrun_callback, void *arg);
    EXPORT int jack_activate (jack_client_t *client);
    EXPORT int jack_deactivate (jack_client_t *client);
    EXPORT jack_port_t * jack_port_register (jack_client_t *client,
            const char *port_name,
            const char *port_type,
            unsigned long flags,
            unsigned long buffer_size);
    EXPORT int jack_port_unregister (jack_client_t *, jack_port_t *);
    EXPORT void * jack_port_get_buffer (jack_port_t *, jack_nframes_t);
    EXPORT const char * jack_port_name (const jack_port_t *port);
    EXPORT const char * jack_port_short_name (const jack_port_t *port);
    EXPORT int jack_port_flags (const jack_port_t *port);
    EXPORT const char * jack_port_type (const jack_port_t *port);
    EXPORT int jack_port_is_mine (const jack_client_t *, const jack_port_t *port);
    EXPORT int jack_port_connected (const jack_port_t *port);
    EXPORT int jack_port_connected_to (const jack_port_t *port,
                                       const char *port_name);
    EXPORT const char ** jack_port_get_connections (const jack_port_t *port);
    EXPORT const char ** jack_port_get_all_connections (const jack_client_t *client,
            const jack_port_t *port);
    EXPORT int jack_port_tie (jack_port_t *src, jack_port_t *dst);
    EXPORT int jack_port_untie (jack_port_t *port);
    EXPORT int jack_port_lock (jack_client_t *, jack_port_t *);
    EXPORT int jack_port_unlock (jack_client_t *, jack_port_t *);
    EXPORT jack_nframes_t jack_port_get_latency (jack_port_t *port);
    EXPORT jack_nframes_t jack_port_get_total_latency (jack_client_t *,
            jack_port_t *port);
    EXPORT void jack_port_set_latency (jack_port_t *, jack_nframes_t);
    EXPORT int jack_recompute_total_latencies (jack_client_t*);
    EXPORT int jack_port_set_name (jack_port_t *port, const char *port_name);
    EXPORT int jack_port_request_monitor (jack_port_t *port, int onoff);
    EXPORT int jack_port_request_monitor_by_name (jack_client_t *client,
            const char *port_name, int onoff);
    EXPORT int jack_port_ensure_monitor (jack_port_t *port, int onoff);
    EXPORT int jack_port_monitoring_input (jack_port_t *port);
    EXPORT int jack_connect (jack_client_t *,
                             const char *source_port,
                             const char *destination_port);
    EXPORT int jack_disconnect (jack_client_t *,
                                const char *source_port,
                                const char *destination_port);
    EXPORT int jack_port_disconnect (jack_client_t *, jack_port_t *);
    EXPORT int jack_port_name_size(void);
    EXPORT int jack_port_type_size(void);
    EXPORT jack_nframes_t jack_get_sample_rate (jack_client_t *);
    EXPORT jack_nframes_t jack_get_buffer_size (jack_client_t *);
    EXPORT const char ** jack_get_ports (jack_client_t *,
                                         const char *port_name_pattern,
                                         const char *type_name_pattern,
                                         unsigned long flags);
    EXPORT jack_port_t * jack_port_by_name (jack_client_t *, const char *port_name);
    EXPORT jack_port_t * jack_port_by_id (jack_client_t *client,
                                          jack_port_id_t port_id);
    EXPORT int jack_engine_takeover_timebase (jack_client_t *);
    EXPORT jack_nframes_t jack_frames_since_cycle_start (const jack_client_t *);
    EXPORT jack_nframes_t jack_frame_time (const jack_client_t *);
    EXPORT jack_nframes_t jack_last_frame_time (const jack_client_t *client);
    EXPORT float jack_cpu_load (jack_client_t *client);
    EXPORT pthread_t jack_client_thread_id (jack_client_t *);
    EXPORT void jack_set_error_function (void (*func)(const char *));

    EXPORT float jack_get_max_delayed_usecs (jack_client_t *client);
    EXPORT float jack_get_xrun_delayed_usecs (jack_client_t *client);
    EXPORT void jack_reset_max_delayed_usecs (jack_client_t *client);

    EXPORT int jack_release_timebase (jack_client_t *client);
    EXPORT int jack_set_sync_callback (jack_client_t *client,
                                       JackSyncCallback sync_callback,
                                       void *arg);
    EXPORT int jack_set_sync_timeout (jack_client_t *client,
                                      jack_time_t timeout);
    EXPORT int jack_set_timebase_callback (jack_client_t *client,
                                           int conditional,
                                           JackTimebaseCallback timebase_callback,
                                           void *arg);
    EXPORT int jack_transport_locate (jack_client_t *client,
                                      jack_nframes_t frame);
    EXPORT jack_transport_state_t jack_transport_query (const jack_client_t *client,
            jack_position_t *pos);
    EXPORT jack_nframes_t jack_get_current_transport_frame (const jack_client_t *client);
    EXPORT int jack_transport_reposition (jack_client_t *client,
                                          jack_position_t *pos);
    EXPORT void jack_transport_start (jack_client_t *client);
    EXPORT void jack_transport_stop (jack_client_t *client);
    EXPORT void jack_get_transport_info (jack_client_t *client,
                                         jack_transport_info_t *tinfo);
    EXPORT void jack_set_transport_info (jack_client_t *client,
                                         jack_transport_info_t *tinfo);

    EXPORT int jack_acquire_real_time_scheduling (pthread_t thread, int priority);
    EXPORT int jack_client_create_thread (jack_client_t* client,
                                          pthread_t *thread,
                                          int priority,
                                          int realtime, 	// boolean
                                          void *(*start_routine)(void*),
                                          void *arg);
    EXPORT int jack_drop_real_time_scheduling (pthread_t thread);

    EXPORT char * jack_get_internal_client_name (jack_client_t *client,
            jack_intclient_t intclient);
    EXPORT jack_intclient_t jack_internal_client_handle (jack_client_t *client,
            const char *client_name,
            jack_status_t *status);
    EXPORT jack_intclient_t jack_internal_client_load (jack_client_t *client,
            const char *client_name,
            jack_options_t options,
            jack_status_t *status, ...);
    EXPORT jack_status_t jack_internal_client_unload (jack_client_t *client,
            jack_intclient_t intclient);
			
	EXPORT jack_client_t * jack_client_open (const char *client_name,
            jack_options_t options,
            jack_status_t *status, ...);
    EXPORT jack_client_t * jack_client_new (const char *client_name);
    EXPORT int jack_client_close (jack_client_t *client);

#ifdef __cplusplus
}
#endif

// Function definition

typedef void* (*jack_port_get_buffer_fun_def)(jack_port_t* port, jack_nframes_t frames);
static jack_port_get_buffer_fun_def jack_port_get_buffer_fun = 0;
EXPORT void* jack_port_get_buffer(jack_port_t* port, jack_nframes_t frames)
{
	return jack_port_get_buffer_fun(port, frames);
}

typedef const char* (*jack_port_name_fun_def)(const jack_port_t* port);
static jack_port_name_fun_def jack_port_name_fun = 0;
EXPORT const char* jack_port_name(const jack_port_t* port)
{
    return jack_port_name_fun(port);
}

typedef const char* (*jack_port_short_name_fun_def) (const jack_port_t* port);
static jack_port_short_name_fun_def jack_port_short_name_fun = 0;
EXPORT const char* jack_port_short_name(const jack_port_t* port)
{
    return jack_port_short_name_fun(port);
}

typedef int (*jack_port_flags_fun_def)(const jack_port_t* port);
static jack_port_flags_fun_def jack_port_flags_fun = 0;
EXPORT int jack_port_flags(const jack_port_t* port)
{
    return jack_port_flags_fun(port);
}

typedef const char* (*jack_port_type_fun_def)(const jack_port_t* port);
static jack_port_type_fun_def jack_port_type_fun = 0;
EXPORT const char* jack_port_type(const jack_port_t* port)
{
    return jack_port_type_fun(port);
}

typedef int (*jack_port_connected_fun_def)(const jack_port_t* port);
static jack_port_connected_fun_def jack_port_connected_fun = 0;
EXPORT int jack_port_connected(const jack_port_t* port)
{
    return jack_port_connected_fun(port);
}

typedef int (*jack_port_connected_to_fun_def)(const jack_port_t* port, const char* portname);
static jack_port_connected_to_fun_def jack_port_connected_to_fun = 0;
EXPORT int jack_port_connected_to(const jack_port_t* port, const char* portname)
{
    return jack_port_connected_to_fun(port, portname);
}

typedef int (*jack_port_tie_fun_def)(jack_port_t* src, jack_port_t* dst);
static jack_port_tie_fun_def jack_port_tie_fun = 0;
EXPORT int jack_port_tie(jack_port_t* src, jack_port_t* dst)
{
    return jack_port_tie_fun(src, dst);
}

typedef int (*jack_port_untie_fun_def)(jack_port_t* port);
static jack_port_untie_fun_def jack_port_untie_fun = 0;
EXPORT int jack_port_untie(jack_port_t* port)
{
    return jack_port_untie_fun(port);
}

typedef jack_nframes_t (*jack_port_get_latency_fun_def)(jack_port_t* port);
static jack_port_get_latency_fun_def jack_port_get_latency_fun = 0;
EXPORT jack_nframes_t jack_port_get_latency(jack_port_t* port)
{
     return jack_port_get_latency(port);
}

typedef void (*jack_port_set_latency_fun_def)(jack_port_t* port, jack_nframes_t frames);
static jack_port_set_latency_fun_def jack_port_set_latency_fun = 0;
EXPORT void jack_port_set_latency(jack_port_t* port, jack_nframes_t frames)
{
    jack_port_set_latency(port, frames);
}

typedef int (*jack_recompute_total_latencies_fun_def)(jack_client_t* ext_client);
static jack_recompute_total_latencies_fun_def jack_recompute_total_latencies_fun = 0;
EXPORT int jack_recompute_total_latencies(jack_client_t* ext_client)
{
	return jack_recompute_total_latencies_fun(ext_client);
}

typedef int (*jack_port_set_name_fun_def)(jack_port_t* port, const char* name);
static jack_port_set_name_fun_def jack_port_set_name_fun = 0;
EXPORT int jack_port_set_name(jack_port_t* port, const char* name)
{
    return jack_port_set_name_fun(port, name);
}

typedef int (*jack_port_request_monitor_fun_def)(jack_port_t* port, int onoff);
static jack_port_request_monitor_fun_def jack_port_request_monitor_fun = 0;
EXPORT int jack_port_request_monitor(jack_port_t* port, int onoff)
{
    return jack_port_request_monitor_fun(port, onoff);
}

typedef int (*jack_port_request_monitor_by_name_fun_def)(jack_client_t* ext_client, const char* port_name, int onoff);
static jack_port_request_monitor_by_name_fun_def jack_port_request_monitor_by_name_fun = 0;
EXPORT int jack_port_request_monitor_by_name(jack_client_t* ext_client, const char* port_name, int onoff)
{
    return jack_port_request_monitor_by_name_fun(ext_client, port_name, onoff);
}

typedef int (*jack_port_ensure_monitor_fun_def)(jack_port_t* port, int onoff);
static jack_port_ensure_monitor_fun_def jack_port_ensure_monitor_fun = 0;
EXPORT int jack_port_ensure_monitor(jack_port_t* port, int onoff)
{
    return jack_port_ensure_monitor_fun(port, onoff);
}

typedef int (*jack_port_monitoring_input_fun_def)(jack_port_t* port);
static jack_port_monitoring_input_fun_def jack_port_monitoring_input_fun = 0;
EXPORT int jack_port_monitoring_input(jack_port_t* port)
{
    return jack_port_monitoring_input_fun(port);
}

typedef int (*jack_is_realtime_fun_def)(jack_client_t* ext_client);
static jack_is_realtime_fun_def jack_is_realtime_fun = 0;
EXPORT int jack_is_realtime(jack_client_t* ext_client)
{
    return jack_is_realtime_fun(ext_client);
}

typedef void (*shutdown_fun)(void* arg);
typedef void (*jack_on_shutdown_fun_def)(jack_client_t* ext_client, shutdown_fun callback, void* arg);
static jack_on_shutdown_fun_def jack_on_shutdown_fun = 0;
EXPORT void jack_on_shutdown(jack_client_t* ext_client, shutdown_fun callback, void* arg)
{
    return jack_on_shutdown_fun(ext_client, callback, arg);
}

typedef int (*jack_set_process_callback_fun_def)(jack_client_t* ext_client, JackProcessCallback callback, void* arg);
static jack_set_process_callback_fun_def jack_set_process_callback_fun = 0;
EXPORT int jack_set_process_callback(jack_client_t* ext_client, JackProcessCallback callback, void* arg)
{
    return jack_set_process_callback_fun(ext_client, callback, arg);
}

typedef int (*jack_set_freewheel_callback_fun_def)(jack_client_t* ext_client, JackFreewheelCallback freewheel_callback, void* arg);
static jack_set_freewheel_callback_fun_def jack_set_freewheel_callback_fun = 0;
EXPORT int jack_set_freewheel_callback(jack_client_t* ext_client, JackFreewheelCallback freewheel_callback, void* arg)
{
    return jack_set_freewheel_callback_fun(ext_client, freewheel_callback, arg);
}

typedef int (*jack_set_freewheel_fun_def)(jack_client_t* ext_client, int onoff);
static jack_set_freewheel_fun_def jack_set_freewheel_fun = 0;
EXPORT int jack_set_freewheel(jack_client_t* ext_client, int onoff)
{
    return jack_set_freewheel_fun(ext_client, onoff);
}

typedef int (*jack_set_buffer_size_fun_def)(jack_client_t* ext_client, jack_nframes_t buffer_size);
static jack_set_buffer_size_fun_def jack_set_buffer_size_fun = 0;
EXPORT int jack_set_buffer_size(jack_client_t* ext_client, jack_nframes_t buffer_size)
{
    return jack_set_buffer_size_fun(ext_client, buffer_size);
}

typedef int (*jack_set_buffer_size_callback_fun_def)(jack_client_t* ext_client, JackBufferSizeCallback bufsize_callback, void* arg);
static jack_set_buffer_size_callback_fun_def jack_set_buffer_size_callback_fun = 0;
EXPORT int jack_set_buffer_size_callback(jack_client_t* ext_client, JackBufferSizeCallback bufsize_callback, void* arg)
{
    return jack_set_buffer_size_callback_fun(ext_client, bufsize_callback, arg);
}

typedef int (*jack_set_sample_rate_callback_fun_def)(jack_client_t* ext_client, JackSampleRateCallback srate_callback, void* arg);
static jack_set_sample_rate_callback_fun_def jack_set_sample_rate_callback_fun = 0;
EXPORT int jack_set_sample_rate_callback(jack_client_t* ext_client, JackSampleRateCallback srate_callback, void* arg)
{
    return jack_set_sample_rate_callback_fun(ext_client, srate_callback, arg);
}

typedef int (*jack_set_port_registration_callback_fun_def)(jack_client_t* ext_client, JackPortRegistrationCallback registration_callback, void* arg);
static jack_set_port_registration_callback_fun_def jack_set_port_registration_callback_fun = 0;
EXPORT int jack_set_port_registration_callback(jack_client_t* ext_client, JackPortRegistrationCallback registration_callback, void* arg)
{
    return jack_set_port_registration_callback_fun(ext_client, registration_callback, arg);
}

typedef int (*jack_set_graph_order_callback_fun_def)(jack_client_t* ext_client, JackGraphOrderCallback graph_callback, void* arg);
static jack_set_graph_order_callback_fun_def jack_set_graph_order_callback_fun = 0;
EXPORT int jack_set_graph_order_callback(jack_client_t* ext_client, JackGraphOrderCallback graph_callback, void* arg)
{
    return jack_set_graph_order_callback_fun(ext_client, graph_callback, arg);
}

typedef int (*jack_set_xrun_callback_fun_def)(jack_client_t* ext_client, JackXRunCallback xrun_callback, void* arg);
static jack_set_xrun_callback_fun_def jack_set_xrun_callback_fun = 0;
EXPORT int jack_set_xrun_callback(jack_client_t* ext_client, JackXRunCallback xrun_callback, void* arg)
{
    return jack_set_xrun_callback_fun(ext_client, xrun_callback, arg);
}

typedef int (*jack_set_thread_init_callback_fun_def)(jack_client_t* ext_client, JackThreadInitCallback init_callback, void *arg);
static jack_set_thread_init_callback_fun_def jack_set_thread_init_callback_fun = 0;
EXPORT int jack_set_thread_init_callback(jack_client_t* ext_client, JackThreadInitCallback init_callback, void *arg)
{
    return jack_set_thread_init_callback_fun(ext_client, init_callback, arg);
}

typedef int (*jack_activate_fun_def)(jack_client_t* ext_client);
static jack_activate_fun_def jack_activate_fun = 0;
EXPORT int jack_activate(jack_client_t* ext_client)
{
    return jack_activate_fun(ext_client);
}

typedef int (*jack_deactivate_fun_def)(jack_client_t* ext_client);
static jack_deactivate_fun_def jack_deactivate_fun = 0;
EXPORT int jack_deactivate(jack_client_t* ext_client)
{
    return jack_deactivate_fun(ext_client);
}

typedef jack_port_t* (*jack_port_register_fun_def)(jack_client_t* ext_client, const char* port_name, const char* port_type, unsigned long flags, unsigned long buffer_size);
static jack_port_register_fun_def jack_port_register_fun = 0;
EXPORT jack_port_t* jack_port_register(jack_client_t* ext_client, const char* port_name, const char* port_type, unsigned long flags, unsigned long buffer_size)
{
    return jack_port_register_fun(ext_client, port_name, port_type, flags, buffer_size);
}

typedef int (*jack_port_unregister_fun_def)(jack_client_t* ext_client, jack_port_t* port);
static jack_port_unregister_fun_def jack_port_unregister_fun = 0;
EXPORT int jack_port_unregister(jack_client_t* ext_client, jack_port_t* port)
{
    return jack_port_unregister_fun(ext_client, port);
}

typedef int (*jack_port_is_mine_fun_def)(const jack_client_t* ext_client, const jack_port_t* port);
static jack_port_is_mine_fun_def jack_port_is_mine_fun = 0;
EXPORT int jack_port_is_mine(const jack_client_t* ext_client, const jack_port_t* port)
{
    return jack_port_is_mine_fun(ext_client, port);
}

typedef const char** (*jack_port_get_connections_fun_def)(const jack_port_t* port);
static jack_port_get_connections_fun_def jack_port_get_connections_fun = 0;
EXPORT const char** jack_port_get_connections(const jack_port_t* port)
{
    return jack_port_get_connections_fun(port);
}

// Calling client does not need to "own" the port
typedef const char** (*jack_port_get_all_connections_fun_def)(const jack_client_t* ext_client, const jack_port_t* port);
static jack_port_get_all_connections_fun_def jack_port_get_all_connections_fun = 0;
EXPORT const char** jack_port_get_all_connections(const jack_client_t* ext_client, const jack_port_t* port)
{
    return jack_port_get_all_connections_fun(ext_client, port);
}

// Does not use the client parameter
typedef int (*jack_port_lock_fun_def)(jack_client_t* ext_client, jack_port_t* port);
static jack_port_lock_fun_def jack_port_lock_fun = 0;
EXPORT int jack_port_lock(jack_client_t* ext_client, jack_port_t* port)
{
    return jack_port_lock_fun(ext_client, port);
}

// Does not use the client parameter
typedef int (*jack_port_unlock_fun_def)(jack_client_t* ext_client, jack_port_t* port);
static jack_port_unlock_fun_def jack_port_unlock_fun = 0;
EXPORT int jack_port_ununlock(jack_client_t* ext_client, jack_port_t* port)
{
    return jack_port_unlock_fun(ext_client, port);
}

typedef jack_nframes_t (*jack_port_get_total_latency_fun_def)(jack_client_t* ext_client, jack_port_t* port);
static jack_port_get_total_latency_fun_def jack_port_get_total_latency_fun = 0;
EXPORT jack_nframes_t jack_port_get_total_latency(jack_client_t* ext_client, jack_port_t* port)
{
    return jack_port_get_total_latency_fun(ext_client, port);
}

typedef int (*jack_connect_fun_def)(jack_client_t* ext_client, const char* src, const char* dst);
static jack_connect_fun_def jack_connect_fun = 0;
EXPORT int jack_connect(jack_client_t* ext_client, const char* src, const char* dst)
{
    return jack_connect_fun(ext_client, src, dst);
}

typedef int (*jack_disconnect_fun_def)(jack_client_t* ext_client, const char* src, const char* dst);
static jack_disconnect_fun_def jack_disconnect_fun = 0;
EXPORT int jack_disconnect(jack_client_t* ext_client, const char* src, const char* dst)
{
    return jack_disconnect_fun(ext_client, src, dst);
}

typedef int (*jack_port_connect_fun_def)(jack_client_t* ext_client, jack_port_t* src, jack_port_t* dst);
static jack_port_connect_fun_def jack_port_connect_fun = 0;
EXPORT int jack_port_connect(jack_client_t* ext_client, jack_port_t* src, jack_port_t* dst)
{
    return jack_port_connect_fun(ext_client, src, dst);
}

typedef int (*jack_port_disconnect_fun_def)(jack_client_t* ext_client, jack_port_t* src);
static jack_port_disconnect_fun_def jack_port_disconnect_fun = 0;
EXPORT int jack_port_disconnect(jack_client_t* ext_client, jack_port_t* src)
{
    return jack_port_disconnect_fun(ext_client, src);
}

typedef jack_nframes_t (*jack_get_sample_rate_fun_def)(jack_client_t* ext_client);
static jack_get_sample_rate_fun_def jack_get_sample_rate_fun = 0;
EXPORT jack_nframes_t jack_get_sample_rate(jack_client_t* ext_client)
{
    return jack_get_sample_rate_fun(ext_client);
}

typedef jack_nframes_t (*jack_get_buffer_size_fun_def)(jack_client_t* ext_client);
static jack_get_buffer_size_fun_def jack_get_buffer_size_fun = 0;
EXPORT jack_nframes_t jack_get_buffer_size(jack_client_t* ext_client)
{
	return jack_get_buffer_size_fun(ext_client);
}

typedef const char** (*jack_get_ports_fun_def)(jack_client_t* ext_client, const char* port_name_pattern, const char* type_name_pattern, unsigned long flags);
static jack_get_ports_fun_def jack_get_ports_fun = 0;
EXPORT const char** jack_get_ports(jack_client_t* ext_client, const char* port_name_pattern, const char* type_name_pattern, unsigned long flags)
{
    return jack_get_ports_fun(ext_client, port_name_pattern, type_name_pattern, flags);
}

typedef jack_port_t* (*jack_port_by_name_fun_def)(jack_client_t* ext_client, const char* portname);
static jack_port_by_name_fun_def jack_port_by_name_fun = 0;
EXPORT jack_port_t* jack_port_by_name(jack_client_t* ext_client, const char* portname)
{
    return jack_port_by_name(ext_client, portname);
}

typedef jack_port_t* (*jack_port_by_id_fun_def)(const jack_client_t* ext_client, jack_port_id_t id);
static jack_port_by_id_fun_def jack_port_by_id_fun = 0;
EXPORT jack_port_t* jack_port_by_id(const jack_client_t* ext_client, jack_port_id_t id)
{
    return jack_port_by_id_fun(ext_client, id);
}

typedef int (*jack_engine_takeover_timebase_fun_def)(jack_client_t* ext_client);
static jack_engine_takeover_timebase_fun_def jack_engine_takeover_timebase_fun = 0;
EXPORT int jack_engine_takeover_timebase(jack_client_t* ext_client)
{
    return jack_engine_takeover_timebase_fun(ext_client);
}

typedef jack_nframes_t (*jack_frames_since_cycle_start_fun_def)(const jack_client_t* ext_client);
static jack_frames_since_cycle_start_fun_def jack_frames_since_cycle_start_fun = 0;
EXPORT jack_nframes_t jack_frames_since_cycle_start(const jack_client_t* ext_client)
{
    return jack_frames_since_cycle_start_fun(ext_client);
}

typedef jack_nframes_t (*jack_frame_time_fun_def)(const jack_client_t* ext_client);
static jack_frame_time_fun_def jack_frame_time_fun = 0;
EXPORT jack_nframes_t jack_frame_time(const jack_client_t* ext_client)
{
    return jack_frame_time_fun(ext_client);
}

typedef jack_nframes_t (*jack_last_frame_time_fun_def)(const jack_client_t* ext_client);
static jack_last_frame_time_fun_def jack_last_frame_time_fun = 0;
EXPORT jack_nframes_t jack_last_frame_time(const jack_client_t* ext_client)
{
    return jack_last_frame_time_fun(ext_client);
}

typedef float (*jack_cpu_load_fun_def)(jack_client_t* ext_client);
static jack_cpu_load_fun_def jack_cpu_load_fun = 0;
EXPORT float jack_cpu_load(jack_client_t* ext_client)
{
    return jack_cpu_load_fun(ext_client);
}

typedef pthread_t (*jack_client_thread_id_fun_def)(jack_client_t* ext_client);
static jack_client_thread_id_fun_def jack_client_thread_id_fun = 0;
EXPORT pthread_t  jack_client_thread_id(jack_client_t* ext_client)
{
    return jack_client_thread_id_fun(ext_client);
}

typedef char* (*jack_get_client_name_fun_def)(jack_client_t* ext_client);
static jack_get_client_name_fun_def jack_get_client_name_fun = 0;
EXPORT char* jack_get_client_name (jack_client_t* ext_client)
{
    return jack_get_client_name_fun(ext_client);
}

typedef int (*jack_client_name_size_fun_def)(void);
static jack_client_name_size_fun_def jack_client_name_size_fun = 0;
EXPORT int jack_client_name_size(void)
{
    return jack_client_name_size_fun();
}

typedef int (*jack_port_name_size_fun_def)(void);
static jack_port_name_size_fun_def jack_port_name_size_fun = 0;
EXPORT int jack_port_name_size(void)
{
    return jack_port_name_size_fun();
}

// transport.h

typedef int (*jack_release_timebase_fun_def)(jack_client_t* ext_client);
static jack_release_timebase_fun_def jack_release_timebase_fun = 0;
EXPORT int jack_release_timebase(jack_client_t* ext_client)
{
    return jack_release_timebase_fun(ext_client);
}

typedef int (*jack_set_sync_callback_fun_def)(jack_client_t* ext_client, JackSyncCallback sync_callback, void *arg);
static jack_set_sync_callback_fun_def jack_set_sync_callback_fun = 0;
EXPORT int jack_set_sync_callback(jack_client_t* ext_client, JackSyncCallback sync_callback, void *arg)
{
    return jack_set_sync_callback_fun(ext_client, sync_callback, arg);
}

typedef int (*jack_set_sync_timeout_fun_def)(jack_client_t* ext_client, jack_time_t timeout);
static jack_set_sync_timeout_fun_def jack_set_sync_timeout_fun = 0;
EXPORT int jack_set_sync_timeout(jack_client_t* ext_client, jack_time_t timeout)
{
    return jack_set_sync_timeout_fun(ext_client, timeout);
}

typedef int (*jack_set_timebase_callback_fun_def)(jack_client_t* ext_client, int conditional, JackTimebaseCallback timebase_callback, void* arg);
static jack_set_timebase_callback_fun_def jack_set_timebase_callback_fun = 0;
EXPORT int jack_set_timebase_callback(jack_client_t* ext_client, int conditional, JackTimebaseCallback timebase_callback, void* arg)
{
    return jack_set_timebase_callback_fun(ext_client, conditional, timebase_callback, arg);
}

typedef int (*jack_transport_locate_fun_def)(jack_client_t* ext_client, jack_nframes_t frame);
static jack_transport_locate_fun_def jack_transport_locate_fun = 0;
EXPORT int jack_transport_locate(jack_client_t* ext_client, jack_nframes_t frame)
{
    return jack_transport_locate_fun(ext_client, frame);
}

typedef jack_transport_state_t (*jack_transport_query_fun_def)(const jack_client_t* ext_client, jack_position_t* pos);
static jack_transport_query_fun_def jack_transport_query_fun = 0;
EXPORT jack_transport_state_t jack_transport_query(const jack_client_t* ext_client, jack_position_t* pos)
{
    return jack_transport_query_fun(ext_client, pos);
}

typedef jack_nframes_t (*jack_get_current_transport_frame_fun_def)(const jack_client_t* ext_client);
static jack_get_current_transport_frame_fun_def jack_get_current_transport_frame_fun = 0;
EXPORT jack_nframes_t jack_get_current_transport_frame(const jack_client_t* ext_client)
{
    return jack_get_current_transport_frame_fun(ext_client);
}

typedef int (*jack_transport_reposition_fun_def)(jack_client_t* ext_client, jack_position_t* pos);
static jack_transport_reposition_fun_def jack_transport_reposition_fun = 0;
EXPORT int jack_transport_reposition(jack_client_t* ext_client, jack_position_t* pos)
{
    return jack_transport_reposition_fun(ext_client, pos);
}

typedef void (*jack_transport_start_fun_def)(jack_client_t* ext_client);
static jack_transport_start_fun_def jack_transport_start_fun = 0;
EXPORT void jack_transport_start(jack_client_t* ext_client)
{
    return jack_transport_start_fun(ext_client);
}

typedef void (*jack_transport_stop_fun_def)(jack_client_t* ext_client);
static jack_transport_stop_fun_def jack_transport_stop_fun = 0;
EXPORT void jack_transport_stop(jack_client_t* ext_client)
{
	return jack_transport_stop_fun(ext_client);
}

// deprecated

typedef void (*jack_get_transport_info_fun_def)(jack_client_t* ext_client, jack_transport_info_t* tinfo);
static jack_get_transport_info_fun_def jack_get_transport_info_fun = 0;
EXPORT void jack_get_transport_info(jack_client_t* ext_client, jack_transport_info_t* tinfo)
{
    jack_get_transport_info_fun(ext_client, tinfo);
}

typedef void (*jack_set_transport_info_fun_def)(jack_client_t* ext_client, jack_transport_info_t* tinfo);
static jack_set_transport_info_fun_def jack_set_transport_info_fun = 0;
EXPORT void jack_set_transport_info(jack_client_t* ext_client, jack_transport_info_t* tinfo)
{
    jack_set_transport_info_fun(ext_client, tinfo);
}

// statistics.h

typedef float (*jack_get_max_delayed_usecs_fun_def)(jack_client_t* ext_client);
static jack_get_max_delayed_usecs_fun_def jack_get_max_delayed_usecs_fun = 0;
EXPORT float jack_get_max_delayed_usecs(jack_client_t* ext_client)
{
    return jack_get_max_delayed_usecs_fun(ext_client);
}

typedef float (*jack_get_xrun_delayed_usecs_fun_def)(jack_client_t* ext_client);
static jack_get_xrun_delayed_usecs_fun_def jack_get_xrun_delayed_usecs_fun = 0;
EXPORT float jack_get_xrun_delayed_usecs(jack_client_t* ext_client)
{
    return jack_get_xrun_delayed_usecs_fun(ext_client);
}

typedef void (*jack_reset_max_delayed_usecs_fun_def)(jack_client_t* ext_client);
static jack_reset_max_delayed_usecs_fun_def jack_reset_max_delayed_usecs_fun = 0;
EXPORT void jack_reset_max_delayed_usecs(jack_client_t* ext_client)
{
    jack_reset_max_delayed_usecs(ext_client);
}

// thread.h

typedef int (*jack_acquire_real_time_scheduling_fun_def)(pthread_t thread, int priority);
static jack_acquire_real_time_scheduling_fun_def jack_acquire_real_time_scheduling_fun = 0;
EXPORT int jack_acquire_real_time_scheduling(pthread_t thread, int priority)
{
    return jack_acquire_real_time_scheduling_fun(thread, priority);
}

typedef void *(*start_routine)(void*);
typedef int (*jack_client_create_thread_fun_def)(jack_client_t* client,
												pthread_t *thread,
												int priority,
												int realtime,  	// boolean 
												start_routine callback,
												void *arg);
static jack_client_create_thread_fun_def jack_client_create_thread_fun = 0;
EXPORT int jack_client_create_thread(jack_client_t* client,
                                     pthread_t *thread,
                                     int priority,
                                     int realtime,  	// boolean 
                                     start_routine callback,
                                     void *arg)
{
    return jack_client_create_thread_fun(client, thread, priority, realtime, callback, arg);
}

typedef int (*jack_drop_real_time_scheduling_fun_def)(pthread_t thread);
static jack_drop_real_time_scheduling_fun_def jack_drop_real_time_scheduling_fun = 0;
EXPORT int jack_drop_real_time_scheduling(pthread_t thread)
{
    return jack_drop_real_time_scheduling_fun(thread);
}

// intclient.h

typedef char* (*jack_get_internal_client_name_fun_def)(jack_client_t* ext_client, jack_intclient_t intclient);
static jack_get_internal_client_name_fun_def jack_get_internal_client_name_fun = 0;
EXPORT char* jack_get_internal_client_name(jack_client_t* ext_client, jack_intclient_t intclient)
{
    return jack_get_internal_client_name_fun(ext_client, intclient);
}

typedef jack_intclient_t (*jack_internal_client_handle_fun_def)(jack_client_t* ext_client, const char* client_name, jack_status_t* status);
static jack_internal_client_handle_fun_def jack_internal_client_handle_fun = 0;
EXPORT jack_intclient_t jack_internal_client_handle(jack_client_t* ext_client, const char* client_name, jack_status_t* status)
{
    return jack_internal_client_handle_fun(ext_client, client_name, status);
}

typedef jack_intclient_t (*jack_internal_client_load_fun_def)(jack_client_t* ext_client, const char* client_name, jack_options_t options, jack_status_t* status, ...);
static jack_internal_client_load_fun_def jack_internal_client_load_fun = 0;
EXPORT jack_intclient_t jack_internal_client_load(jack_client_t* ext_client, const char* client_name, jack_options_t options, jack_status_t* status, ...)
{
	va_list ap;
	va_start(ap, status);
	jack_intclient_t res =  jack_internal_client_load_fun(ext_client, client_name, options, status, ap);
	va_end(ap);
	return res;
}

typedef jack_status_t (*jack_internal_client_unload_fun_def)(jack_client_t* ext_client, jack_intclient_t intclient);
static jack_internal_client_unload_fun_def jack_internal_client_unload_fun = 0;
EXPORT jack_status_t jack_internal_client_unload(jack_client_t* ext_client, jack_intclient_t intclient)
{
    return jack_internal_client_unload_fun(ext_client, intclient);
}

// client
static long gClientCount = 0;
static void* gLibrary = 0;
static bool init_library();
static bool open_library();
static void close_library();

typedef jack_client_t * (*jack_client_open_fun_def)(const char *client_name, jack_options_t options, jack_status_t *status, ...);
static jack_client_open_fun_def jack_client_open_fun = 0;
EXPORT jack_client_t * jack_client_open(const char *client_name, jack_options_t options, jack_status_t *status, ...)
{
	// Library check...
	if (!open_library()) 
		return 0;
		
	va_list ap;
	va_start(ap, status);
	jack_client_t* res = jack_client_open_fun(client_name, options, status, ap);
	va_end(ap);
	return res;
}

typedef jack_client_t * (*jack_client_new_fun_def)(const char *client_name);
static jack_client_new_fun_def jack_client_new_fun = 0;
EXPORT jack_client_t * jack_client_new(const char *client_name)
{
	// Library check...
	if (!open_library()) 
		return 0;
		
	return jack_client_new_fun(client_name);
}

typedef int (*jack_client_close_fun_def)(jack_client_t *client);
static jack_client_close_fun_def jack_client_close_fun = 0;
EXPORT int jack_client_close(jack_client_t *client)
{
	close_library();
	return jack_client_close_fun(client);
}

// Library loader
static bool open_library()
{
	if (gClientCount++ == 0) {
		return init_library();
	} else {
		return true;
	}
}

static void close_library()
{
	if (--gClientCount == 0) {
		dlclose(gLibrary);
	} 
}

static bool check_client(void* library)
{
	jack_client_t* client = 0; 
	
	// Get "new" and "close" entry points...
	jack_client_new_fun = (jack_client_new_fun_def)dlsym(library, "jack_client_new");
	jack_client_close_fun = (jack_client_close_fun_def)dlsym(library, "jack_client_close");
		
	// Try opening a client...
	if ((client = jack_client_new_fun("dummy"))) { // jackd server is running....
		jack_client_close_fun(client);
		return true;
	} else {
		return false;
	}
}

static bool init_library()
{
	void* jackLibrary = dlopen("libjack.so", RTLD_LAZY);
	void* jackmpLibrary = dlopen("libjackmp.so", RTLD_LAZY);
	
	if (jackLibrary) {
		
		if (check_client(jackLibrary)) { // jackd is running...
			gLibrary = jackLibrary;
			if (jackmpLibrary) dlclose(jackmpLibrary);
		} else if (check_client(jackmpLibrary)) { // jackdmp is running...
			gLibrary = jackmpLibrary;
			if (jackLibrary) dlclose(jackLibrary);
		} else {
			goto error;
		}
		
	} else if (jackmpLibrary) { 
	
		if (check_client(jackmpLibrary)) { // jackd is running...
			gLibrary = jackmpLibrary;
		} else {
			goto error;
		}
		
	} else {
		printf("Jack libraries not found, failure...\n");
		goto error;
	}
	
	// Load entry points...
	jack_port_get_buffer_fun = (jack_port_get_buffer_fun_def)dlsym(gLibrary, "jack_port_get_buffer");
	jack_port_name_fun = (jack_port_name_fun_def)dlsym(gLibrary, "jack_port_name");
	jack_port_short_name_fun = (jack_port_short_name_fun_def)dlsym(gLibrary, "jack_port_short_name");
	jack_port_flags_fun = (jack_port_flags_fun_def)dlsym(gLibrary, "jack_port_flags");
	jack_port_type_fun = (jack_port_type_fun_def)dlsym(gLibrary, "jack_port_type");
	jack_port_connected_fun = (jack_port_connected_fun_def)dlsym(gLibrary, "jack_port_connected");
	jack_port_connected_to_fun = (jack_port_connected_to_fun_def)dlsym(gLibrary, "jack_port_connected_to");
	jack_port_tie_fun = (jack_port_tie_fun_def)dlsym(gLibrary, "jack_port_tie");
	jack_port_untie_fun = (jack_port_untie_fun_def)dlsym(gLibrary, "jack_port_untie");
	jack_port_get_latency_fun = (jack_port_get_latency_fun_def)dlsym(gLibrary, "jack_port_get_latency");
	jack_port_set_latency_fun = (jack_port_set_latency_fun_def)dlsym(gLibrary, "jack_port_set_latency");
	jack_recompute_total_latencies_fun = (jack_recompute_total_latencies_fun_def)dlsym(gLibrary, "jack_recompute_total_latencies");
	jack_port_set_name_fun = (jack_port_set_name_fun_def)dlsym(gLibrary, "jack_port_set_name");
	jack_port_request_monitor_fun = (jack_port_request_monitor_fun_def)dlsym(gLibrary, "jack_port_request_monitor");
	jack_port_request_monitor_by_name_fun = (jack_port_request_monitor_by_name_fun_def)dlsym(gLibrary, "jack_port_request_monitor_by_name");
	jack_port_ensure_monitor_fun = (jack_port_ensure_monitor_fun_def)dlsym(gLibrary, "jack_port_ensure_monitor");
	jack_port_monitoring_input_fun = (jack_port_monitoring_input_fun_def)dlsym(gLibrary, "jack_port_monitoring_input_fun");
	jack_is_realtime_fun = (jack_is_realtime_fun_def)dlsym(gLibrary, "jack_is_realtime");
	jack_on_shutdown_fun = (jack_on_shutdown_fun_def)dlsym(gLibrary, "jack_on_shutdown");
	jack_set_process_callback_fun = (jack_set_process_callback_fun_def)dlsym(gLibrary, "jack_set_process_callback");
	jack_set_freewheel_fun = (jack_set_freewheel_fun_def)dlsym(gLibrary, "jack_set_freewheel");
	jack_set_buffer_size_fun = (jack_set_buffer_size_fun_def)dlsym(gLibrary, "jack_set_buffer_size");
	jack_set_buffer_size_callback_fun = (jack_set_buffer_size_callback_fun_def)dlsym(gLibrary, "jack_set_buffer_size_callback");
	jack_set_sample_rate_callback_fun = (jack_set_sample_rate_callback_fun_def)dlsym(gLibrary, "jack_set_sample_rate_callback");
	jack_set_port_registration_callback_fun = (jack_set_port_registration_callback_fun_def)dlsym(gLibrary, "jack_set_port_registration_callback");
	jack_set_graph_order_callback_fun = (jack_set_graph_order_callback_fun_def)dlsym(gLibrary, "jack_set_graph_order_callback");
	jack_set_xrun_callback_fun = (jack_set_xrun_callback_fun_def)dlsym(gLibrary, "jack_set_xrun_callback");
	jack_set_thread_init_callback_fun = (jack_set_thread_init_callback_fun_def)dlsym(gLibrary, "jack_set_thread_init_callback");
	jack_activate_fun = (jack_activate_fun_def)dlsym(gLibrary, "jack_activate");
	jack_deactivate_fun = (jack_deactivate_fun_def)dlsym(gLibrary, "jack_deactivate");
	jack_port_register_fun = (jack_port_register_fun_def)dlsym(gLibrary, "jack_port_register");
	jack_port_unregister_fun = (jack_port_unregister_fun_def)dlsym(gLibrary, "jack_port_unregister");
	jack_port_is_mine_fun = (jack_port_is_mine_fun_def)dlsym(gLibrary, "jack_port_is_mine");
	jack_port_get_connections_fun = (jack_port_get_connections_fun_def)dlsym(gLibrary, "jack_port_get_connections");
	jack_port_get_all_connections_fun = (jack_port_get_all_connections_fun_def)dlsym(gLibrary, "jack_port_get_all_connections_fun");
	jack_port_lock_fun = (jack_port_lock_fun_def)dlsym(gLibrary, "jack_port_lock");
	jack_port_unlock_fun = (jack_port_unlock_fun_def)dlsym(gLibrary, "jack_port_unlock");
	jack_port_get_total_latency_fun = (jack_port_get_total_latency_fun_def)dlsym(gLibrary, "jack_port_get_total_latency");
	jack_connect_fun = (jack_connect_fun_def)dlsym(gLibrary, "jack_connect");
	jack_disconnect_fun = (jack_disconnect_fun_def)dlsym(gLibrary, "jack_disconnect");
	jack_port_connect_fun = (jack_port_connect_fun_def)dlsym(gLibrary, "jack_port_connect");
	jack_port_disconnect_fun = (jack_port_disconnect_fun_def)dlsym(gLibrary, "jack_port_disconnect");
	jack_get_sample_rate_fun = (jack_get_sample_rate_fun_def)dlsym(gLibrary, "jack_get_sample_rate");
	jack_get_buffer_size_fun = (jack_get_buffer_size_fun_def)dlsym(gLibrary, "jack_get_buffer_size");
	jack_get_ports_fun = (jack_get_ports_fun_def)dlsym(gLibrary, "jack_get_ports");
	jack_port_by_name_fun = (jack_port_by_name_fun_def)dlsym(gLibrary, "jack_port_by_name");
	jack_port_by_id_fun = (jack_port_by_id_fun_def)dlsym(gLibrary, "jack_port_by_id");
	jack_engine_takeover_timebase_fun = (jack_engine_takeover_timebase_fun_def)dlsym(gLibrary, "jack_engine_takeover_timebase");
	jack_frames_since_cycle_start_fun = (jack_frames_since_cycle_start_fun_def)dlsym(gLibrary, "jack_frames_since_cycle_start");
	jack_frame_time_fun = (jack_frame_time_fun_def)dlsym(gLibrary, "jack_frame_time_fun");
	jack_last_frame_time_fun = (jack_last_frame_time_fun_def)dlsym(gLibrary, "jack_last_frame_time");
	jack_cpu_load_fun = (jack_cpu_load_fun_def)dlsym(gLibrary, "jack_cpu_load");
	jack_client_thread_id_fun = (jack_client_thread_id_fun_def)dlsym(gLibrary, "jack_client_thread_id");
	jack_get_client_name_fun = (jack_get_client_name_fun_def)dlsym(gLibrary, "jack_get_client_name");
	jack_port_name_size_fun = (jack_port_name_size_fun_def)dlsym(gLibrary, "jack_port_name_size");
	jack_client_name_size_fun = (jack_client_name_size_fun_def)dlsym(gLibrary, "jack_client_name_size");
	jack_release_timebase_fun = (jack_release_timebase_fun_def)dlsym(gLibrary, "jack_release_timebase");
	jack_set_sync_callback_fun = (jack_set_sync_callback_fun_def)dlsym(gLibrary, "jack_set_sync_callback");
	jack_set_sync_timeout_fun = (jack_set_sync_timeout_fun_def)dlsym(gLibrary, "jack_set_sync_timeout");
	jack_set_timebase_callback_fun = (jack_set_timebase_callback_fun_def)dlsym(gLibrary, "jack_set_timebase_callback");
	jack_transport_locate_fun = (jack_transport_locate_fun_def)dlsym(gLibrary, "jack_transport_locate_fun");
	jack_transport_query_fun = (jack_transport_query_fun_def)dlsym(gLibrary, "jack_transport_query");
	jack_get_current_transport_frame_fun = (jack_get_current_transport_frame_fun_def)dlsym(gLibrary, "jack_get_current_transport_frame");
	jack_transport_reposition_fun = (jack_transport_reposition_fun_def)dlsym(gLibrary, "jack_transport_reposition");
	jack_transport_start_fun = (jack_transport_start_fun_def)dlsym(gLibrary, "jack_transport_start");
	jack_transport_stop_fun = (jack_transport_stop_fun_def)dlsym(gLibrary, "jack_transport_stop");
	jack_get_transport_info_fun = (jack_get_transport_info_fun_def)dlsym(gLibrary, "jack_get_transport_info");
	jack_set_transport_info_fun = (jack_set_transport_info_fun_def)dlsym(gLibrary, "jack_set_transport_info");
	jack_get_max_delayed_usecs_fun = (jack_get_max_delayed_usecs_fun_def)dlsym(gLibrary, "jack_get_max_delayed_usecs");
	jack_get_xrun_delayed_usecs_fun = (jack_get_xrun_delayed_usecs_fun_def)dlsym(gLibrary, "jack_get_xrun_delayed_usecs");
	jack_reset_max_delayed_usecs_fun = (jack_reset_max_delayed_usecs_fun_def)dlsym(gLibrary, "jack_reset_max_delayed_usecs");
	jack_acquire_real_time_scheduling_fun = (jack_acquire_real_time_scheduling_fun_def)dlsym(gLibrary, "jack_acquire_real_time_scheduling");
	jack_client_create_thread_fun = (jack_client_create_thread_fun_def)dlsym(gLibrary, "jack_client_create_thread");
	jack_drop_real_time_scheduling_fun = (jack_drop_real_time_scheduling_fun_def)dlsym(gLibrary, "jack_drop_real_time_scheduling");
	jack_get_internal_client_name_fun = (jack_get_internal_client_name_fun_def)dlsym(gLibrary, "jack_get_internal_client_name");
	jack_internal_client_handle_fun = (jack_internal_client_handle_fun_def)dlsym(gLibrary, "jack_internal_client_handle");
	jack_internal_client_load_fun = (jack_internal_client_load_fun_def)dlsym(gLibrary, "jack_internal_client_load");
	jack_internal_client_unload_fun = (jack_internal_client_unload_fun_def)dlsym(gLibrary, "jack_internal_client_unload");
	jack_client_open_fun = (jack_client_open_fun_def)dlsym(gLibrary, "jack_client_open");
	jack_client_new_fun = (jack_client_new_fun_def)dlsym(gLibrary, "jack_client_new");
	jack_client_close_fun = (jack_client_close_fun_def)dlsym(gLibrary, "jack_client_close");
	
	return true;

error:
	if (jackLibrary) dlclose(jackLibrary);
	if (jackmpLibrary) dlclose(jackmpLibrary);
	return false;
}