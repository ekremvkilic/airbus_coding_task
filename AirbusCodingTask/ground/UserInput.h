#ifndef AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H
#define AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>

bool terminate = false;
bool new_message_ready = false;

void GetUserInput(Telecommand* const active_message)
{
    char input_buffer[INPUT_BUFF_LEN];
    ClearInputBuffer(input_buffer);

    read(0, &input_buffer, sizeof(input_buffer));
    usleep(10000);
    sscanf(input_buffer, "%hhu", &active_message->command_id);

    if (!((active_message->command_id == TC_SystemCheck)
       || (active_message->command_id == TC_AdjustOrbitalPath)
       || (active_message->command_id == TC_PayloadOperation)
       || (active_message->command_id == 100)))
    {
        ClearInputBuffer(input_buffer);
        __fpurge(stdin);
    }

    switch (active_message->command_id)
    {
        case TC_SystemCheck:
            new_message_ready = true;
            break;
        case TC_AdjustOrbitalPath:
            sscanf(input_buffer, "%hhu%f%f%f", &active_message->command_id,
                                               &active_message->adjust_orbital_path.delta_v.x,
                                               &active_message->adjust_orbital_path.delta_v.y,
                                               &active_message->adjust_orbital_path.delta_v.z);
            __fpurge(stdin);
            new_message_ready = true;
            break;
        case TC_PayloadOperation:
            sscanf(input_buffer, "%hhu%hhu%hhu", &active_message->command_id,
                                                 &active_message->payload_operation.operation_code,
                                                 &active_message->payload_operation.payload_id);
            __fpurge(stdin);  
            new_message_ready = true;
            break;
        case 100:
            terminate = true;
            break;
    }
}

#endif // AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H
