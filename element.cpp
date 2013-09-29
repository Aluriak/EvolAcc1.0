#include "main.h"

using namespace std;



//===============================
//========CONSTRUCTEURS==========
//===============================


element::element(int x, int y)
{
    CorrectionCoordonnees(x, y);
    X = x;
    Y = y;
}

//================================
//===========ACCESSEURS===========
//================================


int element::getX()
{
    return X;
}
int element::getY()
{
    return Y;
}

