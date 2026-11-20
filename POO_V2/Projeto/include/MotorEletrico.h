#ifndef MOTORELETRICO_H
#define MOTORELETRICO_H

#include "Motor.h"

class MotorEletrico : public Motor
{
    public:
        MotorEletrico();
        virtual ~MotorEletrico();
        MotorEletrico(int _id, string _marca, float _probAvaria, int _x, int _y, float _consumoHora, float _tempAviso, float _tempParagem);

        string GetTIPO() { return "MOTOR-ELETRICO"; }

    protected:
        // graus que o motor aquece por hora em RUN (reimplementa o da classe Motor)
        float getAQUECIMENTO() { return 3; }
};

#endif // MOTORELETRICO_H
