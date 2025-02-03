#ifndef AIRBUS_CODING_TASK_SATELLITE_SCHEDULER_H
#define AIRBUS_CODING_TASK_SATELLITE_SCHEDULER_H

#include "../common/ByteSpan.h"
#include "../common/DataStructures.h"
#include "../common/Encoders.h"
#include "../common/Time.h"
#include "../common/Medium.h"
#include "MessageContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

inline static uint8_t GetRefCount(const uint8_t status_code)
{
    switch (status_code)
    {
        case TM_SystemHealthStatus:
            return 10U;
        case TM_OrbitalDataReport:
            return 5U;
        case TM_PayloadData:
            return 10U;
        default:
            return 0U;
    }
}

inline static uint8_t GetTimeInterval(const uint8_t status_code)
{
    switch (status_code)
    {
        case TM_SystemHealthStatus:
            return 2U;
        case TM_OrbitalDataReport:
            return 5U;
        case TM_PayloadData:
            return 10U;
        default:
            return UINT8_MAX;
    }
}

inline static uint8_t GetMessageSize(const uint8_t status_code)
{
    switch (status_code)
    {
        case TM_SystemHealthStatus:
            return 14U;
            break;
        case TM_OrbitalDataReport:
            return 18U;
            break;
        case TM_PayloadData:
            return 18U;
            break;
        default:
            return 0U;
    }
}

void ScheduleNewMessage(MessageContainer* const container, Telemetry* const tm_data)
{
    if (tm_data == NULL)
    {
        printf("Message to schedule is NULL!\n");
        return; // Defensive
    }

    static uint8_t unique_message_id = 0;

    Timestamp now;
    {
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);
        now.tm_year = tm->tm_year+1900;
        now.tm_mon  = tm->tm_mon+1;
        now.tm_mday = tm->tm_mday;
        now.tm_hour = tm->tm_hour;
        now.tm_min  = tm->tm_min;
        now.tm_sec  = tm->tm_sec;
    }

    Message new_message = {tm_data, now, unique_message_id, GetRefCount(tm_data->status_code)};
    PushNewMessage(container, &new_message, GetTimeInterval(tm_data->status_code));

    unique_message_id++;
}

void TransmitNextMessage(MessageContainer* const container, int* send_socket)
{
    Timestamp now;
    {
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);
        now.tm_year = tm->tm_year+1900;
        now.tm_mon  = tm->tm_mon+1;
        now.tm_mday = tm->tm_mday;
        now.tm_hour = tm->tm_hour;
        now.tm_min  = tm->tm_min;
        now.tm_sec  = tm->tm_sec;
    }

    if ((container->size > 0U) && IsEarlier(&container->messages[0U].timestamp, &now))
    {
        uint8_t buffer[MAX_MSG_LEN];
        ClearMsgBuffer(buffer);

        ByteSpan tm_packet = {buffer, GetMessageSize(container->messages[0].resource->status_code)};
        if (!TelemetryEncoder(container->messages[0U].resource, tm_packet))
        {
            return;
        }

        ssize_t count = send(*send_socket, tm_packet.data, tm_packet.size, 0);
        if (count == 0U)
        {
            printf("Could not write to Telecommand medium!\n");
        }

        printf("A telemetry message is transmitted...\n");

        const uint8_t transmitted_msg_id = container->messages[0U].unique_id;
        bool freed = false;
        if (container->messages[0U].ref_count == 1)
        {
            free(container->messages[0U].resource);
            freed = true;
        }

        for (size_t i = 0; i<container->size; i++)
        {
            container->messages[i] = container->messages[i+1];

            if (!freed && (container->messages[i].unique_id == transmitted_msg_id))
            {
                container->messages[i].ref_count -= 1;
            }
        }
        container->size -= 1;
    }
}

#endif // AIRBUS_CODING_TASK_SATELLITE_SCHEDULER_H
