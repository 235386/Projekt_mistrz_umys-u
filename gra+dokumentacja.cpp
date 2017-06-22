#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <ctime>

using namespace std;
//----------------------------------------------------
// Deklaracja klasy gry
//----------------------------------------------------

  class TKod_z
  {
     public:
      int tz[4];
      void Inicjuj();
      void Wyswietl();
      void Oceniaj(int t[]);
  };

  class TKod_t : public TKod_z
  {
     public:
       void Inicjuj(int runda);
       void Wyswietl(int runda, int kursor);
  };
    class TMasterMind
  {
      TKod_z z;
      TKod_t t;
      int runda;
      void Ocen_t();
      int t_rowne_z();
      int KoniecGry();
    public:
      void StronaTytulowa();
      void Graj();
      int JeszczeRaz();
  };
//----------------------------------------------------
// PROCEDURY I FUNKCJE POMOCNICZE
//----------------------------------------------------

const int BLACK        = 0;
const int BLUE         = 1;
const int GREEN        = 2;
const int CYAN         = 3;
const int RED          = 4;
const int MAGENTA      = 5;
const int BROWN        = 6;
const int LIGHTGRAY    = 7;
const int DARKGRAY     = 8;
const int LIGHTBLUE    = 9;
const int LIGHTGREEN   = 10;
const int LIGHTCYAN    = 11;
const int LIGHTRED     = 12;
const int LIGHTMAGENTA = 13;
const int YELLOW       = 14;
const int WHITE        = 15;

static int __BACKGROUND = BLACK;
static int __FOREGROUND = LIGHTGRAY;

// Procedura ustawia pozycje wydruku w oknie konsoli
//----------------------------------------------------
void pozycja_konsoli(int x, int y)
{
  COORD c;// Klasa koordynatow w windows.h
  c.X = x - 1;
  c.Y = y - 1;
 //ustawianie pozycji kursora w konsoli
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
// Procedura ustawia atrybuty koloru tekstu i tla
//----------------------------------------------------
void kolor_tekstu_tla(int _attr)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}
// Funkcja zwraca aktualna pozycjê x kursora
//----------------------------------------------------
int pozycja_kursora_x()
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.dwCursorPosition.X + 1;
}
// Funkcja zwraca aktualna pozycje y kursora
//----------------------------------------------------
int pozycja_kursora_y()
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.dwCursorPosition.Y + 1;
}
// Procedura czyœci zawartosc okna konsoli
//----------------------------------------------------
void czysc_okno_konsoli()
{
  DWORD written;

  FillConsoleOutputAttribute(GetStdHandle (STD_OUTPUT_HANDLE),
    __FOREGROUND + (__BACKGROUND << 4), 2000, (COORD){0, 0}, &written);
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
    2000, (COORD){0, 0}, &written);
  pozycja_konsoli(1, 1);
}
// Procedura ukrywa kursor okienka konsoli
//------------------------------------------------------
void CursorOff()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  Info.bVisible = 0;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
};
// Procedura przywraca kursor okienka konsoli
//------------------------------------------------------
void CursorOn()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  Info.bVisible = -1;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
};
// Procedura centruje w biezacym wierszu
// tekst podany jako parametr
//----------------------------------------------------

void Centruj(string t)
{
  pozycja_konsoli(1 + (80 - t.length()) / 2, pozycja_kursora_y());//ustawia kursor na lewo od srodka ->
  cout << t << endl;// -> a nastepnie wypisuje wysrodkowany tekst
}
//----------------------------------------------------
// Definicje metod klasy gry
//----------------------------------------------------

// Obiekt TKod_z
//****************************************************

