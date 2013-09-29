#ifndef ESPECE1_H_INCLUDED
#define ESPECE1_H_INCLUDED


//*
// objet Espece1
class Espece1 : public element
{
    public: // PUBLIC
        // constructeurs : pr�d�termin� ou tr�s dirig�s, avec beaucoup d'options en pr�d�finit (il suffit de ne rien mettre, et de d�passer 100 pour les coordon�es pour les avoir en al�atoire)
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

            // accesseurs indiquant/modifiant la capacit� de se rzeproduire, modifi�e lors des calculs
            int getCapaciteReproduction();
            void setCapaciteReproduction(bool estCapable = true);


    private: // PRIVATE
        // fonctions priv�es, car inutiles pour l'utilisateur
        void deplacement();
        void ActionSurCase();
        void GenererProbMvtAleat();
        int DirectionRelativeAleatoire(int D = 20, int AD = 20, int A = 20, int AG = 20, int G = 20); // donne une direction al�atoire, selon probabilit�s donn�es en arg
        int DirectionAbsolueAleatoire();
        void VerifMortaliteAgeOuReserve();

        // attributs de Espece1
        // probabilit�s de mouvement
        int e_pA;
        int e_pD;
        int e_pG;
        int e_pAG;
        int e_pAD;
        int e_Vitesse;

        // gestion de nourriture
        int e_reserveNour;

        // vie g�n�rale (age, repro)
        int e_age;
        bool e_CapableReproduction;
        unsigned int e_ID;

        // coordonn�es
        int e_DerniereDirection; // derni�re direction en ABSOLU
};

//*/


#endif // ESPECE1_H_INCLUDED
