#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Decoders.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Encoders.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Medium.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/ground/Logger.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/ground/UserInput.h"

int main()
{
    Telecommand active_message;
    FILE* tc_message_medium = InitMedium("mediums/tc_medium", "wb");
    FILE* tm_message_medium = InitMedium("mediums/tm_medium", "rb");

    uint8_t buffer[MAX_MSG_LEN+1U];
    ClearMsgBuffer(buffer);

    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

    while (true)
    {
        GetUserInput(&active_message);
        if (terminate || (tm_message_medium == NULL) || (tc_message_medium == NULL))
        {
            break;
        }

        if (new_message_ready)
        {
            ByteSpan tc_packet = {buffer, CalculateBufferSize(&active_message)};
            if (!TelecommandEncoder(&active_message, tc_packet))
            {
                Log(LOG_WARN, "The telecommand could not be encoded!");
            }
            else
            {
                size_t count = fwrite(tc_packet.data, tc_packet.size, 1U, tc_message_medium);
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

        fread(buffer, sizeof(buffer), 1U, tm_message_medium);
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
