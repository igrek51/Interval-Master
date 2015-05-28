#ifndef APP_H
#define APP_H

//#undef UNICODE
#include <windows.h>
#include <sstream>

#include "config.h"
#include "music.h"

using namespace std;

class App{
public:
	App(int w, int h, string version, int ctrls = 0);
	~App();
	HINSTANCE* hInst;
	int window_w, window_h;
	int ctrls_num;
	HWND hwnd;
	HWND *hctrl;
	string version;
public:
	//IO
	string *argv;
	int argc;
	stringstream ss;
	string echos[8];
    int color;
	void clear_file(string filename);
	void log(string l);
	void log(int l);
    void echo_show(string s);
    void echo(string s, int colorr=0);
	void echo(int e);
	void echo();
	bool file_exists(string name);
	void ss_clear(stringstream &sstream);
	void get_argv();
	bool is_arg(string par);
	void message_out(string m);
    void message_out(int m);

	//CONFIG
	void load_config();
    void config_open();
	Config *config;

	//WM
	void wm_create(HWND *window);
	void wm_command(WPARAM wParam);
	void wm_timer(WPARAM wParam);
    void wm_keydown(WPARAM wParam);
	bool wm_colorstatic(WPARAM &wParam, LPARAM lParam);

	//BUTTONS
	int fontsize;
	string fontface;
	HWND create_button(string text, int x, int y, int w, int h, int button_nr);
	HWND create_button_multiline(string text, int x, int y, int w, int h, int button_nr);
	HWND create_edit(string text, int x, int y, int w, int h, int add_style);
	HWND create_static(string text, int x, int y, int w, int h, int add_style);
	HWND create_groupbox(string text, int x, int y, int w, int h);
	HWND create_checkbox(string text, int x, int y, int w, int h, int button_nr);
	HWND create_radiobutton(string text, int x, int y, int w, int h, int button_nr);
	void button_click(WPARAM wParam);
	void change_font(HWND hwdp);

    void select_button(int button, int button_offset, int button_nr);
    void select_button_tryb(int button);
    void select_button_type(int button);
    void select_button_interwaly(int button, int value=-1);
    void select_button_oktawy(int button, int value=-1);

	//SUBCLASSING
	WNDPROC windowProc, wndproc_new, *wndproc_old;
	LRESULT CALLBACK subclass_wndproc_new(HWND hwdp, UINT message, WPARAM wParam, LPARAM lParam);
	void subclass(int ctrl);
	void un_subclass(int ctrl);

	//MIDI
	HMIDIOUT *midi_device;
	HMIDIOUT* midi_init();
    void midi_play_note(int note);
	void midi_close(HMIDIOUT *midi_device);

	//PROGRAM
    void interval_or_chord();
    void odp_b_refresh();
    void odp_b_text_refresh();
    void interval_type_set(int type);
    void nowy_interwal();
    void powtorz_interwal();
    void pokaz_interwal();
    void dalej_interwal();
    void odp_interwal(int odp);
    int sound_0, sound_1;
    int sound_interval;
    void play_interval();

    //STATYSTYKI
    void stat_refresh();
    void stat_reset();

};

#endif // APP_H
