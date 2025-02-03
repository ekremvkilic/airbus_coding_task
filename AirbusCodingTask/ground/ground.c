#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Decoders.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Encoders.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Medium.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Socket.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/ground/Logger.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/ground/UserInput.h"

int main()
{
    Telecommand active_message;
    uint8_t buffer[MAX_MSG_LEN+1U];
    ClearMsgBuffer(buffer);

    int send_socket = 0;
    int send_descriptor = 0;
    int receive_descriptor = 0;
    bool send_socket_created = CreateSendSocket(&send_descriptor, &send_socket, 8080U);
    bool receive_socket_created = CreateReceiveSocket(&receive_descriptor, 8000U, "127.0.0.8");

    // fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    active_message.command_id = 103;
    active_message.payload_operation.operation_code = 1;
    active_message.payload_operation.operation_code = 1;
    new_message_ready = true;

    printf("send_fd: %d\nrecv_fd: %d\nsocket: %d\n", send_descriptor, receive_descriptor, send_socket);

    while (send_socket_created && receive_socket_created)
    {
        // GetUserInput(&active_message);
        if (terminate)
        {
            break;
        }

        if (new_message_ready)
        {
            printf("New message is ready!\n");
            ByteSpan tc_packet = {buffer, CalculateBufferSize(&active_message)};
            if (!TelecommandEncoder(&active_message, tc_packet))
            {
                Log(LOG_WARN, "The telecommand could not be encoded!");
            }
            else
            {
                ssize_t count = send(send_descriptor, tc_packet.data, tc_packet.size, 0);
                printf("count: %ld\n", count);
                if (count == 0U)
                {
                    Log(LOG_WARN, "Could not write to Telecommand medium!");
                }

                LogTC(LOG_INFO, &active_message);
            }

            new_message_ready = false;
            ClearMsgBuffer(buffer);
            active_message.command_id = 0U;
        }

        ssize_t read_bytes = read(receive_descriptor, buffer, MAX_MSG_LEN);
        ByteSpan received_packet = {buffer, GetBufferSize(buffer)};
        Telemetry received_tm;
        if (received_packet.size != 0)
        {
            if (TelemetryDecoder(received_packet, &received_tm))
            {
                LogTM(LOG_INFO, &received_tm);
            }
            else
            {
                Log(LOG_WARN, "Corrupted Telemetry Received!");
            }

            ClearMsgBuffer(buffer);
        }
    }

    close(send_socket);
    close(send_descriptor);
    close(receive_descriptor);

    return 0;
}
