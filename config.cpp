#include "config.h"
#include "app.h"
#include <vector>
#include <fstream>

Config::Config(){
    ustawienia_tryb = TRYB_ROZPOZNAWANIE;
    ustawienia_type = TYP_INTERWAL_RM;
    for(int i=0; i<12; i++)
        ustawienia_interwaly[i]=false;
    for(int i=0; i<7; i++)
        ustawienia_oktawy[i]=false;
    stat_odpowiedziany = false;
    stat_odgadniety = false;
    stat_przesluchania = 0;
    stat_odpowiedzi = 0;
    stat_bledne = 0;
}

Config::~Config(){
    delete ustawienia_interwaly;
}

vector<string>* get_all_lines(string filename){
    fstream plik;
    plik.open(filename.c_str(),fstream::in|fstream::binary);
    if(!plik.good()){
        plik.close();
        return NULL;
    }
    vector<string>* lines = new vector<string>;
    string linia, linia2;
    do{
        getline(plik,linia,'\n');
        linia2="";
        for(unsigned int i=0; i<linia.length(); i++){
            if(linia[i]=='\r') continue;
            linia2+=linia[i];
        }
        lines->push_back(linia2);
    }while(!plik.eof());
    plik.close();
    return lines;
}

bool next_element(string element0, vector<string> *elements, int &next){
    for(unsigned int i=0; i<elements->size(); i++){
        if((*elements)[i]==element0){
            if(i+1<elements->size()){
                next = i+1;
                return true;
            }else{
                cout<<"[!] BLAD: brak parametru po "<<element0<<endl;
                next = -2;
                return false; //nie istnieje nastêpny
            }
        }
    }
    next = -1;
    return false; //nie znalaz³o argumentu
}

void update_config(vector<string> *&lines, string var_name, int var_value){
    int next2 = 0;
    stringstream ss2;
    ss2<<var_value;
    if(next_element(var_name,lines,next2)){ //jeœli ju¿ jest ten element
        //nadpisanie linii danych
        lines->at(next2) = ss2.str();
    }else{
        //utworzenie nowych linii
        lines->push_back(var_name);
        lines->push_back(ss2.str());
    }
}

void App::load_config(){
	log("Wczytywanie ustawieñ...");
    //domyœlne wartoœci
    config->midi_instrument = 25;
    config->midi_volume = 100;
    config->midi_basetone = 60;
    config->midi_pause = 500;
    //wczytanie pliku
    vector<string> *lines = get_all_lines("config.txt");
    if(lines==NULL){
        echo("B³¹d: Brak pliku konfiguracyjnego");
        return;
    }
    int next2 = 0;
    if(next_element("instrument=",lines,next2)){
        config->midi_instrument = atoi(lines->at(next2).c_str());
    }
    if(next_element("volume=",lines,next2)){
        config->midi_volume = atoi(lines->at(next2).c_str());
    }
    if(next_element("basetone=",lines,next2)){
        config->midi_basetone = atoi(lines->at(next2).c_str());
    }
    if(next_element("pause=",lines,next2)){
        config->midi_pause = atoi(lines->at(next2).c_str());
    }
    lines->clear();
    delete lines;
}

void App::save_config(){
    log("Zapisywanie ustawieñ...");
    //wczytanie starego pliku
    vector<string> *lines = get_all_lines("config.txt");
    if(lines==NULL){ //jeœli nie by³o pliku
        lines = new vector<string>;
    }
    //aktualizacja danych
    update_config(lines, "instrument=", config->midi_instrument);
    update_config(lines, "volume=", config->midi_volume);
    update_config(lines, "basetone=", config->midi_basetone);
    update_config(lines, "pause=", config->midi_pause);
    //zapisanie do pliku
    fstream plik;
    plik.open("config.txt",fstream::out|fstream::trunc|fstream::binary);
    if(!plik.good()){
        plik.close();
        return;
    }
    for(unsigned int i=0; i<lines->size(); i++){
        plik<<lines->at(i)<<endl;
    }
    plik.close();
    lines->clear();
    delete lines;
}

void App::config_open(){
    system("config.txt");
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

const int Config::komunikat_good[] = {
0,150,0
};

const int Config::komunikat_bad[] = {
180,0,0
};
