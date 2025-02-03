#ifndef AIRBUS_CODING_TASK_SATELLITE_EXECUTE_COMMAND_H
#define AIRBUS_CODING_TASK_SATELLITE_EXECUTE_COMMAND_H

#include "../common/DataStructures.h"
#include "State.h"
#include <stdio.h>

static void PerformSystemCheck(const SatelliteState* state)
{
    if (state->status.battery < 10.0F)
    {
        printf("Low battery! Shutting down payloads is recommended.\n");
    }
    if (state->payload.camera.health == PH_Malfunction)
    {
        printf("Payload malfunction! Affected module: Camera. Corrective action required.\n");
    }
    if (state->payload.temp_sensor.health == PH_Malfunction)
    {
        printf("Payload malfunction! Affected module: Temperature Sensor. Corrective action required.\n");
    }
    if (state->payload.panel.health == PH_Malfunction)
    {
        printf("Payload malfunction! Affected module: Solar Panel. Corrective action required.\n");
    }
}

static void AdjustOrbitalPath(const Telecommand* const command, SatelliteState* const state)
{
    state->velocity.x += command->adjust_orbital_path.delta_v.x;
    state->velocity.y += command->adjust_orbital_path.delta_v.y;
    state->velocity.z += command->adjust_orbital_path.delta_v.z;

    state->status.battery -= 5.0F;
}

static void OperatePayload(const Telecommand* const command, SatelliteState* const state)
{
    bool commission = command->payload_operation.operation_code == 1;
    switch (command->payload_operation.payload_id)
    {
        case PId_Camera:
            state->payload.camera.active = commission;
            break;
        case PId_TempSensor:
            state->payload.temp_sensor.active = commission;
            break;
        case PId_SolarPanel:
            state->payload.panel.active = commission;
            break;
        default:
            printf("Wrong Payload ID. Should not have happened!\n");
    }
}

void ExecuteCommand(const Telecommand* const command, SatelliteState* const state)
{
    switch (command->command_id)
    {
        case TC_SystemCheck:
            printf("Running System Checks...\n");
            PerformSystemCheck(state);
            break;
        case TC_AdjustOrbitalPath:
            printf("Adjusting Orbital Path...\n");
            AdjustOrbitalPath(command, state);
            break;
        case TC_PayloadOperation:
            printf("Executing Payload Operation...\n");
            OperatePayload(command, state);
            break;
        default:
            printf("Invalid Telecommand Id while executing command!\n");
    }
}

#endif // AIRBUS_CODING_TASK_SATELLITE_EXECUTE_COMMAND_H
