#include "Zespol.h"
#include <iostream>

using namespace std;

Zespol::Zespol(string nazwa, int stawka, int czlonkowie) : Artysta(nazwa, stawka) 
{
    liczbaCzlonkow = czlonkowie;
}

Zespol::~Zespol() {}

void Zespol::wyswietlWymagania() const 
{
    cout << "ZESPOL: " << nazwaArtysty << " | Stawka: " << stawkaZaKoncert << " EUR" << endl;
    cout << "Liczba czlonkow na scenie: " << liczbaCzlonkow << " osob" << endl;
}