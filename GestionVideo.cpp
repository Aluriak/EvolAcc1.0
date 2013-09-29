#include "main.h"

using namespace std;



//===================================================================================================================================================
//=================================================CONSTRUCTEURS=====================================================================================
//===================================================================================================================================================


GestionVideo::GestionVideo(int rapportNourritureBasique) : tablEsp1(), nbEsp1Cree(0), nbNourBaseCree(0)
{
    nbPhasePassee = 0;

    if(rapportNourritureBasique > 1)
        e_rapport_NourBase = rapportNourritureBasique;
    else
        e_rapport_NourBase = 2;
    InitialisationJEVL(); // on intialise le programme
    AffichageJEVL_SDL(); // on le lance
}



//===================================================================================================================================================
//=================================================FONCTION DE PAUSE=================================================================================
//===================================================================================================================================================


void GestionVideo::pause(SDL_Surface *ecran)
{
    AjoutComposantesNourritureJEVL();
    RemplissageEcranJEVL_SDL(ecran, true);
    SDL_Flip(ecran);
    SDL_Event event;
        SDL_EnableKeyRepeat(500,50);
    int continuer = 1;
    while(continuer)
    {
        GestionCapaciteRepro_Esp1_JEVL();
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    continuer=0;
                else if(event.key.keysym.sym == SDLK_SPACE) // passage normal
                {
                    nbPhasePassee++; // on incrémente le nombre de tours.
                    PassagePhaseVecteur();
                    RemplissageEcranJEVL_SDL(ecran);
                }
                else if(event.key.keysym.sym == SDLK_RETURN) // passage avec RAZ de la bouffe
                {
                    nbPhasePassee++; // on incrémente le nombre de tours.
                    AjoutComposantesNourritureJEVL();
                    PassagePhaseVecteur();
                    RemplissageEcranJEVL_SDL(ecran);
                }
                else if(event.key.keysym.sym == SDLK_TAB) // passage auto sur NB_PHASE_AUTO phases, avec stats avant et après
                {
                    int nb = NB_PHASE_AUTO;
                    GenererStats();
                    while(nb)
                    {
                        nbPhasePassee++;
                        PassagePhaseVecteur();
                        if(nb == (NB_PHASE_AUTO/2))
                            GenererStats();
                        nb--;
                    }
                    RemplissageEcranJEVL_SDL(ecran);
                    GenererStats();
                }
                else if (event.key.keysym.sym == SDLK_s) // faire des statistiques après un passage de tour normal
                {
                    nbPhasePassee++; // on incrémente le nombre de tours.
                    PassagePhaseVecteur();
                    RemplissageEcranJEVL_SDL(ecran);
                    GenererStats();
                }
                else if (event.key.keysym.sym == SDLK_i) // calculs sur beaucoup de phases
                {
                    int nb = NB_PHASE_AUTO*50;
                    GenererStats();
                    while(nb)
                    {
                        nbPhasePassee++;
                        PassagePhaseVecteur();
                        if((nb % NB_PHASE_AUTO)) // si nb est un multiple de NB_PHASE_AUTO
                            GenererStats();
                        nb--;
                    }
                }
                break;

            default:
                continuer = 1; // on teste toutes les valeurs possibles, donc !
                break;
        }
        SDL_Flip(ecran);
    }
    SDL_FreeSurface(ecran);
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION DE PHASE=================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void GestionVideo::PassagePhaseVecteur()
{
    ofstream ext(FILE_RESULT, ios::app);

    vector<Espece1> tablBin;
    vector<NourBase> tablBinNourBase;
    int bin(0);

    // on fait changer de phase toutes les Espece1
    for(unsigned int i = 0; i < tablEsp1.size(); i++)
    {
        tablEsp1[i].PassagePhaseSuivante(); // on intialise les valeurs au passage suivant

        // maintenant, on ajoute au vector support les Espce1 encore vivantes
        if(tablEsp1[i].getReserveNour() > 0 && tablEsp1[i].getAge() < ESP1_AGE_MAX) // si la réserve de nourriture n'est pas nulle, et qu'il n'est pas trop vieux
            tablBin.push_back(tablEsp1[i]);
        else if (tablEsp1[i].getReserveNour() <= 0)
            ext << "EVOLUTION: objet Espece1  (ID" << i << ") est mort affamé (Phase-" << nbPhasePassee << ")" << endl;
        else if (tablEsp1[i].getAge() >= ESP1_AGE_MAX)
            ext << "EVOLUTION: objet Espece1  (ID" << i << ") est mort naturellement (Phase-" << nbPhasePassee << ")" << endl;

    }


    for(unsigned int i = 0; i < TAILLE_X; i += TAILLE_CASES_X)
    {
        for(unsigned int u = 0; u < TAILLE_Y; u += TAILLE_CASES_Y)
        {
            bin = NourriturePresente(i,u);
            vector<int> bash = Espece1ReproductivePresente(i, u);
            if(bash.size() == 0) // si pas d'Espece1
            {
                if(bin != -1) // mais qu'il y a de la bouffe !
                tablBinNourBase.push_back(tablNourBase[bin]); // on ajoute cette bouffe dans la liste des bouffes qui resteront à afficher.
            }
            else // si une ou plusieurs Espece1
            {
                if(bin != -1) // et qu'il y a de la bouffe !
                {
                    int e = tablNourBase[bin].getGainNour();
                    for(unsigned int o = 0; o < bash.size(); o++)
                        tablBin[bash[o]].GainNourriture(e); // chacune gagne le agin proposé par la NourBase
                }
                // quoi qu'il en soit, on procède à la procréation
                if(!estPair(bash.size())) // si nombre impair de Espece1, la dernière de la liste ne se repoduit pas
                    bash.pop_back();
                // procréation lancées :
                for(unsigned int o = 0; o < bash.size(); o +=2)
                {
                    for(int u = 0; u < ESP1_NBPROG; u++)
                        tablBin.push_back(EnfantEsp1(&tablBin[bash[o]], &tablBin[bash[o+1]])); // enfants de chacun des couples rajouté à la liste
                    ext << "EVOLUTION: objets Espece1 (ID" << tablBin[bash[o]].getID() << " et ID" << tablBin[bash[o+1]].getID() << ") ont procréé (ID" << nbEsp1Cree-1 << ") (Phase-" << nbPhasePassee << ")" << endl;
                }
                // les enfants ont étés ajoutés.
            }
        }
    }

    // on remet tablesp1 en place, en fonction de tablBin
    while(tablEsp1.size())      tablEsp1.pop_back();
    for(unsigned int i = 0; i < tablBin.size(); i++)
        tablEsp1.push_back(tablBin[i]);
    GestionCapaciteRepro_Esp1_JEVL();

    // pareil pour tablNourBase
    while(tablNourBase.size())      tablNourBase.pop_back();
    for(unsigned int i = 0; i < tablBinNourBase.size(); i++)
        tablNourBase.push_back(tablBinNourBase[i]);
    GestionApparition_NourBase_JEVL();
}



