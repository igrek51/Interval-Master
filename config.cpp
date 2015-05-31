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
                next = -2;
                return false; //nie istnieje nastêpny
            }
        }
    }
    next = -1;
    return false; //nie znalaz³o elementu
}

void update_config(vector<string> *&lines, string var_name, string var_value){
    int next2 = 0;
    if(next_element(var_name,lines,next2)){ //jeœli ju¿ jest ten element
        //nadpisanie linii danych
        lines->at(next2) = var_value;
    }else{
        //utworzenie nowych linii
        lines->push_back(var_name);
        lines->push_back(var_value);
    }
}

void update_config(vector<string> *&lines, string var_name, int var_value){
    stringstream ss2;
    ss2<<var_value;
    update_config(lines, var_name, ss2.str());
}

void App::load_config(){
	log("Wczytywanie ustawieñ...");
    //domyœlne wartoœci
    config->midi_instrument = 25;
    config->midi_volume = 100;
    config->midi_basetone = 60;
    config->midi_pause = 500;
    config->ustawienia_type = TYP_INTERWAL_RM;
    for(int i=0; i<12; i++)
        config->ustawienia_interwaly[i]=false;
    config->ustawienia_interwaly[0] = true;
    config->ustawienia_interwaly[1] = true;
    config->ustawienia_interwaly[2] = true;
    for(int i=0; i<7; i++)
        config->ustawienia_oktawy[i]=false;
    config->ustawienia_oktawy[2] = true;
    config->ustawienia_oktawy[3] = true;
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
    if(next_element("interval_type=",lines,next2)){
        config->ustawienia_type = atoi(lines->at(next2).c_str());
        if(config->ustawienia_type<0) config->ustawienia_type = 0;
        if(config->ustawienia_type>6) config->ustawienia_type = 6;
    }
    if(next_element("interwaly=",lines,next2)){
        string interwaly = lines->at(next2);
        if(interwaly.length()>=12){
            for(int i=0; i<12; i++){
                if(interwaly[i]=='1'){
                    config->ustawienia_interwaly[i] = true;
                }else{
                    config->ustawienia_interwaly[i] = false;
                }
            }
        }
    }
    if(next_element("oktawy=",lines,next2)){
        string oktawy = lines->at(next2);
        if(oktawy.length()>=7){
            for(int i=0; i<7; i++){
                if(oktawy[i]=='1'){
                    config->ustawienia_oktawy[i] = true;
                }else{
                    config->ustawienia_oktawy[i] = false;
                }
            }
        }
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
    update_config(lines, "interval_type=", config->ustawienia_type);
    //ci¹g w³¹czonych interwa³ów
    string interwaly = "";
    for(int i=0; i<12; i++){
        if(config->ustawienia_interwaly[i]){
            interwaly += "1";
        }else{
            interwaly += "0";
        }
    }
    update_config(lines, "interwaly=", interwaly);
    //ci¹g w³¹czonych oktaw
    string oktawy = "";
    for(int i=0; i<7; i++){
        if(config->ustawienia_oktawy[i]){
            oktawy += "1";
        }else{
            oktawy += "0";
        }
    }
    update_config(lines, "oktawy=", oktawy);
    //zapisanie do pliku
    fstream plik;
    plik.open("config.txt",fstream::out|fstream::trunc|fstream::binary);
    if(!plik.good()){
        plik.close();
        return;
    }
    for(unsigned int i=0; i<lines->size(); i++){
        if(i<lines->size()-1 || lines->at(i).length()>0){
            plik<<lines->at(i)<<endl;
        }
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
