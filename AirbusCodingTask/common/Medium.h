#ifndef AIRBUS_CODING_TASK_COMMON_MEDIUM_H
#define AIRBUS_CODING_TASK_COMMON_MEDIUM_H

#include "DataStructures.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_LEN 18U
#define INPUT_BUFF_LEN 50U

void ClearMsgBuffer(uint8_t* const buffer)
{
    for (size_t i = 0; i<MAX_MSG_LEN+1U; i++)
    {
        buffer[i] = 0;
    }
}

void ClearInputBuffer(char* const buffer)
{
    for (size_t i = 0; i<INPUT_BUFF_LEN; i++)
    {
        buffer[i] = '\0';
    }
}

size_t GetBufferSize(uint8_t* buffer)
{
    switch (buffer[0U])
    {
        case TC_SystemCheck:
            return 2U;
        case TC_AdjustOrbitalPath:
            return 14U;
        case TC_PayloadOperation:
            return 4U;
        case TM_SystemHealthStatus:
            return 14U;
        case TM_OrbitalDataReport:
            return 18U;
        case TM_PayloadData:
            return 18U;
        default:
            return 0U;
    }
}

size_t CalculateBufferSize(const Telecommand* const command)
{
    switch (command->command_id)
    {
        case TC_SystemCheck:
            return sizeof(command->command_id)+1U;
        case TC_AdjustOrbitalPath:
            return sizeof(command->command_id)+sizeof(AdjustOrbitalPathTC)+1U;
        case TC_PayloadOperation:
            return sizeof(command->command_id)+sizeof(PayloadOperationTC)+1U;
        default:
            return 0U;
    }
}

FILE* InitMedium(const char* medium_path, const char* init_type)
{
    char cwd[500U];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Path error!");
    }

    int idx = 0;
    for (; idx < 500U; idx++)
    {
        if (cwd[idx] == '\0')
        {
            idx--;
            break;
        }
    }
    for (; cwd[idx] != '/'; idx--)
    {
        cwd[idx] = '\0';
    }
    strncat(cwd, medium_path, strlen(medium_path));

    return fopen(cwd, init_type);
}

#endif // AIRBUS_CODING_TASK_COMMON_MEDIUM_H
