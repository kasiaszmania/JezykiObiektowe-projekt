# Menedżer Trasy Koncertowej (Symulator Agencji)

Projekt to aplikacja w C++, która symuluje działanie agencji koncertowej. Pozwala wcielić się w menedżera – tworzyć trasy dla różnych wykonawców, organizować koncerty na konkretnych stadionach, sprzedawać bilety, na bieżąco zarządzać budżetem, a na koniec podejrzeć całą trasę na interaktywnej mapie świata w oknie graficznym `raylib`.

## Jak to jest zbudowane?

Projekt jest podzielony na osobne pliki nagłówkowe (`.h`) i źródłowe (`.cpp`) i opiera się na 6 klasach, które ze sobą współpracują:

- **Artysta, Solista i Zespol (Dziedziczenie i polimorfizm)**
  Zamiast jednego „ogólnego” artysty stworzyłam klasę abstrakcyjną `Artysta` z polami chronionymi (`nazwaArtysty`, `stawkaZaKoncert`), wirtualnym destruktorem oraz czysto wirtualną metodą `wyswietlWymagania()`. Dziedziczą po niej dwie konkretne klasy: `Solista` (który ma swoje specjalne wymagania) oraz `Zespol` (który ma określoną liczbę członków). Dzięki polimorfizmowi i wywołaniom przez wskaźnik `Artysta*` program sam wie, jak wyświetlić dane dla konkretnego typu gwiazdy.

- **Koncert (Przeciążanie operatorów i wyjątki)**
  Klasa reprezentująca pojedyncze wydarzenie. Przechowuje miasto, nazwę stadionu, datę, pojemność, liczbę sprzedanych biletów i wskaźnik na przypisanego artystę.
  Dodałam tutaj własne przeciążenie operatora `<<`, dzięki czemu wypisywanie informacji o koncercie w konsoli jest bardzo proste i czytelne. Zabezpieczyłam też sprzedaż biletów – jeśli ktoś spróbuje sprzedać więcej biletów, niż wynosi pojemność stadionu, program rzuca wyjątek (`throw`), a blok `try`/`catch` informuje o braku miejsc, nie pozwalając na błąd w obliczeniach.

- **TrasaKoncertowa (Zarządzanie pamięcią, tablice dynamiczne i operator `<`)**
  Główny silnik trasy. Zarządza wszystkimi koncertami danego artysty przy pomocy dynamicznej tablicy wskaźników (`Koncert**`). Tablica sama się powiększa (alokując nową pamięć za pomocą `new`), gdy brakuje miejsca na kolejne miasta, a także pozwala na usuwanie odwołanych koncertów z przesuwaniem pozostałych elementów. W destruktorze wszystko jest czyszczone za pomocą `delete` i `delete[]`, aby nie było wycieków pamięci. Dodałam tu również przeciążenie operatora `<`, które porównuje budżety tras i pozwala w raporcie agencji wskazać najdroższą z nich.

- **OknoMapy (Wizualizacja w `raylib` i współrzędne miast)**
  Klasa odpowiadająca za graficzny podgląd trasy. Na początku może wyświetlić plakat trasy (np. `bts.png` lub `jennie.png`), a po przejściu na mapę rysuje mapę świata (`mapa.png`), łączy miasta liniami i animuje przelot samolotu. Współrzędne miast są pobierane przez PowerShella z Open-Meteo Geocoding API (w razie braku internetu program korzysta ze współrzędnych zapasowych). W oknie mapy można pauzować i restartować lot, klikać konkretne przystanki oraz szybko sprzedawać bilety przyciskiem `+1000 BILETOW`.

## Schemat klas UML

