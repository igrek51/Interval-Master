#ifndef CONFIG_H
#define CONFIG_H

using namespace std;

//tryb
const int TRYB_NAUKA = 0;
const int TRYB_ROZPOZNAWANIE = 1;
//typ interwału
const int TYP_INTERWAL_R = 0;
const int TYP_INTERWAL_M = 1;
const int TYP_INTERWAL_RM = 2;
const int TYP_INTERWAL_H = 3;
const int TYP_INTERWAL = 3;
const int TYP_DZWIEK = 4;
const int TYP_AKORD_D = 5;
const int TYP_AKORD_M = 6;
const int TYP_AKORD = 5;
//odpowiedź akordu
const int ODP_D = 0;
const int ODP_M = 1;

class Config{
public:
    Config();
    ~Config();
    //z pliku config
	int midi_instrument;
	int midi_volume;
	int midi_basetone;
    int midi_pause;
    //zmienne aplikacji
    int ustawienia_tryb;
    int ustawienia_type;
    bool ustawienia_interwaly[12];
    bool ustawienia_oktawy[7];
    //stałe
	static const int komunikaty_c[];
};

#endif // CONFIG_H
