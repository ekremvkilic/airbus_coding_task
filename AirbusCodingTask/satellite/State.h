#ifndef AIRBUS_CODING_TASK_SATELLITE_STATE_H
#define AIRBUS_CODING_TASK_SATELLITE_STATE_H

#include "/home/ekkilic/Documents/TestWorkspace/CWorkspace/AirbusCodingTask/common/VelocityVector.h"
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum
{
    PId_Camera,
    PId_TempSensor,
    PId_SolarPanel
} PayloadId;

typedef enum
{
    PH_Normal,
    PH_Uncalibrated,
    PH_Malfunction
} PayloadHealth;

typedef struct
{
    PayloadHealth health;
    bool active;
    float measurements[3U];
} PayloadCamera;

typedef struct
{
    PayloadHealth health;
    bool active;
    float measurements[3U];
} PayloadTempSensor;

typedef struct
{
    PayloadHealth health;
    bool active;
} PayloadSolarPanel;

typedef struct
{
    struct
    {
        float cpu;
        float memory;
        float battery;
    } status;

    struct
    {
        PayloadCamera camera;
        PayloadTempSensor temp_sensor;
        PayloadSolarPanel panel;
    } payload;

    VelocityVector3D velocity;
    float altitude;
} SatelliteState;

static float GenerateRandomNumber(float min, float max)
{
    srand(time(NULL));
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

void InitState(SatelliteState* state)
{
    state->status.cpu = 10.0F;
    state->status.memory = 15.0F;
    state->status.battery = 100.0F;

    state->payload.camera.health = PH_Normal;
    state->payload.temp_sensor.health = PH_Normal;
    state->payload.panel.health = PH_Normal;
    state->payload.camera.active = false;
    state->payload.temp_sensor.active = false;
    state->payload.panel.active = false;

    state->payload.camera.measurements[0U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.camera.measurements[1U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.camera.measurements[2U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.temp_sensor.measurements[0U] = GenerateRandomNumber(1.0F, 500.0F);
    state->payload.temp_sensor.measurements[1U] = GenerateRandomNumber(1.0F, 500.0F);
    state->payload.temp_sensor.measurements[2U] = GenerateRandomNumber(1.0F, 500.0F);

    state->velocity.x = 100.0F;
    state->velocity.y = 10.0F;
    state->velocity.z = 0.0F;
    state->altitude = 35786.0F;
}

void UpdateSatelliteState(SatelliteState* state)
{
    float cpu_consumption = 10.0F;
    float memory_consumption = 15.0F;

    if (state->payload.camera.active)
    {
        cpu_consumption += 35.0F;
        memory_consumption += 40.0F;
    }
    if (state->payload.temp_sensor.active)
    {
        cpu_consumption += 5.0F;
        memory_consumption += 10.0F;
    }
    state->status.cpu = cpu_consumption;
    state->status.memory = memory_consumption;

    state->status.battery -= (state->status.cpu / 100.0F);
    if (state->payload.panel.active)
    {
        state->status.battery += 0.2F;
    }
    if (state->status.battery > 100.0F)
    {
        state->status.battery = 100.0F;
    }

    state->altitude += state->velocity.z;

    state->payload.camera.measurements[0U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.camera.measurements[1U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.camera.measurements[2U] = GenerateRandomNumber(35500.0F, 36000.0F);
    state->payload.temp_sensor.measurements[0U] = GenerateRandomNumber(1.0F, 500.0F);
    state->payload.temp_sensor.measurements[1U] = GenerateRandomNumber(1.0F, 500.0F);
    state->payload.temp_sensor.measurements[2U] = GenerateRandomNumber(1.0F, 500.0F);
}

#endif // AIRBUS_CODING_TASK_SATELLITE_STATE_H
