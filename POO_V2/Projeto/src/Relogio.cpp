#include "Relogio.h"

Relogio::~Relogio()
{
    //dtor
}


Relogio::Relogio(int _hora, int _minuto)
{
    DIA = 1;
    HORA = _hora;
    MINUTO = _minuto;
}

// avanca o relogio (minutos > 0)
// devolve quantos dias novos comecaram
int Relogio::avancar(int minutos)
{
    MINUTO += minutos;
    HORA += MINUTO / 60;        // se min > 60 entao vai para horas
    MINUTO = MINUTO % 60;       // resto sao os minutos atuais

    int diasNovos = HORA / 24;
    HORA = HORA % 24;
    DIA += diasNovos;

    return diasNovos;
}

// verdadeiro se a fabrica esta aberta (abertura <= hora < fecho)
bool Relogio::dentroHorario (int abertura, int fecho)
{
    return HORA >= abertura && HORA < fecho;
}

// escreve o relogio no formato "Dia 1 08:00"
void Relogio::imprimir(ostream &f)
{
    f << "Dia " << DIA << " " << setfill('0') << setw(2) << HORA << ":" << setw(2) << MINUTO << setfill(' ');
}
