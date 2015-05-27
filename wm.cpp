#include "app.h"
#include <ctime>

void App::wm_create(HWND *window){
	hwnd = *window;
	//parametry
	get_argv();
	//log
	clear_file("log.txt");
	log("Hello World...");
    //ustawienia
    load_config();
    //inicjalizacja parametrów
    sound_0 = -1;
    sound_1 = -1;
    sound_interval = 0;
	//kontrolki
    int ustawienia_w = 145;
	int x_paint, y_paint;
	//komunikaty
    x_paint = ustawienia_w*3+14;
    hctrl[0]=create_groupbox("Komunikaty",x_paint,5,355,146);
    x_paint += 10;
    y_paint = 23;
	for(int i=0; i<8; i++){
        hctrl[i+1]=create_static("",x_paint,y_paint,340,15,0); y_paint+=15;
	}
	//ustawienia
	y_paint = 5;
	x_paint = 7;
    hctrl[9]=create_groupbox("Ustawienia",x_paint,y_paint,ustawienia_w*3,291); x_paint+=7; y_paint+=18;
	//tryb
    hctrl[10]=create_groupbox("Tryb",x_paint,y_paint,ustawienia_w-9,65); x_paint+=10; y_paint+=20;
    hctrl[11]=create_radiobutton("Nauka",x_paint,y_paint,ustawienia_w-23,18,11); y_paint+=20;
    hctrl[12]=create_radiobutton("Rozpoznawanie",x_paint,y_paint,ustawienia_w-23,18,12); x_paint-=10; y_paint+=27;
	//typ interwa³u
    hctrl[13]=create_groupbox("Typ interwa³u / dŸwiêku",x_paint,y_paint,ustawienia_w-9,184); x_paint+=10; y_paint+=20;
    hctrl[14]=create_radiobutton("interwa³ rosn¹cy",x_paint,y_paint,ustawienia_w-23,18,14); y_paint+=20;
    hctrl[15]=create_radiobutton("interwa³ malej¹cy",x_paint,y_paint,ustawienia_w-23,18,15); y_paint+=20;
    hctrl[16]=create_radiobutton("rosn¹cy lub malej¹cy",x_paint,y_paint,ustawienia_w-23,18,16); y_paint+=20;
    hctrl[17]=create_radiobutton("harmoniczny",x_paint,y_paint,ustawienia_w-23,18,17); y_paint+=20;
    hctrl[18]=create_radiobutton("pojedynczy dŸwiêk",x_paint,y_paint,ustawienia_w-23,18,18); y_paint+=20;
    hctrl[19]=create_radiobutton("akord durowy",x_paint,y_paint,ustawienia_w-23,18,19); y_paint+=20;
    hctrl[20]=create_radiobutton("akord molowy",x_paint,y_paint,ustawienia_w-23,18,20); y_paint+=20;
    hctrl[21]=create_radiobutton("durowy lub molowy",x_paint,y_paint,ustawienia_w-23,18,21); y_paint+=20;

    ShowWindow(hctrl[21],SW_HIDE);

	//wielkoœæ interwa³u
    x_paint += ustawienia_w-13; y_paint = 23;
    hctrl[22]=create_groupbox("Wielkoœæ interwa³u",x_paint,y_paint,ustawienia_w-9,265); x_paint+=10; y_paint+=20;
    hctrl[23]=create_checkbox("1 - sekunda ma³a",x_paint,y_paint,ustawienia_w-23,18,23); y_paint+=20;
    hctrl[24]=create_checkbox("2 - sekunda wielka",x_paint,y_paint,ustawienia_w-23,18,24); y_paint+=20;
    hctrl[25]=create_checkbox("3 - tercja ma³a",x_paint,y_paint,ustawienia_w-23,18,25); y_paint+=20;
    hctrl[26]=create_checkbox("4 - tercja wielka",x_paint,y_paint,ustawienia_w-23,18,26); y_paint+=20;
    hctrl[27]=create_checkbox("5 - kwarta czysta",x_paint,y_paint,ustawienia_w-23,18,27); y_paint+=20;
    hctrl[28]=create_checkbox("6 - tryton",x_paint,y_paint,ustawienia_w-23,18,28); y_paint+=20;
    hctrl[29]=create_checkbox("7 - kwinta czysta",x_paint,y_paint,ustawienia_w-23,18,29); y_paint+=20;
    hctrl[30]=create_checkbox("8 - seksta ma³a",x_paint,y_paint,ustawienia_w-23,18,30); y_paint+=20;
    hctrl[31]=create_checkbox("9 - seksta wielka",x_paint,y_paint,ustawienia_w-23,18,31); y_paint+=20;
    hctrl[32]=create_checkbox("10 - septyma ma³a",x_paint,y_paint,ustawienia_w-23,18,32); y_paint+=20;
    hctrl[33]=create_checkbox("11 - septyma wielka",x_paint,y_paint,ustawienia_w-23,18,33); y_paint+=20;
    hctrl[34]=create_checkbox("12 - oktawa czysta",x_paint,y_paint,ustawienia_w-23,18,34);
    //zakres dŸwiêków
    x_paint += ustawienia_w-13; y_paint = 23;
    hctrl[72]=create_groupbox("Zakres dŸwiêków",x_paint,y_paint,ustawienia_w-9,165); x_paint+=10; y_paint+=20;
    hctrl[73]=create_checkbox("oktawa 1 (C1 - H1)",x_paint,y_paint,ustawienia_w-23,18,73); y_paint+=20;
    hctrl[74]=create_checkbox("oktawa 2 (C2 - H2)",x_paint,y_paint,ustawienia_w-23,18,74); y_paint+=20;
    hctrl[75]=create_checkbox("oktawa 3 (C3 - H3)",x_paint,y_paint,ustawienia_w-23,18,75); y_paint+=20;
    hctrl[76]=create_checkbox("oktawa 4 (C4 - H4)",x_paint,y_paint,ustawienia_w-23,18,76); y_paint+=20;
    hctrl[77]=create_checkbox("oktawa 5 (C5 - H5)",x_paint,y_paint,ustawienia_w-23,18,77); y_paint+=20;
    hctrl[78]=create_checkbox("oktawa 6 (C6 - H6)",x_paint,y_paint,ustawienia_w-23,18,78); y_paint+=20;
    hctrl[79]=create_checkbox("oktawa 7 (C7 - H7)",x_paint,y_paint,ustawienia_w-23,18,79); y_paint+=20;
	//przyciski wyboru interwa³u
	x_paint = 5;
	y_paint = 300;
	int sb_w = 32;
	for(int i=-12; i<=12; i++){
		ss_clear(ss);
		if(i>0) ss<<"+";
		ss<<i;
        hctrl[35+i+12]=create_button_multiline(ss.str(),x_paint,y_paint,sb_w,45,35+i+12); x_paint+=sb_w;
	}
	//panel
    x_paint = ustawienia_w*3+14;
	y_paint = 155;
	int pb_h = 28;
    hctrl[60]=create_button("Nowy [F2]",x_paint,y_paint,196,pb_h,60); y_paint+=pb_h;
    hctrl[61]=create_button("Powtórz [F3]",x_paint,y_paint,196,pb_h,61); y_paint+=pb_h;
    hctrl[62]=create_button("Poka¿ [F4]",x_paint,y_paint,196,pb_h,62); y_paint+=pb_h;
    hctrl[63]=create_button("Ustawienia",x_paint,y_paint,196,pb_h,63); y_paint+=pb_h;
    x_paint+=8; y_paint+=7;
    hctrl[64]=create_radiobutton("akord durowy",x_paint,y_paint,90,15,64); x_paint+=95;
    hctrl[65]=create_radiobutton("akord molowy",x_paint,y_paint,90,15,65);
	//statystyki
    x_paint = ustawienia_w*3+14+196+4;
    hctrl[66]=create_groupbox("Statystyki",x_paint,155,155,141);
	x_paint+=10; y_paint = 173;
    hctrl[67]=create_static("Odpowiedzi: 10",x_paint,y_paint,127,15,0); y_paint+=15;
    hctrl[68]=create_static("B³êdne: 2",x_paint,y_paint,127,15,0); y_paint+=15;
    hctrl[69]=create_static("Poprawne: 80%",x_paint,y_paint,127,15,0); y_paint+=15;
    hctrl[70]=create_static("Liczba przes³uchañ: 0",x_paint,y_paint,127,15,0); y_paint+=15;
    hctrl[71]=create_static("Punkty: 1500",x_paint,y_paint,127,15,0); y_paint+=24;
    hctrl[80]=create_button("Resetuj statystyki",x_paint,y_paint,137,pb_h,80); y_paint+=pb_h;
	//czcionki
	fontsize=13;
	fontface="MS Shell Dlg 2";
	for(int i=0; i<ctrls_num; i++){
		change_font(hctrl[i]);
	}
    SetFocus(hwnd);
	//subclassing
    for(int i=0; i<=80; i++)
        subclass(i);
    //pierwsze ustawienia przycisków
    select_button_tryb(config->ustawienia_tryb);
    select_button_type(config->ustawienia_type);
    select_button_odp_akord(config->odp_akord);
    select_button_oktawy(2);
    select_button_oktawy(3);
    select_button_interwaly(0);
    select_button_interwaly(1);
    select_button_interwaly(2);
    select_button_interwaly(3);
    select_button_interwaly(4);
	//midi init
    midi_device = midi_init();
    //inicjalizacja randomizera
    srand(time(0));
	echo("Uruchomiono program w wersji "+version);
}

void App::wm_command(WPARAM wParam){
	button_click(wParam);
}

void App::wm_timer(WPARAM wParam){
    if(wParam==1){
        if(sound_0>=0){
            midi_play_note(sound_1);
        }
        KillTimer(hwnd,1);
    }
}

void App::wm_keydown(WPARAM wParam){
    if(wParam==VK_F2){
        nowy_interwal();
    }else if(wParam==VK_F3){
        powtorz_interwal();
    }else if(wParam==VK_F4){
        pokaz_interwal();
    }
}

bool App::wm_colorstatic(WPARAM &wParam, LPARAM lParam){
	int ctrl=-1;
	for(int i=0; i<ctrls_num; i++){
		if((HWND)lParam==hctrl[i]){
			ctrl=i;
			break;
		}
	}
	if(ctrl==-1) return false;
	if(ctrl>=1&&ctrl<=8){
		SetBkMode((HDC)wParam,TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(config->komunikaty_c[(ctrl-1)*3],config->komunikaty_c[(ctrl-1)*3+1],config->komunikaty_c[(ctrl-1)*3+2]));
		return true;
	}
	return false;
}
