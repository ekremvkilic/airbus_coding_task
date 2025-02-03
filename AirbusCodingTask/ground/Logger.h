#ifndef AIRBUS_CODING_TASK_GROUND_LOGGER_H
#define AIRBUS_CODING_TASK_GROUND_LOGGER_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_LEN_N_RST
} LogLevel;

static const char* kLogTypes[LOG_LEN_N_RST] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
}; 

static const char* kLogColors[LOG_LEN_N_RST+1U] = {
    "\033[1;32m",
    "\033[1;37m",
    "\033[1;33m",
    "\033[1;31m",
    "\033[0m"
};

void Log(const LogLevel level, const char* message)
{
    printf("[%s%s%s]: %s", kLogColors[level], kLogTypes[level], kLogColors[LOG_LEN_N_RST], message);
}

void LogTC(const LogLevel level, const Telecommand* const telecommand)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d ", tm->tm_year+1900,
                                           tm->tm_mon+1,
                                           tm->tm_mday,
                                           tm->tm_hour,
                                           tm->tm_min,
                                           tm->tm_sec);
    printf("TC_%d ", telecommand->command_id);

    switch(telecommand->command_id)
    {
        case TC_SystemCheck:
            printf("System Check Command sent to the satellite.\n");
            break;
        case TC_AdjustOrbitalPath:
            printf("Adjust Orbital Path sent to the satellite. ΔVx: %.3f, ΔVy: %.3f, ΔVz: %.3f\n",
                telecommand->adjust_orbital_path.delta_v.x,
                telecommand->adjust_orbital_path.delta_v.y,
                telecommand->adjust_orbital_path.delta_v.z);
            break;
        case TC_PayloadOperation:
            printf("Payload Operation sent to the satellite. Opcode: %d, PayloadId: %d\n",
                telecommand->payload_operation.operation_code,
                telecommand->payload_operation.payload_id);
            break;
        default:
            printf("Invalid Telecommand Id while logging!");
    }
}

void LogTM(const LogLevel level, const Telemetry* const telemetry)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d ", tm->tm_year+1900,
                                           tm->tm_mon+1,
                                           tm->tm_mday,
                                           tm->tm_hour,
                                           tm->tm_min,
                                           tm->tm_sec);
    printf("TM_%d ", telemetry->status_code);

    switch(telemetry->status_code)
    {
        case TM_SystemHealthStatus:
            printf("System Health Status received from the satellite."
                   "CPU: %.3f%%, Memory: %.3f%%, Battery: %.3f%%\n",
                telemetry->system_health_status.cpu_usage,
                telemetry->system_health_status.memory_usage,
                telemetry->system_health_status.battery_level);
            break;
        case TM_OrbitalDataReport:
            printf("Orbital Data Report received from the satellite."
                   "Alt: %.3f, Vx: %.3f, Vy: %.3f, Vz: %.3f\n",
                telemetry->orbital_data_report.current_altitude,
                telemetry->orbital_data_report.velocity_vector.x,
                telemetry->orbital_data_report.velocity_vector.y,
                telemetry->orbital_data_report.velocity_vector.z);
            break;
        case TM_PayloadData:
            printf("Payload Data received from the satellite."
                   "PayloadId: %d, OpStatus: %d, Measurements 0:%.3f, 1:%.3f, 2:%.3f\n",
                telemetry->payload_data.payload_id,
                telemetry->payload_data.operational_status,
                telemetry->payload_data.measurements[0U],
                telemetry->payload_data.measurements[1U],
                telemetry->payload_data.measurements[2U]);
            break;
        default:
            printf("Invalid Telemetry status code while logging!");
    }
}

#endif // AIRBUS_CODING_TASK_GROUND_LOGGER_H
