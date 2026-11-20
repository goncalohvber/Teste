#ifndef SENSOR_H
#define SENSOR_H

#include "Comum.h"

// herda de Comum e acrescenta apenas o que e so dele (VALOR e VALOR_AVISO)

class Sensor : public Comum
{
    float VALOR;            // leitura atual do sensor
    float VALOR_AVISO;      // valor a partir do qual o sensor envia aviso (vem do XML)

    public:
        Sensor();
        Sensor(int _id, string _marca, float _probAvaria, int _x, int _y, float _valor, float _valorAviso = 0);
        virtual ~Sensor();

        float getVALOR() { return VALOR; }
        float getVALOR_AVISO() { return VALOR_AVISO; }
        string GetTIPO() { return "SENSOR"; }

        // verdadeiro se a leitura atual obriga a enviar aviso ao gestor
        // por defeito avisa quando VALOR >= VALOR_AVISO (cada sensor pode reimplementar)
        virtual bool ESTOU_EM_AVISO();

        void SHOW(ostream &f = std::cout);
};

#endif // SENSOR_H
