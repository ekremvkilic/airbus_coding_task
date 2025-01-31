#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/ExecuteCommand.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/PrepareTelemetry.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/MessageContainer.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/Scheduler.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/State.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/ByteSpan.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Decoders.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Medium.h"
#include <fcntl.h>
#include <stdio_ext.h>
#include <unistd.h>

int main()
{
    printf("Simulation of Satellite on the Orbit.\nPress q or Q to exit: ");

    MessageContainer pending_messages;
    SatelliteState state;
    
    InitMsgContainer(&pending_messages);
    InitState(&state);
    FILE* tm_message_medium = InitMedium("mediums/tm_medium", "wb");
    FILE* tc_message_medium = InitMedium("mediums/tc_medium", "rb");

    uint8_t msg_buffer[MAX_MSG_LEN+1U];
    ClearMsgBuffer(msg_buffer);

    char input_buffer[INPUT_BUFF_LEN];
    ClearInputBuffer(input_buffer);
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    char key = 0;
    while (true)
    {
        {
            read(0, &input_buffer, sizeof(input_buffer));
            usleep(10000);
            sscanf(input_buffer, "%c", &key);
            if ((key == 'q') || (key == 'Q') || (tm_message_medium == NULL) || (tc_message_medium == NULL))
            {
                break;
            }
            ClearInputBuffer(input_buffer);
            __fpurge(stdin);
        }

        fread(&msg_buffer, sizeof(msg_buffer), 1U, tc_message_medium);
        ByteSpan received_packet = {msg_buffer, GetBufferSize(msg_buffer)};

        Telecommand received_tc;
        if (received_packet.size != 0)
        {
            if (TelecommandDecoder(received_packet, &received_tc))
            {
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
            PrepareTelemetry(&received_tc, &pending_tm, &state);

            ScheduleNewMessage(&pending_messages, pending_tm);
        }

        TransmitNextMessage(&pending_messages, tm_message_medium);

        ClearMsgBuffer(msg_buffer);
    }

    ClearContainer(&pending_messages);

    if (tm_message_medium != NULL)
    {
        fclose(tm_message_medium);
    }
    if (tc_message_medium != NULL)
    {
        fclose(tc_message_medium);
    }

    return 0;
}
