#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>

using namespace std;

const string sound_names[48] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "B", "H",
    "c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "b", "h",
    "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "B7", "H7",
    "c7", "c#7", "d7", "d#7", "e7", "f7", "f#7", "g7", "g#7", "a7", "b7", "h7"};

const string interval_names[13] = {
    "pryma", "sekunda ma³a", "sekunda wielka", "tercja ma³a", "tercja wielka", "kwarta czysta",
    "tryton", "kwinta czysta", "seksta ma³a", "seksta wielka", "septyma ma³a", "septyma wielka", "oktawa czysta"};

const int midi_C0 = 12;

int note_C(int octave);
int note_H(int octave);
string note_name(int note);
int note_octave(int note);
int note_semitone(int note);

#endif
