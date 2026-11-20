#ifndef SENSORMISSEL_H
#define SENSORMISSEL_H

#include "Sensor.h"

class SensorMissel : public Sensor
{
    public:
        SensorMissel();
        SensorMissel(int _id, string _marca, float _probAvaria, int _x, int _y, float _valor, float _valorAviso = 0);
        virtual ~SensorMissel();

        string GetTIPO() { return "SENSOR-MISSEL"; }
};

#endif // SENSORMISSEL_H
