#include "app.h"
#include "music.h"
#include <vector>

void App::interval_or_chord(){
    string description;
    if(config->ustawienia_type<=TYP_INTERWAL){ //interwa³y
        description = "Wielkoœæ interwa³u";
        for(int i=0; i<12; i++){
            ss_clear(ss);
            ss<<(i+1)<<" - "<<interval_names[i+1];
            SetWindowText(hctrl[23+i],ss.str().c_str());
        }
    }else{ //akord lub dŸwiêk
        if(config->ustawienia_type==TYP_DZWIEK){
            description = "DŸwiêki";
        }else if(config->ustawienia_type>=TYP_AKORD){
            description = "Akordy";
        }
        for(int i=0; i<12; i++){
            ss_clear(ss);
            if(config->ustawienia_type==TYP_AKORD_M){
                ss<<sound_names[i+12];
            }else{
                ss<<sound_names[i];
            }
            SetWindowText(hctrl[23+i],ss.str().c_str());
        }
    }
    SetWindowText(hctrl[22],description.c_str());
    odp_b_refresh();
}

void App::odp_b_refresh(){
    //panel odpowiedzi - w³¹czenie, wy³¹czenie przycisków
    bool enabled[25];
    for(int i=0; i<25; i++)
        enabled[i] = false;
    int polowa = 0;
    if(config->ustawienia_type==TYP_INTERWAL_R){ //prawa po³owa (z zerem)
        polowa = 1;
    }else if(config->ustawienia_type>=TYP_INTERWAL_H){ //prawa po³owa (bez zera)
        polowa = 2;
    }else if(config->ustawienia_type==TYP_INTERWAL_M){ //lewa po³owa (z zerem)
        polowa = -1;
    }
    for(int i=0; i<12; i++){
        if(config->ustawienia_interwaly[i]){
            if(polowa>=1)
                enabled[i+13] = true;
            else if(polowa==-1)
                enabled[11-i] = true;
            else if(polowa==0){
                enabled[i+13] = true;
                enabled[11-i] = true;
            }
        }
    }
    int show;
    for(int i=-12; i<=12; i++){
        //aktywny / nieaktywny
        EnableWindow(hctrl[35+i+12],enabled[i+12]);
        //schowanie przycisków
        show = SW_SHOW;
        if(polowa==-1&&i>0) show = SW_HIDE;
        if(polowa==1&&i<0) show = SW_HIDE;
        if(polowa==2&&i<=0) show = SW_HIDE;
        ShowWindow(hctrl[35+i+12],show);
        //zmiana d³ugoœci i po³o¿enia
        if(polowa==0){
            SetWindowPos(hctrl[35+i+12],HWND_TOP,5+(i+12)*(window_w-10)/25,300,(window_w-10)/25,45,0);
        }else if(polowa==1){
            if(i<0) continue;
            SetWindowPos(hctrl[35+i+12],HWND_TOP,5+i*(window_w-10)/13,300,(window_w-10)/13,45,0);
        }else if(polowa==2){
            if(i<=0) continue;
            SetWindowPos(hctrl[35+i+12],HWND_TOP,5+(i-1)*(window_w-10)/12,300,(window_w-10)/12,45,0);
        }else if(polowa==-1){
            if(i>0) continue;
            SetWindowPos(hctrl[35+i+12],HWND_TOP,5+(i+12)*(window_w-10)/13,300,(window_w-10)/13,45,0);
        }
    }
    //tekst przycisku
    odp_b_text_refresh();
}

void App::odp_b_text_refresh(){
    for(int i=-12; i<=12; i++){
        //tekst przycisku
        ss_clear(ss);
        if(config->ustawienia_type<=TYP_INTERWAL){ //interwa³y
            if(i>0) ss<<"+";
            ss<<i;
            if(sound_0>=0){
                if(config->ustawienia_type<=TYP_INTERWAL_RM){ //rosn¹cy i/lub malej¹cy
                    ss<<"\r\n";
                    ss<<note_name(sound_0+i);
                }
            }
        }else if(config->ustawienia_type==TYP_DZWIEK||config->ustawienia_type==TYP_AKORD_D){
            if(i<=0) continue;
            ss<<sound_names[i-1];
        }else if(config->ustawienia_type==TYP_AKORD_M){
            if(i<=0) continue;
            ss<<sound_names[i+11];
        }
        SetWindowText(hctrl[35+i+12],ss.str().c_str());
    }
}

void App::interval_type_set(int type){
    config->ustawienia_type = type;
    sound_0 = -1;
    if(config->ustawienia_tryb==TRYB_NAUKA){
        nowy_interwal();
    }
    interval_or_chord();
}

