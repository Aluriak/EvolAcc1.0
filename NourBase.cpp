#include "main.h"

using namespace std;

NourBase::NourBase(int ID, int x, int y, int ValNour) : element(x, y), e_ID(ID)
{
    if(ValNour > 0)
        e_GainNour = ValNour;
    else
        e_GainNour = 1;
}




int NourBase::getGainNour()
{
    return e_GainNour;
}

int NourBase::getID()
{
    return e_ID;
}






