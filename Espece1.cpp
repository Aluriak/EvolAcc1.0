#include "main.h"

using namespace std;

//===============================
//========CONSTRUCTEURS==========
//===============================

Espece1::Espece1(int ID, int x, int y) : element(x, y), e_ID(ID)
{
    // on met les probabilités
    GenererProbMvtAleat();
    // age, réserve de nourriture et coordonnées
    e_age = 0;
    e_CapableReproduction = true;
    e_reserveNour = 10;
    e_DerniereDirection = DirectionAbsolueAleatoire();
    e_Vitesse = ESP1_VITESSE;
}

Espece1::Espece1(int ID, int x, int y, int D, int AD, int A, int AG, int G, int reserve, int age, int direcPrecedente) : element(x, y), e_ID(ID)
{
    // on met les probabilités
    if((A+AD+D+G+AG) != 100)
        GenererProbMvtAleat();
    else
    {
        e_pA = A;
        e_pD = D;
        e_pAD = AD;
        e_pG = G;
        e_pAG = AG;
    }
    e_age = age;
    e_CapableReproduction = true;
    e_reserveNour = reserve;
    e_DerniereDirection = direcPrecedente;
    e_Vitesse = ESP1_VITESSE;
}


//===============================
//=====FONCTIONS GENERALES=======
//===============================


void Espece1::PassagePhaseSuivante() // gère le passage à la phase suivante
{
    e_reserveNour--; // a chaque tour, on perd une unité de réserve
    e_age++; // ... et on prend de l'age !

    for(int i = e_Vitesse; i > 0; i--) // autant de fois que la valeur de vitesse, on opère un déplacement
        deplacement();

    // si il a au moins ESP1_AGE_MIN_REPRO phases d'âge et qu'il a une réserve d'au moins ESP1_RESERVENOUR_MIN_REPRO, alors il peu se reproduire
    if(e_age >= ESP1_AGE_MIN_REPRO && e_reserveNour >= ESP1_RESERVENOUR_MIN_REPRO)
        setCapaciteReproduction(true);
    else
        setCapaciteReproduction(false);
}



//===================================================================================================================================================
//===================================================================================================================================================
//===============================================ACCESSEURS==========================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


int Espece1::getID()
{
    return e_ID;
}
int Espece1::getA()
{
    return e_pA;
}
int Espece1::getAD()
{
    return e_pAD;
}
int Espece1::getD()
{
    return e_pD;
}
int Espece1::getG()
{
    return e_pG;
}
int Espece1::getAG()
{
    return e_pAG;
}
int Espece1::getAge()
{
    return e_age;
}
int Espece1::getDirecPrec()
{
    return e_DerniereDirection;
}
int Espece1::getReserveNour()
{
    return e_reserveNour;
}
void Espece1::GainNourriture(int gain)
{
    e_reserveNour += gain;
}

int Espece1::getCapaciteReproduction()
{
    return e_CapableReproduction;
}
void Espece1::setCapaciteReproduction(bool estCapable)
{
    e_CapableReproduction = estCapable;
}



//===================================================================================================================================================
//===================================================================================================================================================
//===============================================FONCTIONS PRIVEES===================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void Espece1::GenererProbMvtAleat() // pour créer des probabilités de mouvement aléatoires
{
    int nbAleatoire(0), nb(100);
    vector<int> conteneur;

    for(int i = 0; i < 5; i++)
    {
        nbAleatoire = (rand() % nb);
        nb -= nbAleatoire;
        if(i == 4)
            conteneur.push_back(nb);
        else
            conteneur.push_back(nbAleatoire);
    }
    MelangerVector(conteneur);

    e_pA = conteneur[0];
    e_pD = conteneur[1];
    e_pAD = conteneur[2];
    e_pAG = conteneur[3];
    e_pG = conteneur[4];



    while(e_pD+e_pAD+e_pA+e_pAG+e_pG < 100)
    {
        nbAleatoire = rand() % 5; // nb aléatoire entre 0 et 4
        if(nbAleatoire == 0)
            e_pA++;
        if(nbAleatoire == 1)
            e_pD++;
        if(nbAleatoire == 2)
            e_pAD++;
        if(nbAleatoire == 3)
            e_pAG++;
        if(nbAleatoire == 4)
            e_pG++;
    }
    while(e_pD+e_pAD+e_pA+e_pAG+e_pG > 100)
    {
        nbAleatoire = rand() % 5; // nb aléatoire entre 0 et 4
        if(nbAleatoire == 0)
            e_pA--;
        if(nbAleatoire == 1)
            e_pD--;
        if(nbAleatoire == 2)
            e_pAD--;
        if(nbAleatoire == 3)
            e_pAG--;
        if(nbAleatoire == 4)
            e_pG--;
    }
}

