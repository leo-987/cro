#include "Molecule.h"
//#incldue <iostream>


Molecule::Molecule()
{

}
Molecule::Molecule(pair<float, float> pf)
{
   this->pe = 0;
   this->ke = 0;
   this->opt = 999999;
   this->num_of_hits = 0;
   //copy the structure
   this->structure = pf;
}

Molecule::~Molecule()
{
    //dtor
}

void Molecule::update()
{
    if(pe < opt)
    {
        opt = pe;
        num_of_hits = 0;
    }
}
