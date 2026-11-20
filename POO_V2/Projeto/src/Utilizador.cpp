#include "Utilizador.h"

Utilizador::Utilizador()
{
    ID = 0;
    NOME = "";
    poderADICIONAR = false;
    poderREMOVER = false;
    poderLISTAR = false;
    poderRUN = false;
    poderMANUTENCAO = false;
}

Utilizador::~Utilizador()
{
    //dtor
}

Utilizador::Utilizador(int _id, string _nome)
{
    ID = _id;
    NOME = _nome;
    poderADICIONAR = false;
    poderREMOVER = false;
    poderLISTAR = false;
    poderRUN = false;
    poderMANUTENCAO = false;
}
