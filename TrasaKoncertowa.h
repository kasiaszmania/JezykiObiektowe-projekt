#pragma once
#include "Koncert.h"
#include "Artysta.h"
#include <iostream>
#include <string>

using namespace std;

class TrasaKoncertowa
{
    private:
        string nazwaTrasy;
        Artysta* gwiazdaTrasy;
        Koncert** tablicaKoncertow;
        int liczbaKoncertow;
        int pojemnoscTablicy;

        void powiekszTablice();

    public:
        TrasaKoncertowa(string nazwa = "Nowa trasa", Artysta* gwiazda = nullptr);
        ~TrasaKoncertowa();

        string pobierzNazweTrasy() const;

        void dodajKoncert(Koncert* nowyKoncert);
        void usunKoncert(int numer);
        Koncert* pobierzKoncert(int numer);
        int pobierzLiczbeKoncertow() const;
        int obliczBudzet() const;
        void podsumujTrase() const;
        void otworzOknoMapy();
       

        bool operator<(const TrasaKoncertowa& inna) const;
};