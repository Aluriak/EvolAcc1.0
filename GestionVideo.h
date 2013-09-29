#ifndef GESTIONVIDEO_H_INCLUDED
#define GESTIONVIDEO_H_INCLUDED

class GestionVideo
{
    public:
        GestionVideo(int rapportNourritureBasique = RAPPORT_NOURBASE);
        static void ColorerSurface_SDL(SDL_Surface *ecran, int x = 0, int y = 0, int largeur = TAILLE_X, int longueur = TAILLE_Y, int Red = 0, int Green = 0, int Blue = 0); // colore la zone donn�e avec la couleur donn�e
        void RemplissageEcranJEVL_SDL(SDL_Surface *ecran, bool RenseignerFichier = false);
        std::vector<Espece1> getVecteur();


    private:
        // fonctions internes
            // fonctions g�n�riques
            void InitialisationJEVL(); // lecture du SAVEFILE et cr�ation des objets pertinents
            void AffichageJEVL_SDL(); // affichage de l'univers selon conditions initiales, puis traitement des changement de phase
            void pause(SDL_Surface *ecran); // pause, gestion des touches, des fonctions de changement de phase.
            void PassagePhaseVecteur(); // g�re le passage de pahse de tous les composants du vector tablEsp1
            Espece1 EnfantEsp1(Espece1 *pere, Espece1 *mere); // cr�� un enfant a partir des parents envoy�s en arguments.
            void AjoutComposantesNourritureJEVL(); // ajoute la nourriture
                void GestionCapaciteRepro_Esp1_JEVL(); // d�finis la capacit� d'une Espece1 � se reproduire
                void GestionApparition_NourBase_JEVL(); // d�fini d'�ventuels ajouts de nourriture
            int NombreAleatoireDefini_JEVL(std::string var); //utilis� lors de l'initialisation. Renvois un remplacant � "?"
            bool VerifViabiliteVariabiliteGenetique(int D, int AD, int A, int AG, int G, int var); // renvois vrai si on peut all�grement modifier les prob  en arg avec le taux de variabilit� g�n�tique.
            // fonctions relatives au sondage des cases
            int NourriturePresente(int x, int y); // renvois l'ID de la bouffe trouv�e, ou -1 si pas trouv�e
            std::vector<int> Espece1ReproductivePresente(int x, int y); // renvois un vector contenant les ID des Espece1 capable de se repoduire trouv�e en (x,y)

            // fonctions relatives aux stats
            void GenererStats(); // principale, appel�e par la touche "s", par pause
        // attributs
        std::vector<Espece1> tablEsp1; // vector contenant toutes les bestioles Espece1
            int nbEsp1Cree; // utilis� pour d�terminer l'ID des nouvelles Espece1
        std::vector<NourBase> tablNourBase; // vector contenant toutes les bases de nourrriture
            int nbNourBaseCree; // utilis� pour d�terminer l'ID des nouvelles NourBase

        int e_rapport_NourBase;
        int nbPhasePassee;
};

#endif // GESTIONVIDEO_H_INCLUDED
