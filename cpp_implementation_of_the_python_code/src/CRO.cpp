#include "CRO.h"
#include "Molecule.h"
#include <bits/stdc++.h>

#define PI 3.14

using namespace std;


int cnt = 0;
double limit = 5000, KELossRate = 0.2, mole_coll = 0.2;
double alpha = 100, beta = 100, buffer = 0, init_ke = 100;
vector<Molecule> pop;

CRO::CRO(vector<pair<int,int>>& p)
{
    int is_first_molecule = 0;
    for(vector<pair<int,int>>::iterator it = p.begin(); it != p.end(); ++it)
    {
            Molecule m(*it);
            pop.push_back(m);
    }

    for(vector<Molecule>::iterator it = pop.begin(); it != pop.end(); ++it)
    {
        it->pe = this->fit_func(*it);
        it->ke = 100;
        it->update();
        if(!is_first_molecule)
        {
            optimal = *it;
        }
        else if(it->pe < optimal.pe)
        {
            optimal = *it;
        }

    }

}

CRO::~CRO()
{
    //dtor
}

float CRO::random1()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void CRO::run()
{

    Molecule m1, m2;
    while(cnt < limit)
    {
        cnt += 1;
        if(random1() > mole_coll or pop.size() < 2)
        {
            Molecule m = pop[rand()%pop.size()];
            if(m.num_of_hits > alpha)
                decomposition(m);
            else
                on_wall(m);
        }else
        {
             m1 = pop[rand()%pop.size()];
             m2 = pop[rand()%pop.size()];

            if(m1.ke <= beta and m2.ke <= beta)
                synthesis(m1, m2);
            else
                interaction(m1, m2);


        }


    }

}

void CRO::update(Molecule m)
{
    if(m.pe < optimal.pe)
        optimal = m;


}

void CRO::decomposition(Molecule m)
{
    m.num_of_hits += 1;
    Molecule new1 , new2 ;
    pair<Molecule, Molecule> pm;
    pm = dec(m);
    new1 = pm.first;
    new2 = pm.second;


    new1.pe = fit_func(new1);
    new2.pe = fit_func(new2);
    double tmp = m.pe + m.ke - new1.pe - new2.pe;
    if(tmp >= 0 or tmp + buffer >= 0)
    {
        if(tmp >= 0)
        {
            double q = random1();
            new1.ke = tmp * q;
            new2.ke = tmp * (1 - q);
        }else
        {
            new1.ke = (tmp + buffer) * random1() * random1();
            new2.ke = (tmp + buffer - new1.ke) * random1() * random1();
            buffer = buffer + tmp - new1.ke - new2.ke ;
        }
            new1.update();
            new2.update();
            //iterate two same tree for equality check
            erase_molecule(pop, m);
            pop.push_back(new1);
            pop.push_back(new2);
            update(new1);
            update(new2);

    }


}

void CRO::on_wall(Molecule m)
{
    m.num_of_hits += 1;
    Molecule m1 = wall(m);
    m1.pe = fit_func(m1);
    double tmp = m.pe + m.ke - m1.pe;
    if(tmp >= 0)
    {
        //double q = random.uniform in range KELossRate to 1;
        double q = KELossRate + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(1-KELossRate)));

        m1.ke = tmp * q;
        m1.update();
        buffer += tmp * (1 - q);
        erase_molecule(pop, m);
        pop.push_back(m1);
        update(m1);
    }

}

void CRO::interaction(Molecule m1, Molecule m2)
{
    m1.num_of_hits += 1;
    m2.num_of_hits += 1;


    pair<Molecule, Molecule> pm;
    pm = inter(m1, m2);
    Molecule new1, new2;
    new1 = pm.first;
    new2 = pm.second;
    new1.pe = fit_func(new1);
    new2.pe = fit_func(new2);
    double tmp = m1.pe + m2.pe + m1.ke + m2.ke - new1.pe - new2.pe;

    if(tmp >= 0)
    {
        double q = rand();
        new1.ke = tmp * q;
        new2.ke = tmp * (1 - q);
        new1.update();
        new2.update();
        erase_molecule(pop, m1);
        erase_molecule(pop, m2);
        pop.push_back(new1);
        pop.push_back(new2);
        update(new1);
        update(new2);
    }

}

void CRO::erase_molecule(vector<Molecule>& population, Molecule& m)
{
    for(vector<Molecule>::iterator it = population.begin(); it != population.end(); it++)
    {
        if((it->structure.first == m.structure.first) && (it->structure.second == m.structure.second))
        {
            population.erase(it);
            break;
        }
    }

}


void CRO::synthesis(Molecule m1, Molecule m2)
{
    m1.num_of_hits += 1;
    m2.num_of_hits += 1;

    Molecule new1 = syn(m1, m2);
    new1.pe = fit_func(new1);
    double tmp = m1.pe + m2.pe + m1.ke + m2.ke - new1.pe;

    if(tmp >= 0)
    {
        new1.ke = tmp;
        new1.update();

        erase_molecule(pop, m1);
        erase_molecule(pop, m2);

        pop.push_back(new1);
        update(new1);
    }
}

Molecule CRO::syn(Molecule m1, Molecule m2)
{
        Molecule new1  = m1;
        if (random1() < 0.5){
            new1.structure.first = m2.structure.first;
        }else{
            new1.structure.second = m2.structure.second;
        }

        return new1;

}

pair<Molecule, Molecule> CRO::inter(Molecule m1, Molecule m2)
{
        Molecule new1 = m1;
        Molecule new2 = m2;
        new1.structure.first = m2.structure.first;
        new1.structure.second = m1.structure.second;
        new2.structure.first = m1.structure.first;
        new2.structure.second = m2.structure.second;
        pair<Molecule, Molecule> pm;
        pm = make_pair(new1, new2);
        return pm;
}

Molecule CRO::wall(Molecule m)
{
    Molecule new1 = m;

    swap(new1.structure.first, new1.structure.second);
    return new1;
}


pair<Molecule,Molecule> CRO::dec(Molecule m)
{
        pair<Molecule, Molecule> pm;
        Molecule new1 = m;
        Molecule new2 = m;
        new1.structure.first += random();
        new2.structure.second += random();
        pm = make_pair(new1, new2);
        return pm;
}

float CRO::fit_func(Molecule m){

    //return sin(m.structure.first*PI /180.0) + cos(m.structure.second*PI / 180.0);
    return tan(m.structure.first*PI /180.0);
}


