#include "config.h"
#include "app.h"

Config::Config(){
    ustawienia_tryb = TRYB_ROZPOZNAWANIE;
    ustawienia_type = TYP_INTERWAL_RM;
    for(int i=0; i<12; i++)
        ustawienia_interwaly[i]=false;
    for(int i=0; i<7; i++)
        ustawienia_oktawy[i]=false;
    odp_akord = 0;
}

Config::~Config(){
    delete ustawienia_interwaly;
}

void App::load_config(){
	log("Wczytywanie ustawieñ...");
	if(!file_exists("conf.ini")) echo("B³¹d: Brak pliku konfiguracyjnego");
	char *temp = new char [512];
	//GetPrivateProfileString("paths","songs_dir",".",temp,512,".\\conf.ini");
	delete[] temp;
    config->midi_instrument=GetPrivateProfileInt("midi","instrument",25,".\\conf.ini");
    config->midi_volume=GetPrivateProfileInt("midi","volume",100,".\\conf.ini");
    config->midi_basetone=GetPrivateProfileInt("midi","basetone",60,".\\conf.ini");
    config->midi_pause=GetPrivateProfileInt("midi","pause",500,".\\conf.ini");
}

void App::config_open(){
    system("conf.ini");
    load_config();
    midi_close(midi_device);
    midi_device = midi_init();
    echo("Wczytano nowe ustawienia.");
}

const int Config::komunikaty_c[] = {
200,200,200,
160,160,160,
120,120,120,
80,80,80,
40,40,40,
0,0,0,
0,0,0,
0,0,0
};
