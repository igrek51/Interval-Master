#include "app.h"

HMIDIOUT* App::midi_init(){
	log("inicjalizacja MIDI...");
	HMIDIOUT *midi_device = new HMIDIOUT;
	int rc = midiOutOpen(midi_device, 0, 0, 0, CALLBACK_NULL);
    if(rc!=MMSYSERR_NOERROR){
        echo("B³¹d inicjalizacji MIDI");
        return NULL;
    }
    int CHANNEL = 1;
    DWORD command = (0x000000C0|CHANNEL) | (config->midi_instrument<<8);
    midiOutShortMsg(*midi_device, command);
    return midi_device;
}

void App::midi_play_note(int note){
	midiOutShortMsg(*midi_device,0x00000091|((note)<<8)|((config->midi_volume*127/100)<<16));
}

void App::midi_play_chord(int note, int chord_type){
	midiOutReset(*midi_device);
	midi_play_note(note);
	if(chord_type==1) return; //pojedynczy dŸwiêk
	midi_play_note(note+7);
	if(chord_type==2){ //power chords
		midi_play_note(note+12);
	}else if(chord_type==3){ //molowy
		midi_play_note(note+3);
	}else if(chord_type==4){ //durowy
		midi_play_note(note+4);
	}else if(chord_type==5){ //molowy septymowy
		midi_play_note(note+3);
		midi_play_note(note+10);
	}else if(chord_type==6){ //durowy septymowy
		midi_play_note(note+4);
		midi_play_note(note+10);
	}
}

void App::midi_close(HMIDIOUT *midi_device){
	midiOutReset(*midi_device);
	midiOutClose(*midi_device);
}
