#include "Solista.h"
#include <iostream>

using namespace std;

Solista::Solista(string nazwa, int stawka, string wymaganie) : Artysta(nazwa, stawka) 
{
    specjalneWymaganie = wymaganie;
}

Solista::~Solista() {}

void Solista::wyswietlWymagania() const 
{
    cout << "SOLISTA: " << nazwaArtysty << " | Stawka: " << stawkaZaKoncert << " EUR" << endl;
    cout << "Wymagania: " << specjalneWymaganie << endl;
}