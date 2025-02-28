/*
   Socket class for servers, including Bluetooth

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class ServerSocket {

    public:

        void open(const uint16_t port)
        {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);

            struct sockaddr_in serv_addr = {};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            serv_addr.sin_port = htons(port);

            const int reuse = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                        sizeof(int)) < 0) {
                fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
                return;
            }

            signal(SIGPIPE, sigpipe_handler);

            bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

            listen(sockfd, 1);

            connected = false;
        }

        void acceptClient()
        {
            clientfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

            connected = true;
        }

        void sendData(const uint8_t * data, const size_t size)
        {
            connected = connected && 
                (write(clientfd, data, size) == (ssize_t)size);
        }

        bool isConnected()
        {
            return connected;
        }

    private:

        bool connected;

        int sockfd;

        int clientfd;

        static void sigpipe_handler(int arg)
        {
            (void)arg;
        }
};