```mermaid
classDiagram
    class Artysta {
        <<abstract>>
        #nazwaArtysty : string
        #stawkaZaKoncert : int
        +Artysta(nazwa : string, stawka : int)
        +~Artysta()
        +pobierzNazwe() string
        +ustawNazwe(nowaNazwa : string&) void
        +pobierzStawke() int
        +ustawStawke(nowaStawka : int) void
        +wyswietlWymagania()* void
    }

    class Solista {
        -specjalneWymaganie : string
        +Solista(nazwa : string, stawka : int, wymaganie : string)
        +~Solista()
        +wyswietlWymagania() void
    }

    class Zespol {
        -liczbaCzlonkow : int
        +Zespol(nazwa : string, stawka : int, czlonkowie : int)
        +~Zespol()
        +wyswietlWymagania() void
    }

    class Koncert {
        -miasto : string
        -nazwaStadionu : string
        -data : string
        -pojemnosc : int
        -sprzedaneBilety : int
        -glownaGwiazda : Artysta*
        +Koncert(m : string, s : string, poj : int, d : string)
        +~Koncert()
        +pobierzMiasto() string
        +pobierzStadion() string
        +pobierzDate() string
        +pobierzPojemnosc() int
        +pobierzSprzedane() int
        +pobierzKoszt() int
        +ustawGwiazde(artysta : Artysta*) void
        +sprzedajBilety(ilosc : int) void
        +wyswietlSzczegoly() void
        +operator<<(os : ostream&, k : const Koncert&) ostream&
    }

    class TrasaKoncertowa {
        -nazwaTrasy : string
        -gwiazdaTrasy : Artysta*
        -tablicaKoncertow : Koncert**
        -liczbaKoncertow : int
        -pojemnoscTablicy : int
        -powiekszTablice() void
        +TrasaKoncertowa(nazwa : string, gwiazda : Artysta*)
        +~TrasaKoncertowa()
        +pobierzNazweTrasy() string
        +dodajKoncert(nowyKoncert : Koncert*) void
        +usunKoncert(numer : int) void
        +pobierzKoncert(numer : int) Koncert*
        +pobierzLiczbeKoncertow() int
        +obliczBudzet() int
        +podsumujTrase() void
        +otworzOknoMapy() void
        +operator<(inna : const TrasaKoncertowa&) bool
    }

    class OknoMapy {
        -nazwaTrasy : string
        -nazwaArtysty : string
        -koncerty : Koncert**
        -liczbaKoncertow : int
        -budzet : int
        +OknoMapy(trasa : string, artysta : string, tablica : Koncert**, liczba : int, budz : int)
        +pokaz() void
    }

    Artysta <|-- Solista : dziedziczenie
    Artysta <|-- Zespol : dziedziczenie
    TrasaKoncertowa *-- Koncert : kompozycja
    TrasaKoncertowa *-- Artysta : kompozycja
    Koncert o-- Artysta : agregacja
    TrasaKoncertowa ..> OknoMapy : zależność
    OknoMapy o-- Koncert : agregacja
```

## Obsługa programu

Całość obsługuje się przez interaktywne menu w funkcji `main` (pętla `do-while` ze `switch`). Możesz utworzyć nową trasę i przypisać do niej głównego artystę, przełączać się między trasami, dodawać miasta (najlepiej wpisywać je po angielsku, np. `Warsaw`, `London`, `Tokyo`, `New York`), sprzedawać bilety, odwoływać koncerty, wygenerować raport finansowy dla całej agencji ze wskazaniem najdroższej trasy oraz otworzyć okno mapy.

**Wymagania, kompilacja i uruchomienie:**
Do uruchomienia programu potrzebny jest kompilator `g++` (np. MSYS2/MinGW-w64) z biblioteką `raylib` oraz pliki graficzne `mapa.png`, `bts.png` i `jennie.png` w katalogu roboczym. Pobieranie współrzędnych miast korzysta z Open-Meteo API przez Windows PowerShell (wymagany dostęp do internetu).

```bash
g++ main.cpp Artysta.cpp Solista.cpp Zespol.cpp Koncert.cpp TrasaKoncertowa.cpp OknoMapy.cpp -o program.exe -lraylib -lgdi32 -lwinmm
.\program.exe
```