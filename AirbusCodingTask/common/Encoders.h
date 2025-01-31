#ifndef AIRBUS_CODING_TASK_COMMON_ENCODERS_H
#define AIRBUS_CODING_TASK_COMMON_ENCODERS_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/ByteSpan.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Checksum.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include <stdbool.h>
#include <string.h>

bool TelecommandEncoder(const Telecommand* const tc_data, ByteSpan tc_packet)
{
    bool success = false;
    if (tc_data == NULL)
    {
        return success;
    }

    SpanWrite(tc_packet, 0U, tc_data->command_id);
    success = true;

    switch (tc_data->command_id)
    {
        case TC_SystemCheck:
            break;
        case TC_AdjustOrbitalPath:
            memcpy(tc_packet.data+1U, (void*)&(tc_data->adjust_orbital_path), sizeof(AdjustOrbitalPathTC));
            break;
        case TC_PayloadOperation:
            memcpy(tc_packet.data+1U, (void*)&(tc_data->payload_operation), sizeof(PayloadOperationTC));
            break;
        default:
            success = false;
            /// @TODO: Log invalid command
    }

    if (success)
    {
        SpanWrite(tc_packet, tc_packet.size-1U, CalculateCrc8(tc_packet.data, tc_packet.size-1U));
    }

    return success;
}

bool TelemetryEncoder(const Telemetry* const tm_data, ByteSpan tm_packet)
{
    bool success = false;
    if (tm_data == NULL)
    {
        return success;
    }

    SpanWrite(tm_packet, 0U, tm_data->status_code);
    success = true;

    switch (tm_data->status_code)
    {
        case TM_SystemHealthStatus:
            memcpy(tm_packet.data+1U, (void*)&(tm_data->system_health_status), sizeof(SystemHealthStatusTM));
            break;
        case TM_OrbitalDataReport:
            memcpy(tm_packet.data+1U, (void*)&(tm_data->orbital_data_report), sizeof(OrbitalDataReportTM));
            break;
        case TM_PayloadData:
            memcpy(tm_packet.data+1U, (void*)&(tm_data->payload_data), sizeof(PayloadDataTM));
            break;
        default:
            success = false;
            /// @TODO: Log invalid command
    }

    if (success)
    {
        SpanWrite(tm_packet, tm_packet.size-1U, CalculateCrc8(tm_packet.data, tm_packet.size-1U));
    }

    return success;
}

#endif // AIRBUS_CODING_TASK_COMMON_ENCODERS_H
