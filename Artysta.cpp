#include <iostream>
#include "Artysta.h"

using namespace std;

Artysta::Artysta(string nazwa, int stawka)
{
    nazwaArtysty = nazwa;
    stawkaZaKoncert = stawka;
}

Artysta::~Artysta() {}

string Artysta::pobierzNazwe() const
{
    return nazwaArtysty;
}

void Artysta::ustawNazwe(string& nowaNazwa)
{
    nazwaArtysty = nowaNazwa;
}

int Artysta::pobierzStawke() const
{
    return stawkaZaKoncert;
}

void Artysta::ustawStawke(int nowaStawka)
{
    stawkaZaKoncert = nowaStawka;
}