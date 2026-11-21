#pragma once
#include "Artysta.h"
#include <string>
#include <iostream>

using namespace std;

class Solista : public Artysta 
{
    private:
        string specjalneWymaganie;
    public:
        Solista(string nazwa = "Nieznany", int stawka = 0, string wymaganie = "Brak");
        ~Solista();
        
        void wyswietlWymagania() const override; 
};