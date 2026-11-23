#include "OknoMapy.h"
#include "raylib.h"
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <vector>
#include <cctype>

struct Punkt
{
    float x;
    float y;
    string miasto;
};

static string naMale(string s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

static string zakodujUrl(string s)
{
    string out = "";
    for (int i = 0; i < (int)s.size(); i++)
    {
        unsigned char c = (unsigned char)s[i];
        if (isalnum(c) || c == '-' || c == '_') out += (char)c;
        else if (c == ' ') out += "%20";
    }
    return out;
}

static bool pobierzZApi(string miasto, float& lat, float& lon)
{
    string q = zakodujUrl(miasto);
    if (q.empty()) return false;

    string cmd = "powershell -NoProfile -Command \"(Invoke-RestMethod 'https://geocoding-api.open-meteo.com/v1/search?name=" + q +
                 "&count=1' -TimeoutSec 4).results | Select-Object -First 1 | ForEach-Object { $_.latitude.ToString([cultureinfo]::InvariantCulture); $_.longitude.ToString([cultureinfo]::InvariantCulture) }\" > miasto_coords.txt 2>nul";
    system(cmd.c_str());

    ifstream plik("miasto_coords.txt");
    float a = 0.0f, b = 0.0f;
    bool ok = plik.is_open() && (bool)(plik >> a >> b);
    if (plik.is_open()) plik.close();
    remove("miasto_coords.txt");

    if (ok)
    {
        lat = a;
        lon = b;
        return true;
    }
    return false;
}

static void znajdzPozycje(string miasto, int nr, float& x, float& y)
{
    float lat = 0.0f;
    float lon = 0.0f;

    if (!pobierzZApi(miasto, lat, lon))
    {
        int suma = 0;
        for (int i = 0; i < (int)miasto.size(); i++)
        {
            suma += (unsigned char)miasto[i] * (i + 1);
        }
        lat = -20.0f + ((suma + nr * 19) % 70);
        lon = -130.0f + ((suma * 17 + nr * 41) % 270);
    }

    x = 270.0f + (lon + 180.0f) / 370.5f * 830.0f;
    y = 90.0f + (83.6f - lat) / 173.6f * 475.0f;
}

static void rysujZdjecie(Texture2D tex, Rectangle dst)
{
    float skala = (float)tex.width / (float)tex.height;
    float cel = dst.width / dst.height;
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    if (skala > cel)
    {
        src.width = tex.height * cel;
        src.x = (tex.width - src.width) / 2.0f;
    }
    else
    {
        src.height = tex.width / cel;
        src.y = (tex.height - src.height) / 2.0f;
    }
    DrawTexturePro(tex, src, dst, Vector2{ 0, 0 }, 0.0f, WHITE);
}

static void rysujPrzycisk(Font font, Rectangle r, const char* tekst, Color kolor)
{
    DrawRectangleRounded(r, 0.3f, 8, kolor);
    Vector2 wym = MeasureTextEx(font, tekst, 16, 1);
    DrawTextEx(font, tekst, Vector2{ r.x + (r.width - wym.x) / 2, r.y + (r.height - wym.y) / 2 }, 16, 1, WHITE);
}

OknoMapy::OknoMapy(string trasa, string artysta, Koncert** tablica, int liczba, int budz)
{
    nazwaTrasy = trasa;
    nazwaArtysty = artysta;
    koncerty = tablica;
    liczbaKoncertow = liczba;
    budzet = budz;
}

void OknoMapy::pokaz()
{
    if (liczbaKoncertow == 0) return;

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1100, 680, nazwaTrasy.c_str());
    SetTargetFPS(60);

    Font font = GetFontDefault();
    bool maFont = FileExists("C:/Windows/Fonts/segoeui.ttf");
    if (maFont)
    {
        font = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 40, 0, 0);
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }

    Texture2D texFoto = {};
    string klucz = naMale(nazwaArtysty + " " + nazwaTrasy);
    if (klucz.find("bts") != string::npos && FileExists("bts.png")) texFoto = LoadTexture("bts.png");
    else if (klucz.find("jennie") != string::npos && FileExists("jennie.png")) texFoto = LoadTexture("jennie.png");

    Texture2D texMapa = {};
    if (FileExists("mapa.png")) texMapa = LoadTexture("mapa.png");

    vector<Punkt> punkty;
    for (int i = 0; i < liczbaKoncertow; i++)
    {
        Punkt p;
        p.miasto = koncerty[i]->pobierzMiasto();
        znajdzPozycje(p.miasto, i, p.x, p.y);
        punkty.push_back(p);
    }

    bool maPlakat = (texFoto.id != 0);
    int ekran = maPlakat ? 0 : 1;
    int wybrany = 0, odcinek = 0, pauzaKlatki = 20;
    float postep = 0.0f, samX = punkty[0].x, samY = punkty[0].y, kat = 0.0f;
    bool pauza = false, bladKom = false;
    string komunikat = "";

    Rectangle btnMapa = { 465, 515, 280, 48 };
    Rectangle btnPlakat = { 670, 25, 110, 38 };
    Rectangle btnRestart = { 790, 25, 145, 38 };
    Rectangle btnPauza = { 945, 25, 130, 38 };
    Rectangle btnBilety = { 885, 598, 190, 46 };

    Color bordoCiemny = { 24, 8, 12, 255 };
    Color bordoKarta = { 44, 14, 22, 255 };
    Color bordoAkt = { 82, 20, 34, 255 };
    Color czerwien = { 185, 24, 42, 255 };
    Color jasnaCzerw = { 230, 45, 65, 255 };
    Color roz = { 252, 175, 185, 255 };
    Color tekst = { 235, 215, 220, 255 };

    while (!WindowShouldClose())
    {
        if (ekran == 1 && !pauza && punkty.size() >= 2)
        {
            if (pauzaKlatki > 0) pauzaKlatki--;
            else
            {
                Punkt a = punkty[odcinek];
                Punkt b = punkty[odcinek + 1];
                samX = a.x + (b.x - a.x) * postep;
                samY = a.y + (b.y - a.y) * postep;
                kat = atan2(b.y - a.y, b.x - a.x);
                postep += 0.007f;
                if (postep >= 1.0f)
                {
                    postep = 0.0f;
                    odcinek = (odcinek + 1) % ((int)punkty.size() - 1);
                    pauzaKlatki = 35;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mysz = GetMousePosition();
            if (ekran == 0 && CheckCollisionPointRec(mysz, btnMapa))
            {
                ekran = 1;
            }
            else if (ekran == 1)
            {
                if (maPlakat && CheckCollisionPointRec(mysz, btnPlakat)) ekran = 0;
                else if (CheckCollisionPointRec(mysz, btnRestart))
                {
                    odcinek = 0; postep = 0.0f; pauza = false;
                    samX = punkty[0].x; samY = punkty[0].y;
                }
                else if (CheckCollisionPointRec(mysz, btnPauza)) pauza = !pauza;
                else if (CheckCollisionPointRec(mysz, btnBilety))
                {
                    try
                    {
                        koncerty[wybrany]->sprzedajBilety(1000);
                        komunikat = "Sprzedano +1000 biletow (" + koncerty[wybrany]->pobierzMiasto() + ")!";
                        bladKom = false;
                    }
                    catch (const char* blad)
                    {
                        komunikat = blad;
                        bladKom = true;
                    }
                }
                else
                {
                    int ile = liczbaKoncertow < 6 ? liczbaKoncertow : 6;
                    for (int i = 0; i < ile; i++)
                    {
                        if (CheckCollisionPointRec(mysz, Rectangle{ 15, 135.0f + i * 86, 240, 76 }))
                        {
                            wybrany = i; komunikat = "";
                        }
                    }
                    for (int i = 0; i < (int)punkty.size(); i++)
                    {
                        if (CheckCollisionPointCircle(mysz, Vector2{ punkty[i].x, punkty[i].y }, 14))
                        {
                            wybrany = i; komunikat = "";
                        }
                    }
                }
            }
        }

        BeginDrawing();

        if (ekran == 0)
        {
            DrawRectangleGradientV(0, 0, 1100, 680, Color{ 48, 10, 20, 255 }, Color{ 16, 5, 8, 255 });
            DrawRectangleRounded(Rectangle{ 50, 50, 1000, 580 }, 0.05f, 10, bordoCiemny);

            DrawRectangleRounded(Rectangle{ 81, 91, 348, 448 }, 0.04f, 8, czerwien);
            rysujZdjecie(texFoto, Rectangle{ 85, 95, 340, 440 });

            DrawTextEx(font, "OFICJALNA TRASA KONCERTOWA", Vector2{ 465, 98 }, 15, 1, roz);
            DrawTextEx(font, nazwaTrasy.c_str(), Vector2{ 465, 125 }, 32, 1, WHITE);
            string info = "Gwiazda: " + nazwaArtysty + "   |   Budzet: " + to_string(budzet) + " EUR";
            DrawTextEx(font, info.c_str(), Vector2{ 465, 174 }, 18, 1, tekst);
            DrawLine(465, 210, 1010, 210, czerwien);
            DrawTextEx(font, "HARMONOGRAM PRZYSTANKOW", Vector2{ 465, 224 }, 15, 1, roz);

            int ile = liczbaKoncertow < 5 ? liczbaKoncertow : 5;
            for (int i = 0; i < ile; i++)
            {
                float ry = 256.0f + i * 46;
                DrawRectangleRounded(Rectangle{ 465, ry, 545, 38 }, 0.25f, 6, bordoKarta);
                string wpis = to_string(i + 1) + ". " + koncerty[i]->pobierzMiasto() + " - " + koncerty[i]->pobierzStadion();
                DrawTextEx(font, wpis.c_str(), Vector2{ 480, ry + 10 }, 16, 1, WHITE);
                DrawTextEx(font, koncerty[i]->pobierzDate().c_str(), Vector2{ 905, ry + 10 }, 15, 1, roz);
            }

            rysujPrzycisk(font, btnMapa, "PRZEJDZ DO MAPY  ->", czerwien);
        }
        else
        {
            ClearBackground(Color{ 18, 6, 9, 255 });
            DrawRectangleGradientH(0, 0, 1100, 90, Color{ 40, 10, 18, 255 }, Color{ 82, 16, 28, 255 });
            DrawLine(0, 90, 1100, 90, czerwien);

            int przesuniecie = 20;
            if (maPlakat)
            {
                rysujZdjecie(texFoto, Rectangle{ 18, 11, 68, 68 });
                przesuniecie = 100;
            }

            DrawTextEx(font, nazwaTrasy.c_str(), Vector2{ (float)przesuniecie, 15 }, 25, 1, WHITE);
            string pod = "Gwiazda: " + nazwaArtysty + "  |  Budzet: " + to_string(budzet) + " EUR";
            DrawTextEx(font, pod.c_str(), Vector2{ (float)przesuniecie, 50 }, 16, 1, tekst);

            if (maPlakat) rysujPrzycisk(font, btnPlakat, "<- PLAKAT", bordoAkt);
            rysujPrzycisk(font, btnRestart, "ODTWORZ LOT", czerwien);
            rysujPrzycisk(font, btnPauza, pauza ? "WZNOW" : "PAUZA", bordoAkt);

            DrawRectangle(0, 91, 270, 589, bordoCiemny);
            DrawLine(270, 91, 270, 680, czerwien);
            DrawTextEx(font, "PRZYSTANKI TRASY", Vector2{ 18, 106 }, 15, 1, roz);

            int ile = liczbaKoncertow < 6 ? liczbaKoncertow : 6;
            for (int i = 0; i < ile; i++)
            {
                float cy = 135.0f + i * 86;
                DrawRectangleRounded(Rectangle{ 15, cy, 240, 76 }, 0.2f, 6, i == wybrany ? bordoAkt : bordoKarta);
                string l1 = to_string(i + 1) + ". " + koncerty[i]->pobierzMiasto() + " (" + koncerty[i]->pobierzDate() + ")";
                string l2 = koncerty[i]->pobierzStadion();
                string l3 = "Bilety: " + to_string(koncerty[i]->pobierzSprzedane()) + " / " + to_string(koncerty[i]->pobierzPojemnosc());
                DrawTextEx(font, l1.c_str(), Vector2{ 24, cy + 8 }, 15, 1, WHITE);
                DrawTextEx(font, l2.c_str(), Vector2{ 24, cy + 30 }, 14, 1, tekst);
                DrawTextEx(font, l3.c_str(), Vector2{ 24, cy + 50 }, 13, 1, roz);
            }

            if (texMapa.id != 0)
            {
                Rectangle src = { 0, 0, (float)texMapa.width, (float)texMapa.height };
                DrawTexturePro(texMapa, src, Rectangle{ 270, 90, 830, 475 }, Vector2{ 0, 0 }, 0, Color{ 165, 36, 52, 255 });
            }

            for (int i = 0; i + 1 < (int)punkty.size(); i++)
            {
                DrawLineEx(Vector2{ punkty[i].x, punkty[i].y }, Vector2{ punkty[i + 1].x, punkty[i + 1].y }, 2.5f, roz);
            }

            for (int i = 0; i < (int)punkty.size(); i++)
            {
                float r = (i == wybrany) ? 9.0f : 7.0f;
                DrawCircleV(Vector2{ punkty[i].x, punkty[i].y }, r, i == wybrany ? jasnaCzerw : czerwien);
                DrawCircleLines((int)punkty[i].x, (int)punkty[i].y, r, WHITE);
                DrawTextEx(font, to_string(i + 1).c_str(), Vector2{ punkty[i].x - 3, punkty[i].y - 7 }, 13, 1, WHITE);
                DrawTextEx(font, punkty[i].miasto.c_str(), Vector2{ punkty[i].x + r + 4, punkty[i].y - 7 }, 14, 1, WHITE);
            }

            float fx = cos(kat), fy = sin(kat), rx = -fy, ry = fx;
            DrawLineEx(Vector2{ samX - fx * 11, samY - fy * 11 }, Vector2{ samX + fx * 13, samY + fy * 13 }, 3.5f, WHITE);
            DrawLineEx(Vector2{ samX - fx * 2 - rx * 12, samY - fy * 2 - ry * 12 }, Vector2{ samX - fx * 2 + rx * 12, samY - fy * 2 + ry * 12 }, 3.0f, WHITE);
            DrawLineEx(Vector2{ samX - fx * 9 - rx * 5, samY - fy * 9 - ry * 5 }, Vector2{ samX - fx * 9 + rx * 5, samY - fy * 9 + ry * 5 }, 2.5f, roz);

            DrawRectangle(271, 565, 829, 115, bordoCiemny);
            DrawLine(271, 565, 1100, 565, czerwien);

            Koncert* k = koncerty[wybrany];
            string nag = k->pobierzMiasto() + " - " + k->pobierzStadion() + "  |  Data: " + k->pobierzDate();
            DrawTextEx(font, nag.c_str(), Vector2{ 290, 578 }, 19, 1, WHITE);

            float proc = (k->pobierzPojemnosc() > 0) ? (float)k->pobierzSprzedane() / k->pobierzPojemnosc() : 0.0f;
            if (proc > 1.0f) proc = 1.0f;
            DrawRectangleRounded(Rectangle{ 290, 612, 400, 14 }, 0.7f, 6, bordoKarta);
            if (proc > 0) DrawRectangleRounded(Rectangle{ 290, 612, 400 * proc, 14 }, 0.7f, 6, jasnaCzerw);

            string bil = to_string(k->pobierzSprzedane()) + " / " + to_string(k->pobierzPojemnosc()) + " biletow";
            DrawTextEx(font, bil.c_str(), Vector2{ 705, 609 }, 16, 1, tekst);

            if (!komunikat.empty())
            {
                DrawTextEx(font, komunikat.c_str(), Vector2{ 290, 642 }, 15, 1, bladKom ? roz : Color{ 130, 235, 170, 255 });
            }
            else
            {
                DrawTextEx(font, "Miasta podawaj po angielsku (np. Seoul, London, New York).", Vector2{ 290, 642 }, 14, 1, roz);
            }
            rysujPrzycisk(font, btnBilety, "+1000 BILETOW", czerwien);
        }

        EndDrawing();
    }

    if (maFont) UnloadFont(font);
    if (texFoto.id != 0) UnloadTexture(texFoto);
    if (texMapa.id != 0) UnloadTexture(texMapa);
    CloseWindow();
}