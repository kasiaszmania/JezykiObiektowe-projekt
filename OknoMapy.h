#pragma once
#include <string>
#include "Koncert.h"

using namespace std;

class OknoMapy
{
    private:
        string nazwaTrasy;
        string nazwaArtysty;
        Koncert** koncerty;
        int liczbaKoncertow;
        int budzet;

    public:
        OknoMapy(string trasa, string artysta, Koncert** tablica, int liczba, int budz);
        void pokaz();
};