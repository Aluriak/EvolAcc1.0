#include "main.h"

using namespace std;



//===================================================================================================================================================
//===================================================================================================================================================
//======================================================MAIN=========================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


int main(int argc, char *argv[])
{
    ofstream ext(FILE_RESULT);
    ext << "!=============================================================================!" << endl << "!===================================EvoLAcc [Pyrite]==========================!";
    ext << endl << "!=============================================================================!" << endl << endl << endl;
    ofstream stat(STATFILE);
    stat << "!=============================================================================!" << endl << "!===================================EvoLAcc [Pyrite]==========================!";
    stat << endl << "!=============================================================================!" << endl << endl << endl;
    ofstream etu(ETUDFILE);
    etu << "!=============================================================================!" << endl << "!===================================EvoLAcc [Pyrite]==========================!";
    etu << endl << "!=============================================================================!" << endl << endl << endl;



    // on lance l'aléatoire, car c'est UNE SEULE FOIS PAR PROGRAMME
    srand(time(0));

    // et on lance le programme !
    GestionProgrammeRacine();


    return 0;
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION GENERALES================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void GestionProgrammeRacine()
{
    GestionVideo w(RAPPORT_NOURBASE);
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION ANNEXES==================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void CorrectionCoordonnees(int &X, int &Y) // modifie X et Y de façon à ce qu'ils rentrent dans l'univers
{
    while(X >= TAILLE_X) X -= TAILLE_X; // si X trop grand, on l'amène au bon endroit par récursivité
    while(X < 0) X += TAILLE_X; // si X trop petit, on l'amène au bon endroit par récursivité
    while(Y >= TAILLE_Y) Y -= TAILLE_Y; // si Y trop grand, on l'amène au bon endroit par récursivité
    while(Y < 0) Y += TAILLE_Y; // si Y trop petit, on l'amène au bon endroit par récursivité
}


int ConvertionStringToInt(string chaine)
{
    unsigned int taille = chaine.size();
    int resultat = 0, chiffre(0);

    // on effectue ensuite le traitement normal
    for(unsigned int i = 1; i <= taille; i++)
    {
        // on prend la valeur de chaine[i] dans chiffre
        chiffre = chaine[i-1];
        convertAsciiToDec(chiffre); // on convertit  chiffre en nombre décimal depuis ASCII
        chiffre *= pow(10, taille-i); // on multiplie chiffre par 10 puissance taille-i.
        resultat += chiffre;
    }
    return resultat;
}


void convertAsciiToDec(int &chiffre)
{
    if(chiffre >= 48 && chiffre <= 57)
        chiffre -= 48;
}
// implémentation d'une fonction carré
int pow(int a, int b)
{
    int resultat = 1;
    for(int i = 1; i <= b; i++)
        resultat *= a;
    if(estPair(b) && resultat < 0)
        resultat *= -1;
    return resultat;
}

double pow(double a, int b)
{
    double resultat = 1;
    for(int i = 1; i <= b; i++)
        resultat *= a;
    if(estPair(b) && resultat < 0)
        resultat *= -1;

    return resultat;
}

bool SontDeMemeCoordonnees(element objet1, element objet2)
{
    if((objet1.getX() == objet2.getX()) && (objet1.getY() == objet2.getY()))
        return true;
    return false;
}

bool estPair(int n)
{
    if(n % 2 == 0)
        return true;
    return false;
}


void afficheVector(vector<int> vecteur)
{
    ofstream ext(FILE_RESULT, ios::app);
    for(unsigned int i = 0; i < vecteur.size(); i++)
        ext << vecteur[i] << endl;
}

void MelangerVector(std::vector<int> &tabl)
{
    int nb1(0), nb2(0), inter;
    int taille = tabl.size()*5;

    for(int i = 0; i < taille; i++)
    {
        nb1 = rand() % tabl.size();
        nb2 = rand() % tabl.size();
        if(nb1 != nb2)
        {
            inter = tabl[nb1]; // permutation des deux valeurs choisies aléatoirement.
            tabl[nb1] = tabl[nb2];
            tabl[nb2] = inter;
        }
    }
}






