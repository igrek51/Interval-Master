#include "music.h"
#include <sstream>

int note_C(int octave){
    int note = midi_C0;
    while(octave>0){
        octave--;
        note+=12;
    }
    while(octave<0){
        octave++;
        note-=12;
    }
    return note;
}

int note_H(int octave){
    int note = midi_C0+11;
    while(octave>0){
        octave--;
        note+=12;
    }
    while(octave<0){
        octave++;
        note-=12;
    }
    return note;
}

string note_name(int note){
    int diff_from_C0 = note-midi_C0;
    int octave = diff_from_C0/12;
    string sound = sound_names[diff_from_C0%12];
    stringstream ss2;
    ss2<<sound<<octave;
    return ss2.str();
}

int note_octave(int note){
    int diff_from_C0 = note-midi_C0;
    return diff_from_C0/12;
}

int note_semitone(int note){
    int diff_from_C0 = note-midi_C0;
    return diff_from_C0%12;
}