void App::nowy_interwal(){
    sound_0 = -1;
    config->stat_odpowiedziany = false;
    config->stat_odgadniety = false;
    config->stat_przesluchania = 0;
    if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
        //lista dostêpnych dŸwiêków
        vector<int> dzwieki;
        for(int oktawa=0; oktawa<7; oktawa++){ //dla ka¿dej w³¹czonej oktawy
            if(!config->ustawienia_oktawy[oktawa]) continue;
            for(int j=0; j<12; j++){ //dla ka¿dego pó³tonu z oktawy
                dzwieki.push_back(note_C(oktawa+1)+j);
            }
        }
        if(dzwieki.size()==0){
            echo("B³¹d: pusty zakres dŸwiêków.");
            return;
        }
        //losowanie pierwszego dŸwiêku
        sound_0 = dzwieki[rand()%dzwieki.size()];
        //lista dostêpnych interwa³ów
        vector<int> interwaly;
        for(int i=0; i<12; i++){
            if(config->ustawienia_interwaly[i])
                interwaly.push_back(i+1);
        }
        if(interwaly.size()==0){
            echo("B³¹d: brak wybranych interwa³ów.");
            return;
        }
        //losowanie interwa³u
        sound_interval = interwaly[rand()%interwaly.size()];
        //drugi dŸwiêk
        if(config->ustawienia_type==TYP_INTERWAL_R || config->ustawienia_type==TYP_INTERWAL_H){ //rosn¹cy lub harmoniczny
            sound_1 = sound_0 + sound_interval;
        }else if(config->ustawienia_type==TYP_INTERWAL_M){ //malej¹cy
            sound_1 = sound_0 - sound_interval;
        }else if(config->ustawienia_type==TYP_INTERWAL_RM){ //rosn¹cy lub malej¹cy
            if(rand()%2==0){
                sound_1 = sound_0 + sound_interval;
            }else{
                sound_1 = sound_0 - sound_interval;
            }
        }
        //aktualizacja podpisów przycisków odpowiedzi
        odp_b_text_refresh();
    }else if(config->ustawienia_type==TYP_DZWIEK || config->ustawienia_type==TYP_AKORD_D || config->ustawienia_type==TYP_AKORD_M){ //pojedynczy dŸwiêk, akord durowy, akord molowy
        //lista dostêpnych dŸwiêków
        vector<int> dzwieki;
        for(int oktawa=0; oktawa<7; oktawa++){ //dla ka¿dej w³¹czonej oktawy
            if(!config->ustawienia_oktawy[oktawa]) continue;
            for(int j=0; j<12; j++){ //dla ka¿dego wybranego dŸwiêku z oktawy
                if(config->ustawienia_interwaly[j])
                dzwieki.push_back(note_C(oktawa+1)+j);
            }
        }
        if(dzwieki.size()==0){
            echo("B³¹d: pusty zbiór dŸwiêków.");
            return;
        }
        //losowanie dŸwiêku
        sound_0 = dzwieki[rand()%dzwieki.size()];
    }
    if(config->ustawienia_tryb==TRYB_ROZPOZNAWANIE){ //rozpoznawanie
        play_interval();
        config->stat_przesluchania=1;
        if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
            echo("Wygenerowano nowy interwa³.");
        }else if(config->ustawienia_type==TYP_DZWIEK){ //pojedynczy dŸwiêk
            echo("Wygenerowano nowy dŸwiêk.");
        }else if(config->ustawienia_type>=TYP_AKORD){
            echo("Wygenerowano nowy akord.");
        }
    }else{ //nauka
        ss_clear(ss);
        if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
            ss<<"Nowy dŸwiêk podstawowy: "<<note_name(sound_0);
        }else if(config->ustawienia_type==TYP_DZWIEK || config->ustawienia_type==TYP_AKORD_D || config->ustawienia_type==TYP_AKORD_M){
            ss<<"Nowa oktawa: "<<note_octave(sound_0);
        }
        echo(ss.str());
    }
    stat_refresh();
}

void App::powtorz_interwal(){
    if(config->ustawienia_tryb==TRYB_NAUKA) return;
    if(!config->stat_odpowiedziany){
        config->stat_przesluchania++;
        stat_refresh();
    }
    if(config->ustawienia_type<=TYP_INTERWAL){
        if(sound_0<0){
            echo("B³¹d: brak interwa³u");
            return;
        }
        echo("Powtórzono interwa³.");
    }else if(config->ustawienia_type==TYP_DZWIEK){
        if(sound_0<0){
            echo("B³¹d: brak dŸwiêku podstawowego");
            return;
        }
        echo("Powtórzono dŸwiêk.");
    }else if(config->ustawienia_type>=TYP_AKORD){
        if(sound_0<0){
            echo("B³¹d: brak dŸwiêku podstawowego");
            return;
        }
        echo("Powtórzono akord.");
    }
    play_interval();
}

