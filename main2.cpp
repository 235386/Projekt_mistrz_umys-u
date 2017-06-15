#include <conio.h>
#include <windows.h>
#include <strings.h>
#include <iostream>
#include <ctime>

using namespace std;

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

void pozycja_konsoli(int x, int y)
{
  COORD c;
  c.X = x - 1;
  c.Y = y - 1;

  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void kolor_tekstu_tla(int _attr)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _attr);
}
int pozycja_kursora_x()
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.dwCursorPosition.X + 1;
}

int pozycja_kursora_y()
{
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.dwCursorPosition.Y + 1;
}

void czysc_okno_konsoli()
{
  DWORD written;

  FillConsoleOutputAttribute(GetStdHandle (STD_OUTPUT_HANDLE),
    __FOREGROUND + (__BACKGROUND << 4), 2000, (COORD){0, 0}, &written);
  FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ',
    2000, (COORD){0, 0}, &written);
  pozycja_konsoli(1, 1);
}
void CursorOff()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  Info.bVisible = 0;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
};
void CursorOn()
{
  CONSOLE_CURSOR_INFO Info;

  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
  Info.bVisible = -1;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
};
string PL(string s)
{
  int i;
  char c;

  for(i = 0; i < s.length(); i++)
  {
    switch(s[i])
    {
      case '¹' : c = char(165); break;
      case 'æ' : c = char(134); break;
      case 'ê' : c = char(169); break;
      case '³' : c = char(136); break;
      case 'ñ' : c = char(228); break;
      case 'ó' : c = char(162); break;
      case 'œ' : c = char(152); break;
      case '¿' : c = char(190); break;
      case 'Ÿ' : c = char(171); break;
      case '¥' : c = char(164); break;
      case 'Æ' : c = char(143); break;
      case 'Ê' : c = char(168); break;
      case '£' : c = char(157); break;
      case 'Ñ' : c = char(227); break;
      case 'Ó' : c = char(224); break;
      case 'Œ' : c = char(151); break;
      case '¯' : c = char(189); break;
      case '' : c = char(141); break;
      default:   c = s[i];
    };
    s[i] = c;
  };
  return s;
}
void Centruj(string t)
{
  pozycja_konsoli(1 + (80 - t.length()) / 2, pozycja_kursora_y());
  cout << t << endl;
}
void Ramka(int xp,int yp,int xk,int yk)
{
  int i;

  pozycja_konsoli(xp,yp); putch(char(218));
  pozycja_konsoli(xp,yk); putch(char(192));
  pozycja_konsoli(xk,yp); putch(char(191));
  pozycja_konsoli(xk,yk); putch(char(217));
  for(i = xp + 1; i <= xk - 1; i++)
  {
    pozycja_konsoli(i,yp); putch(char(196));
    pozycja_konsoli(i,yk); putch(char(196));
  };
  for(i = yp + 1; i <= yk - 1; i++)
  {
    pozycja_konsoli(xp,i); putch(char(179));
    pozycja_konsoli(xk,i); putch(char(179));
  };
}

void TKod_z::Inicjuj()
{
    for(int i = 0; i <= 3; i++) tz[i] = rand() % 6;
}
void TKod_z::Wyswietl()
{
    const int kolory[6] = {BLACK,LIGHTBLUE,LIGHTGREEN,LIGHTRED,LIGHTMAGENTA,YELLOW};

    for(int i = 0; i < 4; i++)
    {
        kolor_tekstu_tla(0x80 + kolory[tz[i]]);
        cout << char(219) << ' ';
    };
}
void TKod_z::Oceniaj(int t[])
{
    int kz[4], kt[4], czarne, biale, i, j;

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
void TKod_t::Wyswietl(int runda, int kursor)
{
  int y;

  y = pozycja_kursora_y();
  kolor_tekstu_tla(0x87); pozycja_konsoli(1,y);
  Centruj("                    ");
  pozycja_konsoli(31,pozycja_kursora_y()); cout << " " << runda << ": ";
  TKod_z::Wyswietl();

  cout << "        \n";
  kolor_tekstu_tla(0x80); Centruj("____________________");
  if(kursor >= 0)
  {
    kolor_tekstu_tla(0x84); Ramka(34 + 2 * kursor, y, 36 + 2 * kursor, y + 2);
  };
  pozycja_konsoli(1,y);
}
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
        Wyswietl(runda,-1);
        switch(c)
        {
            case char(75) : x = (x + 3) % 4; break;
            case char(77) : x = (x + 1) % 4; break;
            case char(72) : tz[x] = (tz[x] + 1) % 6; break;
            case char(80) : tz[x] = (tz[x] + 5) % 6; break;
        };
        } while(int(c) != 13);
}
void TMasterMind::Ocen_t()
{
    pozycja_konsoli(43, pozycja_kursora_y() + 1);
    z.Oceniaj(t.tz);
    cout << endl << endl;
}
int TMasterMind::t_rowne_z()
{

    for(int i=0; i<4; ++i) if(z.tz[i] != t.tz[i]) return 0;
    return 1;
}
int TMasterMind::KoniecGry()
{
  runda++;
  if(t_rowne_z())
  {
    kolor_tekstu_tla(0x0e);
    Centruj(PL("GRATULACJE - odgad³eœ kod komputera!"));
    pozycja_konsoli(35,3); z.Wyswietl();
  }
  else if(runda > 6)
  {
    kolor_tekstu_tla(0x0c);
    Centruj(PL("GRA SKOÑCZONA - nie odgad³eœ kodu komputera!"));
    pozycja_konsoli(35,3); z.Wyswietl();
    return(true);
  };
  return(false);
}
void TMasterMind::StronaTytulowa()
{

  kolor_tekstu_tla(0); czysc_okno_konsoli();
  pozycja_konsoli(1,3); kolor_tekstu_tla(0x3b);
  Centruj(PL("                                  "));
  Centruj(PL("  -------GRA NA ZALICZENIE------  "));
  Centruj(PL("  ---------MISTRZ UMYSLU----------"));
  Centruj(PL("                                  "));
  kolor_tekstu_tla(0x1f);
  Centruj(PL("                                  "));
  Centruj(PL("         Arkadiusz Wajs           "));
  Centruj(PL("                                  "));
  kolor_tekstu_tla(0xf0);
  Centruj(PL("                                  "));
  Centruj(PL("   Odgadnij w szeœciu kolejkach   "));
  Centruj(PL("   kod koloru  wylosowany przez   "));
  Centruj(PL("    komputer z 6 ró¿nych barw.    "));
  Centruj(PL("                                  "));
  kolor_tekstu_tla(0xf4);
  Centruj(PL("        Gdy bedziesz gotowy,      "));
  Centruj(PL("     naciœnij dowolny klawisz     "));
  Centruj(PL("                                  "));
  kolor_tekstu_tla(0x0e); Ramka(23,2,58,18);
  while(getch() == '\0') ;
  kolor_tekstu_tla(0x07); czysc_okno_konsoli();
}
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
int TMasterMind::JeszczeRaz()
{
  pozycja_konsoli(1,24); kolor_tekstu_tla(WHITE);
  Centruj("Zagrasz jeszcze raz ? [T] = Tak, [inny] = Nie");
  return(toupper(getch()) == 'T');
}

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