// Metoda losuje kod kolorów dla komputera i umieszcza
// go w tablicy tz obiektu
//------------------------------------------------------
void TKod_z::Inicjuj()
{
    for(int i = 0; i <= 3; i++) tz[i] = rand() % 6;
}
// Metoda wyswietla na biezacej pozycji kolejne
// kolory zawarte w kodzie tz. Kolory sa wyswietlane
// w postaci kolorowych prostokatów oddzielonych spacji.
//------------------------------------------------------
void TKod_z::Wyswietl()
{
    const int kolory[6] = {BLACK,LIGHTBLUE,LIGHTGREEN,LIGHTRED,LIGHTMAGENTA,YELLOW};

    for(int i = 0; i < 4; i++)
    {
        kolor_tekstu_tla(0x80 + kolory[tz[i]]);
        cout << char(219) << ' ';
    };
}
// Procedura porównuje podany jej w formie tablicy kod
// z kodem w polu tz. Wynik jest wyswietlany na biezacej
// pozycji ekranu w postaci gwiazdek czarnych - zgodnoœæ
// kolorów i pozycji oraz bia³ych - zgodnosc kolorów
//------------------------------------------------------
void TKod_z::Oceniaj(int t[])
{
    int kz[4], kt[4], czarne, biale, i, j;// k jak kopia

    for(i = 0; i <= 3; i++)
    {
        kz[i] = tz[i]; kt[i] = t[i];
    };
    czarne = 0;
    for(i = 0; i <= 3; i++)
        if(kt[i] == kz[i])
        {
            czarne++; kz[i] = -1; kt[i] = -2;
        };
    biale = 0;
    for(i = 0; i <= 3; i++)
        for(j = 0; j <= 3; j++)
            if(kt[i] == kz[j])
            {
                biale++; kz[j] = -1; kt[i] = -2; break;
            };
    kolor_tekstu_tla(0x80); for(i = 1; i <= czarne; i++) cout << "* ";
    kolor_tekstu_tla(0x8f); for(i = 1; i <= biale; i++)  cout << "* ";
}
// Obiekt TKod_t
//***********************************************************

// Zmodyfikowana procedura wyœwietlania kodu tz,
// z której korzysta obiekt kodu gracza. Wyswietla
// ona w bie¿¹cym wierszu ekranu segment gry, który
// oprócz kodu zawieraæ mo¿e dodatkowo kursor, czyli
// prostok¹tn¹ ramka otaczajaca kwadrat koloru na
// pozycji kursor. Przed kodem koloru wyœwietlony
// zostaje numer rundy, który obiekt kodu gracza
// otrzymuje z obiektu gry.
//------------------------------------------------------
void TKod_t::Wyswietl(int runda, int kursor)
{
  int y;

  y = pozycja_kursora_y();// zapamietujemy pozycje wydruku
  kolor_tekstu_tla(0x87); pozycja_konsoli(1,y);
  Centruj("                    ");
  pozycja_konsoli(31,pozycja_kursora_y()); cout << " " << runda << ": ";
  TKod_z::Wyswietl();// do wyswietlenia kodu wykorzystujemy
                       // odziedziczona metode

  cout << "        \n";
  kolor_tekstu_tla(0x80); Centruj("____________________");
  if(kursor >= 0)
  {
    kolor_tekstu_tla(0x84);
  };
  pozycja_konsoli(1,y);// odtwarzamy pozycje wydruku
}

// Procedura odczytuje kod koloru gracza.
//------------------------------------------------------
void TKod_t::Inicjuj(int runda)
{
    int x,i;
    char c;

    for(i = 0; i < 4; i++) tz[i] = 0;
    x = 0;
    do
    {
        Wyswietl(runda,x);
        do
        {
            c = getch();
        } while(c == '\0');
        Wyswietl(runda,-1);// to usunie kursor z ekranu
        switch(c)
        {
            case char(75) : x = (x + 3) % 4; break;//strzalka w lewo
            case char(77) : x = (x + 1) % 4; break;//strzalka w prawo
            case char(72) : tz[x] = (tz[x] + 1) % 6; break;//strzalka w góre
            case char(80) : tz[x] = (tz[x] + 5) % 6; break;//strza³ka w dól
        };
        } while(int(c) != 13);// klawisz Enter
}
// Obiekt TMasterMind
//*************************************************************

// Metoda ocenia kod wprowadzony przez gracza. Wydruk
// jest przemieszczany do nastêpnego segmentu gry.
//------------------------------------------------------
void TMasterMind::Ocen_t()
{
    pozycja_konsoli(43, pozycja_kursora_y() + 1);// ustawiamy pozycje oceny
    z.Oceniaj(t.tz);// oceniamy kod gracza
    cout << endl << endl;// przechodzimy do nastepnego segmentu
}

