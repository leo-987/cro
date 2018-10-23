#ifndef MOLECULE_H
#define MOLECULE_H
#include <iostream>

using namespace std;


class Molecule
{
    public:
        Molecule();
        Molecule(pair<float, float> pf);
        virtual ~Molecule();
        void update();
        double pe, ke, opt;
        int num_of_hits;
        pair<float, float> structure;
    protected:
    private:

};

#endif // MOLECULE_H
