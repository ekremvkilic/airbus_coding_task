#ifndef AIRBUS_CODING_TASK_COMMON_DATA_STRUCTURES_H
#define AIRBUS_CODING_TASK_COMMON_DATA_STRUCTURES_H

#include "VelocityVector.h"
#include <stdint.h>

typedef struct
{
} SystemCheckTC;

typedef struct
{
    VelocityVector3D delta_v;
} AdjustOrbitalPathTC;

typedef struct
{
    uint8_t operation_code;
    uint8_t payload_id;
} PayloadOperationTC;

typedef struct
{
    uint8_t command_id;

    union
    {
        SystemCheckTC system_check;
        AdjustOrbitalPathTC adjust_orbital_path;
        PayloadOperationTC payload_operation;
    };
} Telecommand;

typedef struct
{
    float cpu_usage;
    float memory_usage;
    float battery_level;
} SystemHealthStatusTM;

typedef struct
{
    float current_altitude;
    VelocityVector3D velocity_vector;
} OrbitalDataReportTM;

typedef struct
{
    uint8_t payload_id;
    uint8_t operational_status;
    float measurements[3U];
} PayloadDataTM;

typedef struct
{
    uint8_t status_code;

    union
    {
        SystemHealthStatusTM system_health_status;
        OrbitalDataReportTM orbital_data_report;
        PayloadDataTM payload_data;
    };
} Telemetry;

enum TCCommandIds
{
    TC_SystemCheck = 101U,
    TC_AdjustOrbitalPath = 102U,
    TC_PayloadOperation = 103U
};

enum TMStatusCodes
{
    TM_SystemHealthStatus = 201U,
    TM_OrbitalDataReport = 202U,
    TM_PayloadData = 203U
};

#endif // AIRBUS_CODING_TASK_COMMON_DATA_STRUCTURES_H
