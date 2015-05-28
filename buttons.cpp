#include "app.h"
#include <commctrl.h>

void App::button_click(WPARAM wParam){
    if(wParam>=11 && wParam<=12){
        select_button_tryb(wParam-11);
    }else if(wParam>=14 && wParam<=20){
        select_button_type(wParam-14);
    }else if(wParam>=23 && wParam<=34){
        select_button_interwaly(wParam-23);
    }else if(wParam>=73 && wParam<=79){
        select_button_oktawy(wParam-73);
    }else if(wParam==60){
        nowy_interwal();
    }else if(wParam==61){
        powtorz_interwal();
    }else if(wParam==62){
        pokaz_interwal();
    }else if(wParam==63){
        dalej_interwal();
    }else if(wParam==64){
        config_open();
    }else if(wParam==80){
        stat_reset();
    }else if(wParam>=35 && wParam<=59){
        odp_interwal(wParam-47);
    }
}

HWND App::create_button(string text, int x, int y, int w, int h, int button_nr){
    return CreateWindowEx(0,WC_BUTTON,text.c_str(),WS_CHILD|WS_VISIBLE,x,y,w,h,hwnd,(HMENU)button_nr,*hInst,0);
}

HWND App::create_button_multiline(string text, int x, int y, int w, int h, int button_nr){
    return CreateWindowEx(0,WC_BUTTON,text.c_str(),WS_CHILD|WS_VISIBLE|BS_MULTILINE,x,y,w,h,hwnd,(HMENU)button_nr,*hInst,0);
}

HWND App::create_edit(string text, int x, int y, int w, int h, int add_style){
    return CreateWindowEx(WS_EX_CLIENTEDGE,WC_EDIT,text.c_str(),WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|add_style,x,y,w,h,hwnd,0,*hInst,0);
}

HWND App::create_static(string text, int x, int y, int w, int h, int add_style){
    return CreateWindowEx(0,WC_STATIC,text.c_str(),WS_CHILD|WS_VISIBLE|add_style,x,y,w,h,hwnd,0,*hInst,0);
}

HWND App::create_groupbox(string text, int x, int y, int w, int h){
    return CreateWindowEx(0,WC_BUTTON,text.c_str(),WS_CHILD|WS_VISIBLE|BS_GROUPBOX,x,y,w,h,hwnd,(HMENU)0,*hInst,0);
}

HWND App::create_checkbox(string text, int x, int y, int w, int h, int button_nr){
    return CreateWindowEx(0,WC_BUTTON,text.c_str(),WS_CHILD|WS_VISIBLE|BS_CHECKBOX,x,y,w,h,hwnd,(HMENU)button_nr,*hInst,0);
}

HWND App::create_radiobutton(string text, int x, int y, int w, int h, int button_nr){
    return CreateWindowEx(0,WC_BUTTON,text.c_str(),WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,x,y,w,h,hwnd,(HMENU)button_nr,*hInst,0);
}

void App::select_button(int button, int button_offset, int button_nr){
    int checked=BST_UNCHECKED;
    for(int i=0; i<button_nr; i++){
        if(i==button) checked = BST_CHECKED;
        else checked=BST_UNCHECKED;
        SendMessage(hctrl[button_offset+i],BM_SETCHECK,checked,0);
    }
}

void App::select_button_tryb(int button){
    config->ustawienia_tryb=button;
    sound_0 = -1;
    if(config->ustawienia_tryb==TRYB_NAUKA){
        //wybierz wszystkie interwały
        /*
        for(int i=0; i<12; i++){
            select_button_interwaly(i,1);
        }
        */
        nowy_interwal();
    }
    select_button(button,11,2);
}

void App::select_button_type(int button){
    config->ustawienia_type=button;
    select_button(button,14,7);
    interval_type_set(config->ustawienia_type);
}

void App::select_button_interwaly(int button, int value){
    if(value==-1){
        config->ustawienia_interwaly[button]=!config->ustawienia_interwaly[button];
    }else if(value==0){
        config->ustawienia_interwaly[button] = false;
    }else if(value==1){
        config->ustawienia_interwaly[button] = true;
    }
    int checked=BST_UNCHECKED;
    if(config->ustawienia_interwaly[button]) checked = BST_CHECKED;
    SendMessage(hctrl[23+button],BM_SETCHECK,checked,0);
    sound_0 = -1;
    odp_b_refresh();
    if(config->ustawienia_tryb==TRYB_NAUKA){
        nowy_interwal();
    }
}

void App::select_button_oktawy(int button, int value){
    if(value==-1){
        config->ustawienia_oktawy[button]=!config->ustawienia_oktawy[button];
    }else if(value==0){
        config->ustawienia_oktawy[button] = false;
    }else if(value==1){
        config->ustawienia_oktawy[button] = true;
    }
    int checked=BST_UNCHECKED;
    if(config->ustawienia_oktawy[button]) checked = BST_CHECKED;
    SendMessage(hctrl[73+button],BM_SETCHECK,checked,0);
}

void App::change_font(HWND hwdp){
	if(hwdp==NULL) return;
    HFONT hFont=CreateFont(fontsize,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,0,0,0,0,fontface.c_str());
	SendMessage(hwdp,WM_SETFONT,(WPARAM)hFont,true);
}
