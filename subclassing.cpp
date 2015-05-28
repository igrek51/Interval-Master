#include "app.h"

LRESULT CALLBACK App::subclass_wndproc_new(HWND hwdp, UINT message, WPARAM wParam, LPARAM lParam){
	//rozpoznanie numeru kontrolki
	int ctrl=-1;
	for(int i=0; i<ctrls_num; i++){
		if(hwdp==hctrl[i]){
			ctrl=i;
			break;
		}
	}
	if(ctrl==-1) return 0;
	//nowe procedury kontrolek
    if(ctrl>=0 && ctrl<=80){
        switch(message){
            case WM_KEYDOWN:{
                if(wParam==VK_F2||wParam==VK_F3||wParam==VK_F4||wParam==VK_SPACE){
                    CallWindowProc(windowProc, hwnd, message, wParam, lParam); //przekazanie do głównego okna
                    return 0; //przechwycenie
                }
            }break;
        }
    }
	return CallWindowProc(wndproc_old[ctrl], hwdp, message, wParam, lParam);
}

void App::subclass(int ctrl){
    if(hctrl[ctrl]==NULL) return;
	wndproc_old[ctrl] = (WNDPROC) SetWindowLong(hctrl[ctrl],GWL_WNDPROC,(LONG)wndproc_new);
}

void App::un_subclass(int ctrl){
	if(wndproc_old[ctrl]==NULL) return;
	SetWindowLong(hctrl[ctrl],GWL_WNDPROC,(LONG)wndproc_old[ctrl]);
}
