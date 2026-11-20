#ifndef MOTORINDUCAO_H
#define MOTORINDUCAO_H

#include "Motor.h"

class MotorInducao : public Motor
{
    public:
        MotorInducao();
        virtual ~MotorInducao();
        MotorInducao(int _id, string _marca, float _probAvaria, int _x, int _y, float _consumoHora, float _tempAviso, float _tempParagem);

        string GetTIPO() { return "MOTOR-INDUCAO"; }

    protected:
        // graus que o motor aquece por hora em RUN (reimplementa o da classe Motor)
        float getAQUECIMENTO() { return 5; }
};

#endif // MOTORINDUCAO_H