void App::pokaz_interwal(){
    if(config->ustawienia_tryb==TRYB_NAUKA) return;
    if(!config->stat_odpowiedziany){
        config->stat_odpowiedziany = true;
        config->stat_odpowiedzi++;
        config->stat_bledne++;
        stat_refresh();
    }
    if(sound_0<0){
        echo("B³¹d: brak dŸwiêku podstawowego");
        return;
    }
    ss_clear(ss);
    if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
        ss<<"OdpowiedŸ: Interwa³ ";
        if(sound_1-sound_0>0){
            ss<<"+";
        }
        ss<<sound_1-sound_0;
        ss<<" ("<<interval_names[sound_interval]<<": "<<note_name(sound_0)<<" - "<<note_name(sound_1)<<")";
    }else if(config->ustawienia_type==TYP_DZWIEK){
        ss<<"OdpowiedŸ: DŸwiêk "<<note_name(sound_0);
    }else if(config->ustawienia_type==TYP_AKORD_D){
        ss<<"OdpowiedŸ: Akord "<<sound_names[note_semitone(sound_0)];
    }else if(config->ustawienia_type==TYP_AKORD_M){
        ss<<"OdpowiedŸ: Akord "<<sound_names[note_semitone(sound_0)+12];
    }
    echo(ss.str());
}

void App::dalej_interwal(){
    if(config->ustawienia_tryb==TRYB_NAUKA) return;
    if(sound_0<0 || (config->stat_odpowiedziany && config->stat_odgadniety)){
        nowy_interwal();
    }else{
        powtorz_interwal();
    }
}

void App::odp_interwal(int odp){
    if(config->ustawienia_tryb==TRYB_NAUKA){ //nauka
        if(config->ustawienia_type<=TYP_INTERWAL){
            if(sound_0<0){
                echo("B³¹d: brak dŸwiêku podstawowego. Wylosuj nowy.");
                return;
            }
            sound_1 = sound_0 + odp;
        }else if(config->ustawienia_type==TYP_DZWIEK || config->ustawienia_type==TYP_AKORD_D || config->ustawienia_type==TYP_AKORD_M){
            if(sound_0<0){
                echo("B³¹d: Wylosuj now¹ oktawê.");
                return;
            }
            sound_0 = note_C(note_octave(sound_0)) + odp-1;
        }
        play_interval();
    }else{ //rozpoznawanie
        if(sound_0<0){
            echo("B³¹d: brak dŸwiêku podstawowego");
            return;
        }
        bool odpowiedz = false;
        if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
            //poprawna odpowiedŸ
            if(odp == sound_1-sound_0){
                odpowiedz = true;
            }else{
                odpowiedz = false;
            }
        }else if(config->ustawienia_type==TYP_DZWIEK || config->ustawienia_type==TYP_AKORD_D || config->ustawienia_type==TYP_AKORD_M){
            //poprawna odpowiedŸ
            if(odp-1 == note_semitone(sound_0)){
                odpowiedz = true;
            }else{
                odpowiedz = false;
            }
        }
        if(!config->stat_odpowiedziany){
            config->stat_odpowiedzi++;
            if(!odpowiedz){
                config->stat_bledne++;
            }
        }
        config->stat_odpowiedziany = true;
        if(!config->stat_odgadniety)
            config->stat_odgadniety = odpowiedz;
        stat_refresh();
        ss_clear(ss);
        if(odpowiedz){
            ss<<"Poprawna odpowiedŸ!";
            color = 1;
        }else{
            ss<<"Z³a odpowiedŸ.";
            color = 2;
        }
        echo(ss.str(),color);
    }
}

void App::play_interval(){
    if(config->ustawienia_type<=TYP_INTERWAL){ //dla interwa³ów
        midi_play_note(sound_0);
        if(config->ustawienia_type<=TYP_INTERWAL_RM){ //rosn¹cy i/lub malej¹cy
            SetTimer(hwnd, 1, config->midi_pause, NULL);
        }else{ //harmoniczny
            midi_play_note(sound_1);
        }
    }else if(config->ustawienia_type==TYP_DZWIEK){
        midi_play_note(sound_0);
    }else if(config->ustawienia_type==TYP_AKORD_D){
        midi_play_note(sound_0);
        midi_play_note(sound_0+4);
        midi_play_note(sound_0+7);
        midi_play_note(sound_0+12); //z oktaw¹
    }else if(config->ustawienia_type==TYP_AKORD_M){
        midi_play_note(sound_0);
        midi_play_note(sound_0+3);
        midi_play_note(sound_0+7);
        midi_play_note(sound_0+12);
    }
}

void App::stat_refresh(){
    ss_clear(ss);
    ss<<"Odpowiedzi: "<<config->stat_odpowiedzi;
    SetWindowText(hctrl[67],ss.str().c_str());
    ss_clear(ss);
    ss<<"B³êdne: "<<config->stat_bledne;
    SetWindowText(hctrl[68],ss.str().c_str());
    ss_clear(ss);
    if(config->stat_odpowiedzi>0){
        ss<<"Poprawne: "<<(config->stat_odpowiedzi-config->stat_bledne)*100/config->stat_odpowiedzi<<"%";
    }
    SetWindowText(hctrl[69],ss.str().c_str());
    ss_clear(ss);
    ss<<"Liczba przes³uchañ: "<<config->stat_przesluchania;
    SetWindowText(hctrl[70],ss.str().c_str());
}

void App::stat_reset(){
    config->stat_odpowiedziany = false;
    config->stat_odgadniety = false;
    config->stat_przesluchania = 0;
    config->stat_odpowiedzi = 0;
    config->stat_bledne = 0;
    stat_refresh();
}
