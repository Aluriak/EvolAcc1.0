#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

// définitions des fichiers
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
    TAILLE_CASES_X = TAILLE_X / NB_CASES_X, // taille des cases (auto-généré)
    TAILLE_CASES_Y = TAILLE_Y / NB_CASES_Y, // taille des cases (auto-généré)
    NB_PHASE_AUTO = 100 // nombre de phase passée avec la touche tab
};
enum ReglesEspece1
{
    ESP1_AGE_MIN_REPRO = 5, // age minimum à avoir pour se reproduire
    ESP1_RESERVENOUR_MIN_REPRO = 6, // bouffe minimum à avoir pour se reproduire
    ESP1_VITESSE = 3, // nb de cases parcourues par phase
    ESP1_AGE_MAX = 1000000000, // age maximum
    ESP1_RESERVE_INITIAL = 10, // réserve de bouffe initiale
    ESP1_VARIABILITE_GENETIQUE = 5, // utilisé lors de la création d'enfant (variabilité plus grande si chiffre plus grand, nulle si 0
    ESP1_NBPROG = 5, // nb de gosse créé par accouplement
    ESP1_MODEREPRO = 1 // 1 = moyennes; 2 = aléatoires; 0 = parent le mieux nourris
};
enum DefinitionsUnivers
{
    VAL_NOUR_BASE = 4, // valeur de la nourriture de base
    REGEN_NOUR_BASE = 30, // nombre d'apparition de nourriture de base par phase
    RAPPORT_NOURBASE = 3, // rapport (nombre de case où se trouve de la bouffe initialement) / (nombre de case total)
    // EN COURS DE REALISATION
    NOUR_JDV = 0 // boléen : si vrai, les NourBase se comportent comme les cellules du jeu de la vie. (règles spéciales)
};


// enumeration directions
enum DirectionsRelatives {AVANT_DROITE = 2, DROITE = 1, AVANT = 3, GAUCHE = 4, AVANT_GAUCHE = 5};
enum DirectionsAbsolues {NORD = 11, EST = 12, SUD = 13, OUEST = 14};


#include <iostream>
#include <string>
#include <vector>
#include <fstream> // gestion de fichiers
#include <ctime> // bibliothèque obligatoire pour tirer un nombre au hasard
#include <cstdlib> // bibliothèque obligatoire pour tirer un nombre au hasard
#include <cmath>
#include <SDL/SDL.h>
#include "element.h"
#include "Espece1.h"
#include "NourBase.h"
#include "GestionVideo.h"

// fonctions principales
void GestionProgrammeRacine();

void InitialisationJEVL(); // lecture du SAVEFILE et création des objets pertinents
void AffichageJEVL_SDL(); // affichage de l'univers selon conditions initiales, puis traitement des changement de phase
void afficheVector(std::vector<int> vecteur); // débuggage : affiche les coordonées des objets NourBase contenus dans le vector en arg
int ConvertionStringToInt(std::string chaine); // convertit le contenu de chaine en un réel. ("48" deviens 48)

// fonctions liées aux coordonnées
void CorrectionCoordonnees(int &X, int &Y); // modification des coordonnées envoyées en arg pour les faire rentrer dans l'univers
bool SontDeMemeCoordonnees(element objet1, element objet2); // indique si les deux objets sont de même coordonées.

// fonctions annexes
int pow(int a, int b); // fonction carré
    double pow(double a, double b); // surcharge en double
bool estPair(int n); // est pair ?
void convertAsciiToDec(int &chiffre); // utilisée par ConvertionStringToInt()
void MelangerVector(std::vector<int> &tabl); // pour mélanger un vector
#endif // MAIN_H_INCLUDED
