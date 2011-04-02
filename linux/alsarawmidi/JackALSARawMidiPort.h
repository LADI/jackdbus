/*
Copyright (C) 2011 Devin Anderson

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

#ifndef __JackALSARawMidiPort__
#define __JackALSARawMidiPort__

#include <alsa/asoundlib.h>
#include <poll.h>

#include "JackConstants.h"

namespace Jack {

    class JackALSARawMidiPort {

    private:

        char alias[JACK_CLIENT_NAME_SIZE + JACK_PORT_NAME_SIZE];
        char name[JACK_CLIENT_NAME_SIZE + JACK_PORT_NAME_SIZE];
        int num_fds;
        struct pollfd *poll_fds;

    protected:

        snd_rawmidi_t *rawmidi;

        bool
        ProcessPollEvents(unsigned short *revents);

        void
        SetPollEventMask(unsigned short events);

    public:

        JackALSARawMidiPort(snd_rawmidi_info_t *info, size_t index);

        virtual
        ~JackALSARawMidiPort();

        const char *
        GetAlias();

        const char *
        GetName();

        int
        GetPollDescriptorCount();

        bool
        PopulatePollDescriptors(struct pollfd *poll_fd);

    };

}

#endif
