#ifndef CRO_H
#define CRO_H
#include "Molecule.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class CRO
{
    public:
        CRO(vector<pair<int,int>>& p);
        virtual ~CRO();
        float random1();
        void run();
        void update(Molecule m);
        void decomposition(Molecule m);
        void on_wall(Molecule m);
        void interaction(Molecule m1, Molecule m2);
        void synthesis(Molecule m1, Molecule m2);
        Molecule syn(Molecule m1, Molecule m2);
        pair<Molecule, Molecule> inter(Molecule m1, Molecule m2);
        Molecule wall(Molecule m);
        pair<Molecule,Molecule> dec(Molecule m);
        float fit_func(Molecule m);
        void erase_molecule(vector<Molecule>& population, Molecule& m);

        Molecule optimal;
    protected:
    private:
};

#endif // CRO_H
