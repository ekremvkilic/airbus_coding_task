#ifndef AIRBUS_CODING_TASK_SATELLITE_PREPARE_TELEMETRY_H
#define AIRBUS_CODING_TASK_SATELLITE_PREPARE_TELEMETRY_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/satellite/State.h"

void PrepareTelemetry(const Telecommand* const command, Telemetry** message, const SatelliteState* const state)
{
    *message = (Telemetry*)malloc(sizeof(Telemetry));

    if (*message == NULL)
    {
        printf("Memory allocation failed!");
        return;
    }

    switch (command->command_id)
    {
        case TC_SystemCheck:
            (*message)->status_code = TM_SystemHealthStatus;
            (*message)->system_health_status.battery_level = state->status.battery;
            (*message)->system_health_status.cpu_usage = state->status.cpu;
            (*message)->system_health_status.memory_usage = state->status.memory;
            break;
        case TC_AdjustOrbitalPath:
            (*message)->status_code = TM_OrbitalDataReport;
            (*message)->orbital_data_report.current_altitude = state->altitude;
            (*message)->orbital_data_report.velocity_vector.x = state->velocity.x;
            (*message)->orbital_data_report.velocity_vector.y = state->velocity.y;
            (*message)->orbital_data_report.velocity_vector.z = state->velocity.z;
            break;
        case TC_PayloadOperation:
            (*message)->status_code = TM_PayloadData;
            (*message)->payload_data.payload_id = command->payload_operation.payload_id;

            switch (command->payload_operation.payload_id)
            {
                case PId_Camera:
                    (*message)->payload_data.operational_status = state->payload.camera.active;
                    (*message)->payload_data.measurements[0U] = state->payload.camera.measurements[0U];
                    (*message)->payload_data.measurements[1U] = state->payload.camera.measurements[1U];
                    (*message)->payload_data.measurements[2U] = state->payload.camera.measurements[2U];
                    break;
                case PId_TempSensor:
                    (*message)->payload_data.operational_status = state->payload.temp_sensor.active;
                    (*message)->payload_data.measurements[0U] = state->payload.temp_sensor.measurements[0U];
                    (*message)->payload_data.measurements[1U] = state->payload.temp_sensor.measurements[1U];
                    (*message)->payload_data.measurements[2U] = state->payload.temp_sensor.measurements[2U];
                    break;
                case PId_SolarPanel:
                    (*message)->payload_data.operational_status = state->payload.panel.active;
                    (*message)->payload_data.measurements[0U] = 0.0F;
                    (*message)->payload_data.measurements[1U] = 0.0F;
                    (*message)->payload_data.measurements[2U] = 0.0F;
                    break;
                default:
                    printf("Wrong Payload Id. This should not have happened!");
            }
            break;
        default:
            printf("Wrong Telecommand Id. This should not have happened!");
    }
}

#endif // AIRBUS_CODING_TASK_SATELLITE_PREPARE_TELEMETRY_H
