#include <iostream>
#include <string>
#include "Solista.h"
#include "Zespol.h"
#include "Koncert.h"
#include "TrasaKoncertowa.h"

using namespace std;

int main()
{
    TrasaKoncertowa* agencjaTras[10];
    int liczbaTras = 0;
    int idAktywnej = -1;

    cout << "=== WITAJ W SYSTEMIE AGENCJI KONCERTOWEJ ===" << endl;
    cout << "Uwaga: nazwy miast przy dodawaniu koncertu podawaj po angielsku." << endl;

    int wybor;
    do
    {
        cout << "\n===== MENU MENADZERA =====" << endl;
        if (idAktywnej != -1)
        {
            cout << "-> Pracujesz nad trasa: " << agencjaTras[idAktywnej]->pobierzNazweTrasy() << endl;
        }
        else
        {
            cout << "-> Brak wybranej trasy! Najpierw utworz nowa." << endl;
        }

        cout << "1. Utworz NOWA trase i przypisz do niej GWIAZDE" << endl;
        cout << "2. Przelacz sie na inna trase" << endl;
        cout << "3. Zaplanuj nowy koncert (w obecnej trasie)" << endl;
        cout << "4. Wyswietl plan i WYCENE (obecnej trasy)" << endl;
        cout << "5. Sprzedaj bilety (w obecnej trasie)" << endl;
        cout << "6. Odwolaj koncert (w obecnej trasie)" << endl;
        cout << "7. Wyswietl raport wszystkich tras" << endl;
        cout << "8. Otworz graficzne OKNO MAPY tej trasy" << endl;
        cout << "0. Wyjdz z programu" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor)
        {
            case 1:
            {
                if (liczbaTras < 10)
                {
                    string nazwa, nazwaGwiazdy;
                    int typ, stawka;

                    cout << "\n--- NOWA TRASA ---" << endl;
                    cout << "Podaj nazwe nowej trasy (np. BTS ARIRANG WORLD TOUR): ";
                    getline(cin >> ws, nazwa);

                    cout << "Kto jest glowna gwiazda? (1 - Solista, 2 - Zespol): ";
                    cin >> typ;

                    cout << "Nazwa wykonawcy (np. BTS, Jennie): ";
                    getline(cin >> ws, nazwaGwiazdy);
                    cout << "Wycena artysty za koncert (stawka w EUR): ";
                    cin >> stawka;

                    Artysta* nowaGwiazda = nullptr;

                    if (typ == 1)
                    {
                        string wym;
                        cout << "Specjalne wymagania: ";
                        getline(cin >> ws, wym);
                        nowaGwiazda = new Solista(nazwaGwiazdy, stawka, wym);
                    }
                    else
                    {
                        int czlonkowie;
                        cout << "Ilu czlonkow liczy zespol? ";
                        cin >> czlonkowie;
                        nowaGwiazda = new Zespol(nazwaGwiazdy, stawka, czlonkowie);
                    }

                    agencjaTras[liczbaTras] = new TrasaKoncertowa(nazwa, nowaGwiazda);
                    idAktywnej = liczbaTras;
                    liczbaTras++;
                    cout << "Utworzono nowa trase z przypisana gwiazda!" << endl;
                }
                else
                {
                    cout << "Brak miejsca na kolejne trasy." << endl;
                }
                break;
            }
            case 2:
            {
                if (liczbaTras == 0)
                {
                    cout << "Brak utworzonych tras." << endl;
                }
                else
                {
                    cout << "Dostepne trasy:" << endl;
                    for (int i = 0; i < liczbaTras; i++)
                    {
                        cout << "[" << i + 1 << "] " << agencjaTras[i]->pobierzNazweTrasy() << endl;
                    }
                    int nr;
                    cout << "Wybierz numer trasy: ";
                    cin >> nr;
                    if (nr > 0 && nr <= liczbaTras)
                    {
                        idAktywnej = nr - 1;
                    }
                    else
                    {
                        cout << "Nieprawidlowy numer." << endl;
                    }
                }
                break;
            }
            case 3:
            {
                if (idAktywnej == -1)
                {
                    cout << "Wybierz lub utworz trase najpierw!" << endl;
                    break;
                }

                string miasto, stadion, data;
                int poj;

                cout << "\n--- LOKALIZACJA KONCERTU ---" << endl;
                cout << "Podaj nazwe miasta po angielsku (np. Seoul, Brussels, New York, Beijing): ";
                getline(cin >> ws, miasto);
                cout << "Podaj stadion (np. King Baudouin Stadium): ";
                getline(cin >> ws, stadion);
                cout << "Podaj date koncertu (np. 15.06.2026): ";
                getline(cin >> ws, data);
                cout << "Podaj pojemnosc stadionu: ";
                cin >> poj;

                Koncert* nowyKoncert = new Koncert(miasto, stadion, poj, data);
                agencjaTras[idAktywnej]->dodajKoncert(nowyKoncert);

                cout << "Dodano koncert! Gwiazda przypisana automatycznie." << endl;
                break;
            }
            case 4:
            {
                if (idAktywnej == -1)
                {
                    cout << "Wybierz lub utworz trase najpierw!" << endl;
                    break;
                }
                agencjaTras[idAktywnej]->podsumujTrase();
                cout << "\n>>> CALKOWITA WYCENA TRASY: " << agencjaTras[idAktywnej]->obliczBudzet() << " EUR <<<" << endl;
                break;
            }
            case 5:
            {
                if (idAktywnej == -1)
                {
                    cout << "Wybierz lub utworz trase najpierw!" << endl;
                    break;
                }
                if (agencjaTras[idAktywnej]->pobierzLiczbeKoncertow() == 0)
                {
                    cout << "Brak koncertow w trasie." << endl;
                }
                else
                {
                    int nr, ilosc;
                    cout << "Wybierz numer koncertu z listy: ";
                    cin >> nr;

                    Koncert* wybrany = agencjaTras[idAktywnej]->pobierzKoncert(nr);

                    if (wybrany != nullptr)
                    {
                        cout << "Ile biletow chcesz sprzedac? ";
                        cin >> ilosc;

                        try
                        {
                            wybrany->sprzedajBilety(ilosc);
                        }
                        catch (const char* blad)
                        {
                            cout << "Blad operacji: " << blad << endl;
                        }
                    }
                    else
                    {
                        cout << "Zly numer koncertu." << endl;
                    }
                }
                break;
            }
            case 6:
            {
                if (idAktywnej == -1)
                {
                    cout << "Wybierz lub utworz trase najpierw!" << endl;
                    break;
                }
                if (agencjaTras[idAktywnej]->pobierzLiczbeKoncertow() == 0)
                {
                    cout << "Brak koncertow do usuniecia." << endl;
                }
                else
                {
                    int nr;
                    cout << "Wybierz numer koncertu do odwolania: ";
                    cin >> nr;
                    agencjaTras[idAktywnej]->usunKoncert(nr);
                }
                break;
            }
            case 7:
            {
                if (liczbaTras == 0)
                {
                    cout << "Brak utworzonych tras w agencji!" << endl;
                }
                else
                {
                    cout << "\n===== RAPORT WSZYSTKICH TRAS =====" << endl;
                    int idNajdrozszej = 0;
                    for (int i = 0; i < liczbaTras; i++)
                    {
                        agencjaTras[i]->podsumujTrase();
                        cout << "\n>>> BUDZET TEJ TRASY: " << agencjaTras[i]->obliczBudzet() << " EUR <<<\n" << endl;
                        if (*agencjaTras[idNajdrozszej] < *agencjaTras[i])
                        {
                            idNajdrozszej = i;
                        }
                    }
                    if (liczbaTras > 1)
                    {
                        cout << ">>> NAJDROZSZA TRASA W AGENCJI: " << agencjaTras[idNajdrozszej]->pobierzNazweTrasy()
                             << " (" << agencjaTras[idNajdrozszej]->obliczBudzet() << " EUR) <<<" << endl;
                    }
                }
                break;
            }
            case 8:
            {
                try
                {
                    if (liczbaTras == 0 || idAktywnej == -1)
                    {
                        throw "BLAD: Nie utworzono jeszcze zadnej trasy! Wybierz najpierw opcje 1.";
                    }
                    agencjaTras[idAktywnej]->otworzOknoMapy();
                }
                catch (const char* blad)
                {
                    cout << blad << endl;
                }
                break;
            }
            case 0:
                cout << "Zamykanie systemu..." << endl;
                break;
            default:
                cout << "Nieznana opcja." << endl;
        }
    } while (wybor != 0);

    for (int i = 0; i < liczbaTras; i++)
    {
        delete agencjaTras[i];
    }

    return 0;
}