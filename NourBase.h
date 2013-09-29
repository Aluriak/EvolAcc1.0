#ifndef NOURBASE_H_INCLUDED
#define NOURBASE_H_INCLUDED

class NourBase : public element
{
    public:
        NourBase(int ID, int x, int y, int ValNour);
            int getGainNour();
            int getID();

    private:
        int e_GainNour;
        unsigned int e_ID;
};

#endif // NOURBASE_H_INCLUDED
