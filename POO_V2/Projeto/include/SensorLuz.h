#ifndef SENSORLUZ_H
#define SENSORLUZ_H

#include "Sensor.h"

class SensorLuz : public Sensor
{
    public:
        SensorLuz();
        SensorLuz(int _id, string _marca, float _probAvaria, int _x, int _y, float _valor, float _valorAviso = 0);
        virtual ~SensorLuz();

        string GetTIPO() { return "SENSOR-LUZ"; }

        // ao contrario dos outros, avisa quando a luminosidade desce abaixo do valor de aviso
        bool ESTOU_EM_AVISO();
};

#endif // SENSORLUZ_H
