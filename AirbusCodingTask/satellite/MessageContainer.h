#ifndef AIRBUS_CODING_TASK_SATELLITE_MESSAGE_CONTAINER_H
#define AIRBUS_CODING_TASK_SATELLITE_MESSAGE_CONTAINER_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Time.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_CONTAINER_MSG_SIZE 100

typedef struct
{
    Telemetry* resource;
    Timestamp timestamp;
    uint8_t unique_id;
    uint8_t ref_count;
} Message;

typedef struct
{
    Message messages[MAX_CONTAINER_MSG_SIZE];
    size_t size;
} MessageContainer;

void InitMsgContainer(MessageContainer* const container)
{
    container->size = 0;

    for (size_t i = 0; i<MAX_CONTAINER_MSG_SIZE; i++)
    {
        container->messages[i].resource = NULL;
        container->messages[i].unique_id = 0;
        container->messages[i].ref_count = 0;
    }
}

bool IsFull(const MessageContainer* const container)
{
    return container->size == MAX_CONTAINER_MSG_SIZE;
}

bool IsEmpty(const MessageContainer* const container)
{
    return container->size == 0;
}

void ClearContainer(MessageContainer* const container)
{
    for (size_t i = 0; i<MAX_CONTAINER_MSG_SIZE; i++)
    {
        if (container->messages[i].ref_count > 0)
        {
            free(container->messages[i].resource);
            container->messages[i].ref_count = 0U;

            for (size_t j = i; j<MAX_CONTAINER_MSG_SIZE; j++)
            {
                if (container->messages[j].unique_id == container->messages[i].unique_id)
                {
                    container->messages[j].ref_count = 0U;
                }
            }
        }
    }
}

void PushNewMessage(MessageContainer* const container, Message* const message, const int seconds)
{
    if (IsFull(container))
    {
        printf("Message container is full!");
        return;
    }

    for (size_t i = 0; i<message->ref_count; i++)
    {
        size_t idx = 0;
        for (; idx<container->size; idx++)
        {
            if (IsEarlier(&message->timestamp, &container->messages[idx].timestamp))
            {
                break;
            }
        }

        if (container->size != 0U)
        {
            for (int j = container->size-1; j>=idx; j--)
            {
                container->messages[j+1U] = container->messages[j];
            }
        }

        container->messages[idx] = *message;
        container->size += 1;

        AdvanceTime(&message->timestamp, seconds);
    }
}

#endif // AIRBUS_CODING_TASK_SATELLITE_MESSAGE_CONTAINER_H