int Espece1::DirectionRelativeAleatoire(int D, int AD, int A, int AG, int G) // détermine direction au hasard selon les proba envoyées en argument (25% chacune par défaut)
{
    int nb((rand() % 100) + 1); // nb aléatoire entre 1 et 100
        // cette valeur sera utilisée pour trouver une direction aléatoire
// on considère l'ordre de 1 à 100 occupé par DROITE, AVANT_DROITE, AVANT, AVANT_GAUCHE, GAUCHE

    nb -= D;
    if(nb <= 0)
        return DROITE;

    nb -= AD;
    if(nb <= 0)
        return AVANT_DROITE;

    nb -= A;
    if(nb <= 0)
        return AVANT;

    nb -= AG;
    if(nb <= 0)
        return AVANT_GAUCHE;

    nb -= G;
    if(nb <= 0)
        return GAUCHE;

    return AVANT;
}

int Espece1::DirectionAbsolueAleatoire()
{
    int nb = rand() % 4;
    if(nb == 0)
        return NORD;
    else if(nb == 1)
        return OUEST;
    else if(nb == 2)
        return EST;
    else
        return SUD;
}

void Espece1::deplacement() // gère le déplacement de la bestiole
{
    int uniteDeplacementY = TAILLE_CASES_Y, uniteDeplacementX = TAILLE_CASES_X;
    int nb = DirectionRelativeAleatoire(e_pD, e_pAD, e_pA, e_pAG, e_pG);/*
ofstream err("Erreur.txt", ios::app);
err << e_DerniereDirection << endl;//*/
    if(e_DerniereDirection == NORD) // si la dernière fois elle a avancé (en absolu !)
    {
        if(nb == AVANT) // déplacement relatif
        {
            e_DerniereDirection = NORD; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == DROITE)
        {
            e_DerniereDirection = EST; // alors le dernier déplacement en absolu est
            X += uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == GAUCHE)
        {
            e_DerniereDirection = OUEST; // alors le dernier déplacement en absolu est
            X -= uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == AVANT_DROITE)
        {
            e_DerniereDirection = EST; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X += uniteDeplacementX;
        }
        if(nb == AVANT_GAUCHE)
        {
            e_DerniereDirection = OUEST; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X -= uniteDeplacementX;
        }
    }
    else if(e_DerniereDirection == EST) // si la dernière fois elle a avancé (en absolu !)
    {
        if(nb == AVANT) // déplacement relatif
        {
            e_DerniereDirection = EST; // alors le dernier déplacement en absolu est
            X += uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == DROITE)
        {
            e_DerniereDirection = SUD; // alors le dernier déplacement en absolu est
            Y += uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == GAUCHE)
        {
            e_DerniereDirection = NORD; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == AVANT_DROITE)
        {
            e_DerniereDirection = SUD; // alors le dernier déplacement en absolu est
            Y += uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X += uniteDeplacementX;
        }
        if(nb == AVANT_GAUCHE)
        {
            e_DerniereDirection = NORD; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X += uniteDeplacementX;
        }
    }
    else if(e_DerniereDirection == OUEST) // si la dernière fois elle a avancé (en absolu !)
    {
        if(nb == AVANT) // déplacement relatif
        {
            e_DerniereDirection = OUEST; // alors le dernier déplacement en absolu est
            X -= uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == DROITE)
        {
            e_DerniereDirection = NORD; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == GAUCHE)
        {
            e_DerniereDirection = SUD; // alors le dernier déplacement en absolu est
            Y += uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == AVANT_DROITE)
        {
            e_DerniereDirection = NORD; // alors le dernier déplacement en absolu est
            Y -= uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X -= uniteDeplacementX;
        }
        if(nb == AVANT_GAUCHE)
        {
            e_DerniereDirection = SUD; // alors le dernier déplacement en absolu est
            Y += uniteDeplacementY; // les coordonnées sont modifiées en conséquences
            X -= uniteDeplacementX;
        }
    }
    else
    {
        if(nb == AVANT) // déplacement relatif
        {
            e_DerniereDirection = SUD; // alors le dernier déplacement en absolu est
            Y += uniteDeplacementY; // les coordonnées sont modifiées en conséquences
        }
        if(nb == DROITE)
        {
            e_DerniereDirection = OUEST; // alors le dernier déplacement en absolu est
            X -= uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == GAUCHE)
        {
            e_DerniereDirection = EST; // alors le dernier déplacement en absolu est
            X += uniteDeplacementX; // les coordonnées sont modifiées en conséquences
        }
        if(nb == AVANT_DROITE)
        {
            e_DerniereDirection = OUEST; // alors le dernier déplacement en absolu est
            X -= uniteDeplacementX; // les coordonnées sont modifiées en conséquences
            Y += uniteDeplacementY;
        }
        if(nb == AVANT_GAUCHE)
        {
            e_DerniereDirection = EST; // alors le dernier déplacement en absolu est
            X += uniteDeplacementX; // les coordonnées sont modifiées en conséquences
            Y += uniteDeplacementY;
        }
    }
    // on vérifie que X et Y ne sortent pas des clous, sinon on met en jeu la récursivité
    CorrectionCoordonnees(X, Y);
}



