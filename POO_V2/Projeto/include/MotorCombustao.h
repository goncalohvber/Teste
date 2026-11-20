#ifndef MOTORCOMBUSTAO_H
#define MOTORCOMBUSTAO_H

#include "Motor.h"

class MotorCombustao : public Motor
{
    public:
        MotorCombustao();
        virtual ~MotorCombustao();
        MotorCombustao(int _id, string _marca, float _probAvaria, int _x, int _y, float _consumoHora, float _tempAviso, float _tempParagem);

        string GetTIPO() { return "MOTOR-COMBUSTAO"; }

    protected:
        // graus que o motor aquece por hora em RUN (reimplementa o da classe Motor)
        float getAQUECIMENTO() { return 7; }
};

#endif // MOTORCOMBUSTAO_H
