#ifndef AIRBUS_CODING_TASK_COMMON_SOCKET_H
#define AIRBUS_CODING_TASK_COMMON_SOCKET_H

#include "Medium.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

bool CreateSendSocket(int* file_descriptor, int* the_socket, const uint16_t port_no)
{
    // Creating socket file descriptor
    *file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (*file_descriptor < 0)
    {
        printf("Ground socket creation error!\n");
        return false;
    }

    // Setting socket options for the port 8080
    int opt = 1;
    if (setsockopt(*file_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("Ground socket option setting error!\n");
        return false;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_no);

    // Forcefully attaching socket to the port 8080
    int result = bind(*file_descriptor, (struct sockaddr*)&address, sizeof(address));
    if (result < 0)
    {
        printf("Ground socket binding failed!\n");
        return false;
    }
    result = listen(*file_descriptor, 3);
    if (result < 0)
    {
        printf("Ground socket listen error!\n");
        return false;
    }

    socklen_t addrlen = sizeof(address);
    *the_socket = accept(*file_descriptor, (struct sockaddr*)&address, &addrlen);
    if (*the_socket < 0)
    {
        printf("Ground socket accept error!\n");
        return false;
    }

    return true;
}

bool CreateReceiveSocket(int* file_descriptor, const uint16_t port_no, const char address[10U])
{
    // Creating socket file descriptor
    *file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (*file_descriptor < 0)
    {
        printf("Satellite socket creation error!");
        return false;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);

    // Convert IPv4 and IPv6 addresses from text to binary form
    int result = inet_pton(AF_INET, address, &serv_addr.sin_addr);
    if (result <= 0)
    {
        printf("Invalid socket address or address not supported error\n");
        return false;
    }

    result = connect(*file_descriptor, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result < 0)
    {
        printf("Socket connection failed!\n");
        return false;
    }

    return true;
}

#endif // AIRBUS_CODING_TASK_COMMON_SOCKET_H
