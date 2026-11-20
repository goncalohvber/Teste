#include "Sensor.h"

Sensor::Sensor()
: Comum()
{
    VALOR = 0;
    VALOR_AVISO = 0;
}

Sensor::Sensor(int _id, string _marca, float _probAvaria, int _x, int _y, float _valor, float _valorAviso)
: Comum(_id, _marca, _probAvaria, _x, _y)
{
    VALOR = _valor;
    VALOR_AVISO = _valorAviso;
}

Sensor::~Sensor()
{
    //dtor
}


bool Sensor::ESTOU_EM_AVISO()
{
    return VALOR >= VALOR_AVISO;
}

void Sensor::SHOW(ostream &f)
{
    Comum::SHOW(f);
    f << " - Valor: " << VALOR << " (aviso: " << VALOR_AVISO << ")";
    if(ESTOU_EM_AVISO())
        f << " - EM AVISO";
}
