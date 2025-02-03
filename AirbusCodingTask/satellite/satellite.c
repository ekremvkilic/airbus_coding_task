#include "ExecuteCommand.h"
#include "PrepareTelemetry.h"
#include "MessageContainer.h"
#include "Scheduler.h"
#include "State.h"
#include "UserInput.h"
#include "../common/ByteSpan.h"
#include "../common/Decoders.h"
#include "../common/Medium.h"
#include "../common/Socket.h"
#include <fcntl.h>
#include <stdio_ext.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    MessageContainer pending_messages;
    SatelliteState state;
    
    InitMsgContainer(&pending_messages);
    InitState(&state);

    uint8_t msg_buffer[MAX_MSG_LEN+1U];
    ClearMsgBuffer(msg_buffer);

    int send_socket = 0;
    int send_descriptor = 0;
    int receive_descriptor = 0;
    bool receive_socket_created = CreateReceiveSocket(&receive_descriptor, 8000U, "127.0.0.8");
    bool send_socket_created = CreateSendSocket(&send_descriptor, &send_socket, 8080U);

    char input_buffer[INPUT_BUFF_LEN];
    ClearInputBuffer(input_buffer);

    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    fcntl(receive_descriptor, F_SETFL, O_NONBLOCK);

    while (send_socket_created && receive_socket_created)
    {
        if (!GetUserInput(input_buffer))
        {
            break;
        }

        read(receive_descriptor, msg_buffer, MAX_MSG_LEN);
        ByteSpan received_packet = {msg_buffer, GetBufferSize(msg_buffer)};

        Telecommand received_tc;
        if (received_packet.size != 0)
        {
            printf("Telecommand is received!\n");
            if (TelecommandDecoder(received_packet, &received_tc))
            {
                printf("Telecommand is executed!\n");
                ExecuteCommand(&received_tc, &state);
            }
            else
            {
                printf("Corrupted Telecommand Received!\n");
            }
        }

        UpdateSatelliteState(&state);

        Telemetry* pending_tm = NULL;
        if (received_packet.size != 0)
        {
            printf("Telemetry is prepared!\n");
            PrepareTelemetry(&received_tc, &pending_tm, &state);

            ScheduleNewMessage(&pending_messages, pending_tm);
        }

        TransmitNextMessage(&pending_messages, &send_socket);

        ClearMsgBuffer(msg_buffer);
    }

    ClearContainer(&pending_messages);

    close(send_socket);
    close(send_descriptor);
    close(receive_descriptor);

    return 0;
}
