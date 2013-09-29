#ifndef ESPECE1_H_INCLUDED
#define ESPECE1_H_INCLUDED


//*
// objet Espece1
class Espece1 : public element
{
    public: // PUBLIC
        // constructeurs : prédéterminé ou très dirigés, avec beaucoup d'options en prédéfinit (il suffit de ne rien mettre, et de dépasser 100 pour les coordonées pour les avoir en aléatoire)
        Espece1(int ID, int x, int y);
        Espece1(int ID, int x, int y, int D = 20, int AD = 20, int A = 20, int AG = 20, int G = 20, int reserve = 10, int age = 0, int direcPrecedente = AVANT);
        void PassagePhaseSuivante();
        // accesseurs
            int getID();
            int getA();
            int getAD();
            int getAG();
            int getD();
            int getG();
            int getAge();
            int getDirecPrec();
            int getReserveNour();
                void GainNourriture(int gain = 1);

            // accesseurs indiquant/modifiant la capacité de se rzeproduire, modifiée lors des calculs
            int getCapaciteReproduction();
            void setCapaciteReproduction(bool estCapable = true);


    private: // PRIVATE
        // fonctions privées, car inutiles pour l'utilisateur
        void deplacement();
        void ActionSurCase();
        void GenererProbMvtAleat();
        int DirectionRelativeAleatoire(int D = 20, int AD = 20, int A = 20, int AG = 20, int G = 20); // donne une direction aléatoire, selon probabilités données en arg
        int DirectionAbsolueAleatoire();
        void VerifMortaliteAgeOuReserve();

        // attributs de Espece1
        // probabilités de mouvement
        int e_pA;
        int e_pD;
        int e_pG;
        int e_pAG;
        int e_pAD;
        int e_Vitesse;

        // gestion de nourriture
        int e_reserveNour;

        // vie générale (age, repro)
        int e_age;
        bool e_CapableReproduction;
        unsigned int e_ID;

        // coordonnées
        int e_DerniereDirection; // dernière direction en ABSOLU
};

//*/


#endif // ESPECE1_H_INCLUDED
