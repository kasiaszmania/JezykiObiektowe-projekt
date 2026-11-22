#include "Koncert.h"
#include <iostream>

using namespace std;

Koncert::Koncert(string m, string s, int poj, string d)
{
    miasto = m;
    nazwaStadionu = s;
    pojemnosc = poj;
    data = d;
    sprzedaneBilety = 0;
    glownaGwiazda = nullptr;
}

Koncert::~Koncert() {}

string Koncert::pobierzMiasto() const
{
    return miasto;
}

string Koncert::pobierzStadion() const
{
    return nazwaStadionu;
}

string Koncert::pobierzDate() const
{
    return data;
}

int Koncert::pobierzPojemnosc() const
{
    return pojemnosc;
}

int Koncert::pobierzSprzedane() const
{
    return sprzedaneBilety;
}

int Koncert::pobierzKoszt() const
{
    if (glownaGwiazda != nullptr)
    {
        return glownaGwiazda->pobierzStawke();
    }
    return 0;
}

void Koncert::ustawGwiazde(Artysta* artysta)
{
    glownaGwiazda = artysta;
}

void Koncert::sprzedajBilety(int ilosc)
{
    if (sprzedaneBilety + ilosc > pojemnosc)
    {
        throw "BLAD: Brak miejsc na stadionie!";
    }
    sprzedaneBilety += ilosc;
    cout << "Sprzedano " << ilosc << " biletow." << endl;
}

void Koncert::wyswietlSzczegoly() const
{
    if (glownaGwiazda != nullptr)
    {
        cout << "Na scenie wystapi:\n";
        glownaGwiazda->wyswietlWymagania();
    }
    else
    {
        cout << "Brak przypisanego artysty." << endl;
    }
}

ostream& operator<<(ostream& os, const Koncert& k)
{
    os << k.miasto << " (" << k.nazwaStadionu << ") | Data: " << k.data
       << " | Zajete miejsca: " << k.sprzedaneBilety << " / " << k.pojemnosc;
    return os;
}