// Metoda porównuje obiekty z i t. Zwraca true, jesli
// zawieraje ten sam kod.
//------------------------------------------------------
int TMasterMind::t_rowne_z()
{
 //int test,i;
    for(int i=0; i<4; ++i) if(z.tz[i] != t.tz[i]) return 0;
    return 1;
  //test = true;
  //for(i = 0; i <= 3; i++) test = test && (z.tz[i] == t.tz[i]);
  //return(test);
}
// Metoda testuje warunek zakonczenia gry. Jesli gra sie
// koñczy, wypisuje odpowiedni komunikat i zwraca true
//------------------------------------------------------
int TMasterMind::KoniecGry()
{
  runda++; // zwiekszamy numer rundy
  if(t_rowne_z())
  {
    kolor_tekstu_tla(0x0e);
    Centruj("GRATULACJE - odgadles kod komputera!");
    pozycja_konsoli(35,3); z.Wyswietl();
  }
  else if(runda > 6)
  {
    kolor_tekstu_tla(0x0c);
    Centruj("GRA SKONCZONA - nie odgadles kodu komputera!");
    pozycja_konsoli(35,3); z.Wyswietl();// wyswietlamy kod komputera
    return(true);
  };
  return(false);
}
// Metoda wyswietla strone tytulow i czeka na
// dowolny klawisz, po czym czyœci ekran.
//------------------------------------------------------
void TMasterMind::StronaTytulowa()
{

  kolor_tekstu_tla(0); czysc_okno_konsoli();
  pozycja_konsoli(1,3); kolor_tekstu_tla(0x3b);
  Centruj("                                  ");
  Centruj("         GRA NA ZALICZENIE        ");
  Centruj("  -------> MISTRZ UMYSLU <------  ");
  Centruj("                                  ");
  kolor_tekstu_tla(0x1f);
  Centruj("                                  ");
  Centruj("         Arkadiusz Wajs           ");
  Centruj("                                  ");
  kolor_tekstu_tla(0xf0);
  Centruj("                                  ");
  Centruj("   Odgadnij w szesciu kolejkach   ");
  Centruj("   kod koloru  wylosowany przez   ");
  Centruj("    komputer z 6 roznych barw.    ");
  Centruj("   Zmiana kolorow strzalkami.     ");
  kolor_tekstu_tla(0xf4);
  Centruj("        Gdy bedziesz gotowy,      ");
  Centruj("     nacsnij dowolny klawisz      ");
  Centruj("                                  ");
  kolor_tekstu_tla(0x0e);
  while(getch() == '\0') ;// Oczekiwanie na dowolny klawisz
  kolor_tekstu_tla(0x07); czysc_okno_konsoli();
}
// Metoda odpowiedzialna za prowadzenie gry.
//------------------------------------------------------
void TMasterMind::Graj()
{
  z.Inicjuj(); runda = 1;
  kolor_tekstu_tla(0x4e);
  Centruj("    MISTRZ UMYSLU   ");
  kolor_tekstu_tla(0x87);
  Centruj("                    ");
  Centruj("    ? ? ? ?         ");
  Centruj("                    ");
  do
  {
    t.Inicjuj(runda);
    Ocen_t();
  } while(!KoniecGry());
}
// Metoda upewnia sie, ze gracz ma ochote na jeszcze
// jedna gre. Zwraca w atkim wypadku true.
//------------------------------------------------------
int TMasterMind::JeszczeRaz()
{
  pozycja_konsoli(1,24); kolor_tekstu_tla(WHITE);
  Centruj("Zagrasz jeszcze raz ? [T] = Tak, [inny] = Nie");
  return(toupper(getch()) == 'T');
}
// Blok glówny programu
//--------------------------------------------------
int main()
{
  TMasterMind gra;

  srand((unsigned)time(NULL));
  CursorOff();
  do
  {
     gra.StronaTytulowa();
     gra.Graj();
  } while(gra.JeszczeRaz());
  kolor_tekstu_tla(0x07); CursorOn(); czysc_okno_konsoli();

    return(0);
}
