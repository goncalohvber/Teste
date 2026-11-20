#include "Visitante.h"

Visitante::Visitante()
: Utilizador(0, "")
{
    poderLISTAR = true;
}

Visitante::~Visitante()
{
    //dtor
}

Visitante::Visitante(int _id, string _nome)
: Utilizador(_id, _nome)
{
    poderLISTAR = true;
}
