#pragma once
#include "Artysta.h"
#include <string>
#include <iostream>

using namespace std;

class Zespol : public Artysta 
{
    private:
        int liczbaCzlonkow;
    public:
        Zespol(string nazwa = "Nieznany", int stawka = 0, int czlonkowie = 1);
        ~Zespol();
        
        void wyswietlWymagania() const override; 
};