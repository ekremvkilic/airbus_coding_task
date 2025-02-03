#ifndef AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H
#define AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H

#include "../common/Medium.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <unistd.h>

bool GetUserInput(char input_buffer[INPUT_BUFF_LEN])
{
    read(0, &input_buffer, sizeof(INPUT_BUFF_LEN));

    usleep(10000);

    char key;
    sscanf(input_buffer, "%c", &key);
    if ((key == 'q') || (key == 'Q'))
    {
        return false;
    }
    
    ClearInputBuffer(input_buffer);
    __fpurge(stdin);

    return true;
}

#endif // AIRBUS_CODING_TASK_SATELLITE_USER_INPUT_H
