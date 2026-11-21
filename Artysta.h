#pragma once
#include <iostream>
#include <string>
using namespace std;

class Artysta
{
    protected:
        string nazwaArtysty;
        int stawkaZaKoncert;
    public:
        Artysta(string nazwa="Nieznany",int stawka=0);
        virtual ~Artysta();
        string pobierzNazwe() const;
        void ustawNazwe(string& nowaNazwa);
        int pobierzStawke() const;
        void ustawStawke(int nowaStawka);

        virtual void wyswietlWymagania() const=0;

};