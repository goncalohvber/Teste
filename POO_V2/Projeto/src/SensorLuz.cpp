#include "SensorLuz.h"

SensorLuz::SensorLuz()
{
    //ctor
}

SensorLuz::SensorLuz(int _id, string _marca, float _probAvaria, int _x, int _y, float _valor, float _valorAviso)
: Sensor(_id, _marca, _probAvaria, _x, _y, _valor, _valorAviso)
{

}

SensorLuz::~SensorLuz()
{
    //dtor
}

bool SensorLuz::ESTOU_EM_AVISO()
{
    return getVALOR() < getVALOR_AVISO();
}
