#include "Normal.h"

Normal::Normal()
: Visitante(0, "")
{
    poderADICIONAR = true;
    poderRUN = true;
    poderMANUTENCAO = true;
}

Normal::~Normal()
{
    //dtor
}

Normal::Normal(int _id, string _nome)
: Visitante(_id, _nome)
{
    poderADICIONAR = true;
    poderRUN = true;
    poderMANUTENCAO = true;
}
