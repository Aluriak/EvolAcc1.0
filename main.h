#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// d�finitions des fichiers
#define FILE_RESULT "Gestion.JEVL"
#define SAVEFILE "Donnees.JEVL"
#define STATFILE "Statistiques.JEVL"
#define ETUDFILE "ETUDFILE.JEVL"
#define NOM_FENETRE "EvolAcc [pyrite]"

enum DefinitionVideo
{
    TAILLE_X = 600, // taille largeur en px
    TAILLE_Y = TAILLE_X, // taille longueur en px
    NB_CASES_X = 60, // nombre de cases en X
    NB_CASES_Y = NB_CASES_X, // nombre de cases en Y
    TAILLE_CASES_X = TAILLE_X / NB_CASES_X, // taille des cases (auto-g�n�r�)
    TAILLE_CASES_Y = TAILLE_Y / NB_CASES_Y, // taille des cases (auto-g�n�r�)
    NB_PHASE_AUTO = 100 // nombre de phase pass�e avec la touche tab
};
enum ReglesEspece1
{
    ESP1_AGE_MIN_REPRO = 5, // age minimum � avoir pour se reproduire
    ESP1_RESERVENOUR_MIN_REPRO = 6, // bouffe minimum � avoir pour se reproduire
    ESP1_VITESSE = 3, // nb de cases parcourues par phase
    ESP1_AGE_MAX = 1000000000, // age maximum
    ESP1_RESERVE_INITIAL = 10, // r�serve de bouffe initiale
    ESP1_VARIABILITE_GENETIQUE = 5, // utilis� lors de la cr�ation d'enfant (variabilit� plus grande si chiffre plus grand, nulle si 0
    ESP1_NBPROG = 5, // nb de gosse cr�� par accouplement
    ESP1_MODEREPRO = 1 // 1 = moyennes; 2 = al�atoires; 0 = parent le mieux nourris
};
enum DefinitionsUnivers
{
    VAL_NOUR_BASE = 4, // valeur de la nourriture de base
    REGEN_NOUR_BASE = 30, // nombre d'apparition de nourriture de base par phase
    RAPPORT_NOURBASE = 3, // rapport (nombre de case o� se trouve de la bouffe initialement) / (nombre de case total)
    // EN COURS DE REALISATION
    NOUR_JDV = 0 // bol�en : si vrai, les NourBase se comportent comme les cellules du jeu de la vie. (r�gles sp�ciales)
};


// enumeration directions
enum DirectionsRelatives {AVANT_DROITE = 2, DROITE = 1, AVANT = 3, GAUCHE = 4, AVANT_GAUCHE = 5};
enum DirectionsAbsolues {NORD = 11, EST = 12, SUD = 13, OUEST = 14};


#include <iostream>
#include <string>
#include <vector>
#include <fstream> // gestion de fichiers
#include <ctime> // biblioth�que obligatoire pour tirer un nombre au hasard
#include <cstdlib> // biblioth�que obligatoire pour tirer un nombre au hasard
#include <cmath>
#include <SDL/SDL.h>
#include "element.h"
#include "Espece1.h"
#include "NourBase.h"
#include "GestionVideo.h"

// fonctions principales
void GestionProgrammeRacine();

void InitialisationJEVL(); // lecture du SAVEFILE et cr�ation des objets pertinents
void AffichageJEVL_SDL(); // affichage de l'univers selon conditions initiales, puis traitement des changement de phase
void afficheVector(std::vector<int> vecteur); // d�buggage : affiche les coordon�es des objets NourBase contenus dans le vector en arg
int ConvertionStringToInt(std::string chaine); // convertit le contenu de chaine en un r�el. ("48" deviens 48)

// fonctions li�es aux coordonn�es
void CorrectionCoordonnees(int &X, int &Y); // modification des coordonn�es envoy�es en arg pour les faire rentrer dans l'univers
bool SontDeMemeCoordonnees(element objet1, element objet2); // indique si les deux objets sont de m�me coordon�es.

// fonctions annexes
int pow(int a, int b); // fonction carr�
    double pow(double a, double b); // surcharge en double
bool estPair(int n); // est pair ?
void convertAsciiToDec(int &chiffre); // utilis�e par ConvertionStringToInt()
void MelangerVector(std::vector<int> &tabl); // pour m�langer un vector
#endif // MAIN_H_INCLUDED
