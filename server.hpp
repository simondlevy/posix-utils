/*
   Threaded socket server class

   Copyright (C) 2025 Simon D. Levy

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, in version 3.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http:--www.gnu.org/licenses/>.
 */

#pragma once

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "socket.hpp"

class Server {

    public:

        void init(const uint16_t port, const char * name="")
        {
            strcpy(this->name, name);

            socket.open(port);

            pthread_create(&thread, NULL, thread_fun, this);
        }

        void sendData(const uint8_t * data, const size_t size)
        {
            socket.sendData(data, size);
        }

    private:

        char name[256];

        pthread_t thread;

        ServerSocket socket;

        static void * thread_fun(void * arg)
        {
            auto server = (Server *)arg;

            while (true) {

                // Serve up a socket for the visualizer
                printf("%s server listening for client... ", server->name);
                fflush(stdout);

                server->socket.acceptClient();

                printf("%s client connected\n", server->name);

                while (server->socket.isConnected()) {
                    usleep(1000); // yield
                }
            }

            return NULL;
        }
};
