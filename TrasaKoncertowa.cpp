#include "TrasaKoncertowa.h"
#include "OknoMapy.h"



TrasaKoncertowa::TrasaKoncertowa(string nazwa, Artysta* gwiazda)
{
    nazwaTrasy = nazwa;
    gwiazdaTrasy = gwiazda;
    pojemnoscTablicy = 2;
    liczbaKoncertow = 0;
    tablicaKoncertow = new Koncert*[pojemnoscTablicy];
}

TrasaKoncertowa::~TrasaKoncertowa()
{
    for (int i = 0; i < liczbaKoncertow; i++)
    {
        delete tablicaKoncertow[i];
    }
    delete[] tablicaKoncertow;
    delete gwiazdaTrasy;
}

string TrasaKoncertowa::pobierzNazweTrasy() const
{
    return nazwaTrasy;
}

void TrasaKoncertowa::powiekszTablice()
{
    pojemnoscTablicy *= 2;
    Koncert** nowaTablica = new Koncert*[pojemnoscTablicy];

    for (int i = 0; i < liczbaKoncertow; i++)
    {
        nowaTablica[i] = tablicaKoncertow[i];
    }

    delete[] tablicaKoncertow;
    tablicaKoncertow = nowaTablica;
}

void TrasaKoncertowa::dodajKoncert(Koncert* nowyKoncert)
{
    if (liczbaKoncertow >= pojemnoscTablicy)
    {
        powiekszTablice();
    }
    nowyKoncert->ustawGwiazde(gwiazdaTrasy);
    tablicaKoncertow[liczbaKoncertow] = nowyKoncert;
    liczbaKoncertow++;
}

void TrasaKoncertowa::usunKoncert(int numer)
{
    int indeks = numer - 1;
    if (indeks >= 0 && indeks < liczbaKoncertow)
    {
        delete tablicaKoncertow[indeks];
        for (int i = indeks; i < liczbaKoncertow - 1; i++)
        {
            tablicaKoncertow[i] = tablicaKoncertow[i + 1];
        }
        liczbaKoncertow--;
        cout << "Koncert zostal odwolany i usuniety z trasy." << endl;
    }
    else
    {
        cout << "Nieprawidlowy numer koncertu." << endl;
    }
}

Koncert* TrasaKoncertowa::pobierzKoncert(int numer)
{
    int indeks = numer - 1;
    if (indeks >= 0 && indeks < liczbaKoncertow)
    {
        return tablicaKoncertow[indeks];
    }
    return nullptr;
}

int TrasaKoncertowa::pobierzLiczbeKoncertow() const
{
    return liczbaKoncertow;
}

int TrasaKoncertowa::obliczBudzet() const
{
    int suma = 0;
    for (int i = 0; i < liczbaKoncertow; i++)
    {
        suma += tablicaKoncertow[i]->pobierzKoszt();
    }
    return suma;
}

void TrasaKoncertowa::podsumujTrase() const
{
    cout << "\n=========================================" << endl;
    cout << " TRASA KONCERTOWA: " << nazwaTrasy << endl;
    cout << " Liczba zaplanowanych koncertow: " << liczbaKoncertow << endl;
    cout << "=========================================" << endl;

    for (int i = 0; i < liczbaKoncertow; i++)
    {
        cout << "\n[" << i + 1 << "] " << *(tablicaKoncertow[i]) << endl;
        tablicaKoncertow[i]->wyswietlSzczegoly();
    }
}



bool TrasaKoncertowa::operator<(const TrasaKoncertowa& inna) const
{
    return obliczBudzet() < inna.obliczBudzet();
}
void TrasaKoncertowa::otworzOknoMapy()
{
    if (liczbaKoncertow == 0)
    {
        throw "BLAD: Ta trasa nie ma jeszcze zadnych koncertow! Dodaj najpierw koncert (opcja 3).";
    }
    string nazwaGwiazdy = (gwiazdaTrasy != nullptr) ? gwiazdaTrasy->pobierzNazwe() : "Brak";
    cout << "Otwieranie okna graficznego dla trasy: " << nazwaTrasy << "..." << endl;
    OknoMapy okno(nazwaTrasy, nazwaGwiazdy, tablicaKoncertow, liczbaKoncertow, obliczBudzet());
    okno.pokaz();
}