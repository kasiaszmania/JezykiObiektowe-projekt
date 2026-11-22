#pragma once
#include <iostream>
#include "Artysta.h"
#include <string>

using namespace std;

class Koncert
{
    private:
        string miasto;
        string nazwaStadionu;
        string data;
        int pojemnosc;
        int sprzedaneBilety;
        Artysta* glownaGwiazda;
    public:
        Koncert(string m = "Nieznane", string s = "Brak", int poj = 0, string d = "TBA");
        ~Koncert();

        string pobierzMiasto() const;
        string pobierzStadion() const;
        string pobierzDate() const;
        int pobierzPojemnosc() const;
        int pobierzSprzedane() const;
        int pobierzKoszt() const;

        void ustawGwiazde(Artysta* artysta);
        void sprzedajBilety(int ilosc);
        void wyswietlSzczegoly() const;

        friend ostream& operator<<(ostream& os, const Koncert& k);
};