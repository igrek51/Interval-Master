#include "app.h"
#include "version.h"

using namespace std;

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

const char *progName = "Igrek Interval Master";

App *app = new App(810,352,VERSION,100);

LRESULT CALLBACK wndproc_new(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	return app->subclass_wndproc_new(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	app->hInst = &hInstance;
	app->windowProc = windowProc;
	app->wndproc_new = wndproc_new;
	WNDCLASS windowClass;
	windowClass.lpfnWndProc = windowProc;
	windowClass.style       = CS_HREDRAW | CS_VREDRAW;
	windowClass.hInstance   = hInstance;
	windowClass.hCursor     = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon       = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hbrBackground=(HBRUSH)COLOR_BTNSHADOW;
	windowClass.cbClsExtra  = 0;
	windowClass.cbWndExtra  = 0;
    windowClass.lpszClassName = progName;
	windowClass.lpszMenuName  = NULL;
	if(!RegisterClass(&windowClass)){
		app->message_out("Error: RegisterClass failed");
		return 0;
	}
    HWND window = CreateWindowEx(0,progName,progName,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,app->window_w+16,app->window_h+38,NULL,NULL,hInstance,NULL);
	if(!window){
		app->message_out("Error: window NULL pointer");
		return 0;
	}
	ShowWindow(window,nCmdShow);
	UpdateWindow(window);
	MSG message;
	while(GetMessage(&message, NULL, 0, 0)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
		case WM_CREATE:{
			app->wm_create(&hwnd);
		}break;
		case WM_COMMAND:{
			app->wm_command(wParam);
		}break;
		case WM_TIMER:{
			app->wm_timer(wParam);
		}break;
        case WM_KEYDOWN:{
            app->wm_keydown(wParam);
		}break;
		case WM_CTLCOLORSTATIC:{
			if(app->wm_colorstatic(wParam, lParam)){
				return (BOOL)GetSysColorBrush(COLOR_MENU);
			}
		}break;
		case WM_DESTROY:{
			delete app;
			return 0;
		}break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
