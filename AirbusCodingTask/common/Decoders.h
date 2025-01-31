#ifndef AIRBUS_CODING_TASK_COMMON_DECODERS_H
#define AIRBUS_CODING_TASK_COMMON_DECODERS_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/ByteSpan.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/Checksum.h"
#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/DataStructures.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static bool DecodeData(const ByteSpan packet_source, void* const data_destination, const size_t data_size)
{
    if (packet_source.size != (sizeof(uint8_t) + data_size + sizeof(uint8_t)))
    {
        return false;
    }

    if (CalculateCrc8(packet_source.data, packet_source.size-1U) != SpanRead(packet_source, packet_source.size-1U))
    {
        return false;
    }

    memcpy(data_destination, packet_source.data+1U, data_size);

    return true;
}

bool TelecommandDecoder(const ByteSpan tc_packet, Telecommand* const tc_data)
{
    bool success = false;
    if (tc_packet.size == 0U || tc_data == NULL)
    {
        return success;
    }

    tc_data->command_id = SpanRead(tc_packet, 0U);

    switch (tc_data->command_id)
    {
        case TC_SystemCheck:
            success = true;
            break;
        case TC_AdjustOrbitalPath:
            success = DecodeData(tc_packet, (void*)&(tc_data->adjust_orbital_path), sizeof(AdjustOrbitalPathTC));
            break;
        case TC_PayloadOperation:
            success = DecodeData(tc_packet, (void*)&(tc_data->payload_operation), sizeof(PayloadOperationTC));
            break;
        default:
            printf("Invalid Telecommand Id while decoding!");
    }

    return success;
}

bool TelemetryDecoder(const ByteSpan tm_packet, Telemetry* const tm_data)
{
    bool success = false;
    if (tm_packet.size == 0U || tm_data == NULL)
    {
        return success;
    }

    tm_data->status_code = SpanRead(tm_packet, 0U);

    switch (tm_data->status_code)
    {
        case TM_SystemHealthStatus:
            success = DecodeData(tm_packet, (void*)&(tm_data->system_health_status), sizeof(SystemHealthStatusTM));
            break;
        case TM_OrbitalDataReport:
            success = DecodeData(tm_packet, (void*)&(tm_data->orbital_data_report), sizeof(OrbitalDataReportTM));
            break;
        case TM_PayloadData:
            success = DecodeData(tm_packet, (void*)&(tm_data->payload_data), sizeof(PayloadDataTM));
            break;
        default:
            printf("Invalid Telemetry status code while decoding!");
    }

    return success;
}

#endif // AIRBUS_CODING_TASK_COMMON_DECODERS_H