//===================================================================================================================================================
//===================================================================================================================================================
//===============================================FONCTION D'AFFICHAGE================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void GestionVideo::RemplissageEcranJEVL_SDL(SDL_Surface *ecran, bool RenseignerFichier) // affchage du contenu à l'écran !
{
    ofstream ext(FILE_RESULT, ios::app);
    ColorerSurface_SDL(ecran, 0, 0, TAILLE_X, TAILLE_Y, 0, 0, 0); // l'ecran entier en noir!
    int x, y, Red, Green, Blue;
//*
    //                  NOURBASE
    for(unsigned int i = 0; i < tablNourBase.size(); i++)
    {
        x = tablNourBase[i].getX();
        y = tablNourBase[i].getY();
        CorrectionCoordonnees(x, y);
        ColorerSurface_SDL(ecran, x, y, TAILLE_CASES_X, TAILLE_CASES_Y, 39, 213, 9);
    }//*/

    //                  ESPECE1
    for(unsigned int i = 0; i < tablEsp1.size(); i++)
    {
        x = tablEsp1[i].getX();
        y = tablEsp1[i].getY();
        CorrectionCoordonnees(x, y);
        Green = 0;
        Red = 0;
        Blue = 255;
        /*
        Green = 100 + i;
        Red = 100 - i;
        Blue = 255;
        while(Red > 255)    Red -= 255;
        while(Red < 0)    Red += 255;
        while(Green > 255)    Green -= 255;
        while(Green < 0)    Green += 255;
        while(Blue > 255)    Blue -= 255;
        while(Blue < 0)    Blue += 255;//*/
        ColorerSurface_SDL(ecran, x, y, TAILLE_CASES_X, TAILLE_CASES_Y, Red, Green, Blue);
        if(RenseignerFichier)
            ext << "AFFICHAGE_SDL: Objet Espece1  (ID" << i << ") a été affiché aux coordonnées (" << x << ";" << y << ")." << endl;
    }
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION GENERALES================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void GestionVideo::InitialisationJEVL()
{
    ofstream ext(FILE_RESULT, ios::app);
    ifstream sav(SAVEFILE);
    if(!sav)
        ext << "ERREUR: Ouverture de " << SAVEFILE << " a échouée !" << endl;
    else
    {
        string ligne(""), carac("");
        int x(0), y(0), age(0), reserve(0), D(0), AD(0), A(0), AG(0), G(0), bin(0), direcPrec(0), compteur(0);
        while(getline(sav, ligne)) // on met ligne par ligne le contenu de sav dans ligne, jusqu'à fin du fichier
        { // les lignes sont donc traitées dans la boucle
            compteur = 0; // il faut bien que le prochain objet recommence à zéro !

            for(unsigned int i = 0; i < ligne.size() && ligne[i] != ':'; i++)
                carac+=ligne[i];

            if(carac == "Espece1")
            {
                bin = carac.size();
                carac = "";
                for(unsigned int i = bin + 1; i < ligne.size(); i++) // on lit à partir du caractère après le double point
                {
                    if(ligne[i] != ',')
                        carac += ligne[i];
                    else
                    {
                        compteur++;
                        if(carac == "?")
                        {
                            if(compteur == 1)       x = NombreAleatoireDefini_JEVL("x");
                            else if(compteur == 2)  y = NombreAleatoireDefini_JEVL("y");
                            else if(compteur == 3)  D = NombreAleatoireDefini_JEVL("D");
                            else if(compteur == 4)  AD = NombreAleatoireDefini_JEVL("AD");
                            else if(compteur == 5)  A = NombreAleatoireDefini_JEVL("A");
                            else if(compteur == 6)  AG = NombreAleatoireDefini_JEVL("AG");
                            else if(compteur == 7)  G = NombreAleatoireDefini_JEVL("G");
                            else if(compteur == 8)  reserve = NombreAleatoireDefini_JEVL("Reserve");
                            else if(compteur == 9)  age = NombreAleatoireDefini_JEVL("Age");
                            else                    direcPrec = NombreAleatoireDefini_JEVL("Direc");
                        }
                        else
                        {
                            if(compteur == 1)       x = ConvertionStringToInt(carac);
                            else if(compteur == 2)  y = ConvertionStringToInt(carac);
                            else if(compteur == 3)  D = ConvertionStringToInt(carac);
                            else if(compteur == 4)  AD = ConvertionStringToInt(carac);
                            else if(compteur == 5)  A = ConvertionStringToInt(carac);
                            else if(compteur == 6)  AG = ConvertionStringToInt(carac);
                            else if(compteur == 7)  G = ConvertionStringToInt(carac);
                            else if(compteur == 8)  reserve = ConvertionStringToInt(carac);
                            else if(compteur == 9)  age = ConvertionStringToInt(carac);
                            else                    direcPrec = ConvertionStringToInt(carac);
                        }
                        carac = "";
                    }
                } // fin lecture de chaine
                x *= TAILLE_CASES_X; // on met les coordonées comme il faut
                y *= TAILLE_CASES_Y;
                CorrectionCoordonnees(x, y);
                tablEsp1.push_back(Espece1(nbEsp1Cree, x, y, D, AD, A, AG, G, reserve, age, direcPrec));

                ext << "INITIALISATION: objet Espece1(" << nbEsp1Cree << "," << tablEsp1.back().getX() << "," << tablEsp1.back().getY() << "," << tablEsp1.back().getD() << "," << tablEsp1.back().getAD() << "," << tablEsp1.back().getA() << "," << tablEsp1.back().getAG() << "," << tablEsp1.back().getG() << "," << reserve << "," << age << "," << direcPrec << ") pour ligne '" << ligne << "'" << endl;
                nbEsp1Cree++;
            } // fin condition ESPECE1
            else
                ext << "ERREUR: " << FILE_RESULT <<" erroné" << endl << "               Ligne lue : '" << ligne << "'" << endl << "      carac lu : '" << carac << "'" << endl;
            ligne = "";
            carac = "";
        } // fin lecture fichier
    }
}



void GestionVideo::AffichageJEVL_SDL() // initialisation vidéo des données récupérées, et relancement des phases
{
    SDL_Init(SDL_INIT_VIDEO); //Initialisation du module vidéo de la SDL.
    SDL_Surface *ecran; // on pointe la surface écran, histoire de pouvoir y accéder.
    //réglages des paramètres vidéos : largeur, longueur, nb couleur, options (flags)
    ecran = SDL_SetVideoMode(TAILLE_X, TAILLE_Y, 32, SDL_DOUBLEBUF | SDL_HWSURFACE); // le pointeur SDL surface récupère le pointeur vers la surface créée par SetVideoMode
    SDL_WM_SetCaption(NOM_FENETRE, NULL); //modification du nom de la fenêtre

    SDL_Flip(ecran); // màj écran
    pause(ecran); // gestion affichage, passage de phase et réaffichage
    SDL_Quit();
}



void GestionVideo::ColorerSurface_SDL(SDL_Surface *ecran, int x, int y, int longueur, int largeur, int Red, int Green, int Blue)
{
    SDL_Surface *Rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, longueur, 32, 0,0,0,0);
    CorrectionCoordonnees(x, y);
    SDL_Rect Position;
    Position.x = x;
    Position.y = y;
    SDL_FillRect(Rectangle, NULL, SDL_MapRGB(ecran->format, Red, Green, Blue)); // couleur noire
    SDL_BlitSurface(Rectangle, NULL, ecran, &Position);
    SDL_FreeSurface(Rectangle);
    SDL_FreeSurface(ecran);
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION ANNEXES==================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


vector<Espece1> GestionVideo::getVecteur()
{
    return tablEsp1;
}



Espece1 GestionVideo::EnfantEsp1(Espece1 *pere, Espece1 *mere)
{
    int A, AD, D, AG, G, bin;
    pere->setCapaciteReproduction(false); // ils ne peuvent plus se reproduire ce tour-ci
    mere->setCapaciteReproduction(false);
    if(ESP1_MODEREPRO == 1) // dans le cas où la règle est : moyenne des deux parents
    {
        A = (pere->getA() + mere->getA()) / 2;
        AD = (pere->getAD() + mere->getAD()) / 2;
        AG = (pere->getAG() + mere->getAG()) / 2;
        D = (pere->getD() + mere->getD()) / 2;
        G = (pere->getG() + mere->getG()) / 2;
    }
    else if(ESP1_MODEREPRO == 0) // dans le cas où la règle est : le parent le plus nourris donne ses probabilité
    {
        if(pere->getReserveNour() > mere->getReserveNour()) // si c'est le père le plus nourris
        {
            A = pere->getA();
            AD = pere->getAD();
            AG = pere->getAG();
            D = pere->getD();
            G = pere->getG();
        }
        else if(pere->getReserveNour() < mere->getReserveNour()) // si c'est la mère le plus nourris
        {
            A = mere->getA();
            AD = mere->getAD();
            AG = mere->getAG();
            D = mere->getD();
            G = mere->getG();
        }
        else // si ils ont pareils, c'est un des deux au hasard
        {
            bin  = rand() % 2; // bin = 0 ou 1
            if(bin)// si 1, on prend le père. Sinon la mère
            {
                A = pere->getA();
                AD = pere->getAD();
                AG = pere->getAG();
                D = pere->getD();
                G = pere->getG();
            }
            else
            {
                A = mere->getA();
                AD = mere->getAD();
                AG = mere->getAG();
                D = mere->getD();
                G = mere->getG();
            }
        }
    }
    else if(ESP1_MODEREPRO == 2) // si c'est du aléatoire
    {
        A = AD = AG = G = D = 100; // on met tout à 100. Le constructeur de la classe saura quoi faire
    }

    // gestion de variabilité génétique
    if(ESP1_VARIABILITE_GENETIQUE > 0)
    {
        int var = (rand() % ESP1_VARIABILITE_GENETIQUE) + (ESP1_VARIABILITE_GENETIQUE/2); // on prend une valeur de variabilité dépendant le celle de l'espèce
        var = VerifViabiliteVariabiliteGenetique(D, AD, A, AG, G, var);
        if(var)
        {
            int i = rand() % 5, u = rand() % 5;
            if(i == 0)
                A -= var;
            else if(i == 1)
                AD -= var;
            else if(i == 2)
                D -= var;
            else if(i == 3)
                AG -= var;
            else
                G -= var;

            if(u == 0)
                A += var;
            else if(u == 1)
                AD += var;
            else if(u == 2)
                D += var;
            else if(u == 3)
                AG += var;
            else
                G += var;
        }
    } // fin gestion de variabilité génétique
    nbEsp1Cree++;
    return Espece1(nbEsp1Cree-1, pere->getX(), pere->getY(), D, AD, A, AG, G, 10, 0, mere->getX());
}


bool GestionVideo::VerifViabiliteVariabiliteGenetique(int D, int AD, int A, int AG, int G, int var) // renvois la valeur max de variabilité pour les probs en arguments
{
    while(var)
    {
        if((D + var <= 100 && D - var > 0) && (AD + var <= 100 && AD - var > 0) && (A + var <= 100 && A - var > 0) && (AG + var <= 100 && AG - var > 0) && (G + var <= 100 && G - var > 0))
            return var;
        else
            var--;
    }
    return 0;
}


void GestionVideo::AjoutComposantesNourritureJEVL()
{
    int rapport = e_rapport_NourBase, nb;

    while(tablNourBase.size() > 0)
        tablNourBase.pop_back();

    for(int i = 0; i < NB_CASES_X; i++)
    {
        for(int u = 0; u < NB_CASES_Y; u++)
        {
            nb = rand() % rapport;
            if(nb == 0)
            {
                tablNourBase.push_back(NourBase(nbNourBaseCree, i*TAILLE_CASES_X, u*TAILLE_CASES_Y, VAL_NOUR_BASE));
                nbNourBaseCree++;
            }
        }
    }
}


void GestionVideo::GestionCapaciteRepro_Esp1_JEVL()
{
    for(unsigned int i = 0; i < tablEsp1.size(); i++)
    {
        if(tablEsp1[i].getAge() >= ESP1_AGE_MIN_REPRO && tablEsp1[i].getReserveNour() >= ESP1_RESERVENOUR_MIN_REPRO)
            tablEsp1[i].setCapaciteReproduction(true);
    }
}


int GestionVideo::NourriturePresente(int x, int y) // renvois l'ID de la bouffe s'il y en aux coordonnées données en argument, ou renvois -1.
{
    for(unsigned int i = 0; i < tablNourBase.size(); i++)
    {
        if(tablNourBase[i].getX() == x && tablNourBase[i].getY() == y)
            return i;
    }
    return -1;
}

vector<int> GestionVideo::Espece1ReproductivePresente(int x, int y) // renvois les ID des Espece1 reproductives s'il y en aux coordonnées données en argument
{
    vector<int> bash;
    for(unsigned int i = 0; i < tablEsp1.size(); i++)
    {
        if(tablEsp1[i].getX() == x && tablEsp1[i].getY() == y && tablEsp1[i].getCapaciteReproduction())
            bash.push_back(i);
    }
    return bash;
}

int GestionVideo::NombreAleatoireDefini_JEVL(string var)
{
    int chaine(0);
        if(var == "x")                        chaine = (rand() % NB_CASES_X) * TAILLE_CASES_X;
        else if(var == "y")                   chaine = (rand() % NB_CASES_Y) * TAILLE_CASES_Y;
        else if(var == "D")                   chaine = (rand() % 20) +1;
        else if(var == "AD")                  chaine = (rand() % 20) +1;
        else if(var == "A")                   chaine = (rand() % 20) +1;
        else if(var == "AG")                  chaine = (rand() % 20) +1;
        else if(var == "G")                   chaine = (rand() % 20) +1;
        else if(var == "Reserve")             chaine = (rand() % ESP1_RESERVE_INITIAL) + (ESP1_RESERVE_INITIAL/2);
        else if(var == "Age")                 chaine = ESP1_AGE_MIN_REPRO;
        else if(var == "Direc")               chaine = (rand() % 4) + 11; // aléatoire entre 11 et 14, cf énumérations en main.h
    return chaine;
}

void GestionVideo::GestionApparition_NourBase_JEVL()
{
    int x, y, nb;

    if(REGEN_NOUR_BASE > 0)
    {
        nb = rand() % REGEN_NOUR_BASE;
        while(nb)
        {
            x = (rand() % NB_CASES_X) * TAILLE_CASES_X;
            y = (rand() % NB_CASES_Y) * TAILLE_CASES_Y;
            if(NourriturePresente(x,y) == -1) // pour éviter d'avoir plusieurs NourBase au même endroit. (la fct renvois -1 si pas de bouffe)
            {
                tablNourBase.push_back(NourBase(nbNourBaseCree, x, y, VAL_NOUR_BASE));
                nbNourBaseCree++;
            }
            nb--;
        }
    }
}



//===================================================================================================================================================
//===================================================================================================================================================
//=================================================FONCTION DE STAT==================================================================================
//===================================================================================================================================================
//===================================================================================================================================================


void GestionVideo::GenererStats()
{
    ofstream stat(STATFILE, ios::app);

    stat << endl << endl << "Probabilités à la phase " << nbPhasePassee << " :" << endl;
    double moy_D(0), moy_AD(0), moy_A(0), moy_AG(0), moy_G(0); // contient les moyennes
    static double Evmoy_D(0), Evmoy_AD(0), Evmoy_A(0), Evmoy_AG(0), Evmoy_G(0); // contient les moyennes précédentes
    static int compteurEsp1(0); // pour compter les Esp1, et pouvoir faire une évolution
    double ecart_type_D(0), ecart_type_AD(0), ecart_type_A(0), ecart_type_AG(0), ecart_type_G(0); // contient les écart-types
    int nb(0);


    //===================================================
    // calcul des moyennes et des évolutions de moyenne :
    //===================================================
        stat << "IDX      :    D          AD          A          AG          G" << endl;
    for(unsigned int i = 0; i < tablEsp1.size(); i++)
    {
        stat << "ID" << tablEsp1[i].getID() << "     :    " << tablEsp1[i].getD() << "          " << tablEsp1[i].getAD() << "          " << tablEsp1[i].getA() << "          " << tablEsp1[i].getAG() << "          " << tablEsp1[i].getG() << endl;
        moy_D += tablEsp1[i].getD();
        moy_AD += tablEsp1[i].getAD();
        moy_A += tablEsp1[i].getA();
        moy_AG += tablEsp1[i].getAG();
        moy_G += tablEsp1[i].getG();
        nb++;
    }
    moy_D = moy_D/nb;
    moy_AD = moy_AD/nb;
    moy_A = moy_A/nb;
    moy_AG = moy_AG/nb;
    moy_G = moy_G/nb;



    // CALCUL DES EVOLUTIONS
    // on calcule l'évolution
    Evmoy_D = moy_D - Evmoy_D;
    Evmoy_AD = moy_AD - Evmoy_AD;
    Evmoy_A = moy_A - Evmoy_A;
    Evmoy_AG = moy_AG - Evmoy_AG;
    Evmoy_G = moy_G - Evmoy_G;


    // affichage dans le fichier
    stat << endl << "ESPECE1  :    nombre : " << nb << ";  évolution : " << nb - compteurEsp1 << endl;
    stat << endl << "MOYENNES :    " << moy_D << "          " << moy_AD << "          " << moy_A << "          " << moy_AG << "          " << moy_G << endl;
    stat << "EVOL MOY :    " << Evmoy_D << "          " << Evmoy_AD << "          " << Evmoy_A << "          " << Evmoy_AG << "          " << Evmoy_G << endl;


    // on passe les moyennes dans les Evmoy, histoire de pouvoir calculer l'évolution la prochaine fois
    Evmoy_D = moy_D;
    Evmoy_AD = moy_AD;
    Evmoy_A = moy_A;
    Evmoy_AG = moy_AG;
    Evmoy_G = moy_G;
    // on passe le nombre d'espèce actuel dans le compteur, pour pouvoir faire l'évollution la prochaine fois
    compteurEsp1 = nb;

    //==========================
    // calcul des écarts-types :
    //==========================
    double somme_A = 0, somme_AD = 0, somme_AG = 0, somme_G = 0, somme_D = 0, inter = 0;
            for(unsigned int i = 0; i < tablEsp1.size(); i++)
            {
                inter = tablEsp1[i].getA() - moy_A;
                somme_A += pow(inter, 2); // pour chacune des valeurs, on ajoute à ecart_type (xi - moy*moy)
                inter = tablEsp1[i].getD() - moy_D;
                somme_D += pow(inter, 2); // pour chacune des valeurs, on ajoute à ecart_type (xi - moy*moy)
                inter = tablEsp1[i].getG() - moy_G;
                somme_G += pow(inter, 2); // pour chacune des valeurs, on ajoute à ecart_type (xi - moy*moy)
                inter = tablEsp1[i].getAG() - moy_AG;
                somme_AG += pow(inter, 2); // pour chacune des valeurs, on ajoute à ecart_type (xi - moy*moy)
                inter = tablEsp1[i].getAD() - moy_AD;
                somme_AD += pow(inter, 2); // pour chacune des valeurs, on ajoute à ecart_type (xi - moy*moy)
            }
            ecart_type_A = sqrt(somme_A/tablEsp1.size()); // et on prend la racine carrée de somme_A*(1/N) !
            ecart_type_AD = sqrt(somme_AD/tablEsp1.size()); // et on prend la racine carrée !
            ecart_type_AG = sqrt(somme_AG/tablEsp1.size()); // et on prend la racine carrée !
            ecart_type_D = sqrt(somme_D/tablEsp1.size()); // et on prend la racine carrée !
            ecart_type_G = sqrt(somme_G/tablEsp1.size()); // et on prend la racine carrée !


    // affichage de l'écart-type dans le fichier :
    stat << endl << "ECART-TYPES : " << ecart_type_D << "      " << ecart_type_AD << "      " << ecart_type_A << "      " << ecart_type_AG << " " << ecart_type_G << endl;


/*
    //=======================================
    // enregistrements pour étude graphique :
    //=======================================
    stat.close();
    ofstream etu(ETUDFILE, ios::app);
    // on stocke sous la forme PHASE:NB_ESP1:MOY_D:MOY_AD:MOY_A:MOY_GA:MOY_G:ECART_TYPE_D:ECART_TYPE_AD:ECART_TYPE_A:ECART_TYPE_GA:ECART_TYPE_G


    // écart-types
    etu << ":" << ecart_type_D << ":" << ecart_type_AD << ":" << ecart_type_A << ":" << ecart_type_AG << ":" << ecart_type_G << endl;

//*/
}









