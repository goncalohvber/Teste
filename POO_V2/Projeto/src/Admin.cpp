#include "Admin.h"

Admin::Admin()
: Normal(0, "")
{
    poderREMOVER = true;
}

Admin::~Admin()
{
    //dtor
}

Admin::Admin(int _id, string _nome)
: Normal(_id, _nome)
{
    poderREMOVER = true;
}
