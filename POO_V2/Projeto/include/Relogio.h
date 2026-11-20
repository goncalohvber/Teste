#ifndef RELOGIO_H
#define RELOGIO_H

#include <iostream>
#include <iomanip>

using namespace std;

// relogio da fabrica
// guarda o dia, hora e minutos da simulacao
class Relogio
{
    int DIA;        //comeca em 1
    int HORA;       //0 a 23
    int MINUTO;     //0 a 59
    public:
        Relogio(int _hora = 0, int _minuto = 0);    // tambem serve de construtor por defeito
        virtual ~Relogio();

        int getDIA()        { return DIA; }
        int getHORA()       { return HORA; }
        int getMINUTO()     { return MINUTO; }

        // avanca o relogio (minutos > 0)
        // devolve quantos dias novos comecaram
        int avancar(int minutos);

        // verdadeiro se a fabrica esta aberta (abertura <= hora < fecho)
        bool dentroHorario (int abertura, int fecho);

        // escreve o relogio no formato "Dia 1 08:00"
        void imprimir(ostream &f = std::cout);
};

#endif // RELOGIO_H
