#include "../common/Decoders.h"
#include "../common/Encoders.h"
#include "../common/Medium.h"
#include "../common/Socket.h"
#include "Logger.h"
#include "UserInput.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    Telecommand active_message;
    uint8_t msg_buffer[MAX_MSG_LEN+1U];
    ClearMsgBuffer(msg_buffer);

    int send_socket = 0;
    int send_descriptor = 0;
    int receive_descriptor = 0;
    bool send_socket_created = CreateSendSocket(&send_descriptor, &send_socket, 8000U);
    bool receive_socket_created = CreateReceiveSocket(&receive_descriptor, 8080U, "127.0.0.1");

    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    fcntl(receive_descriptor, F_SETFL, O_NONBLOCK);

    while (send_socket_created && receive_socket_created)
    {
        if (GetUserInput(&active_message))
        {
            break;
        }

        if (new_message_ready)
        {
            Log(LOG_DEBUG, "User requested new message to send.\n");

            ByteSpan tc_packet = {msg_buffer, CalculateBufferSize(&active_message)};
            if (!TelecommandEncoder(&active_message, tc_packet))
            {
                Log(LOG_WARN, "The telecommand could not be encoded!\n");
            }
            else
            {
                ssize_t count = send(send_socket, tc_packet.data, tc_packet.size, 0);
                if (count == 0U)
                {
                    Log(LOG_WARN, "Could not send the message!\n");
                }

                LogTC(LOG_INFO, &active_message);
            }

            new_message_ready = false;
            ClearMsgBuffer(msg_buffer);
            active_message.command_id = 0U;
        }

        read(receive_descriptor, msg_buffer, MAX_MSG_LEN);
        ByteSpan received_packet = {msg_buffer, GetBufferSize(msg_buffer)};
        Telemetry received_tm;
        if (received_packet.size != 0)
        {
            Log(LOG_DEBUG, "New message received from the satellite!");

            if (TelemetryDecoder(received_packet, &received_tm))
            {
                LogTM(LOG_INFO, &received_tm);
            }
            else
            {
                Log(LOG_WARN, "Corrupted Telemetry Received!\n");
            }

            ClearMsgBuffer(msg_buffer);
        }
    }

    close(send_socket);
    close(send_descriptor);
    close(receive_descriptor);

    return 0;
}
