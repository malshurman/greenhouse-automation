#pragma once

#include <Notes.h>

enum SoundSetting {
    SOUND,
    SILENT
};

struct Melody {
    int notes[15];
    int tempo;
    int numberOfNotes;
};

void playMelody(const Melody melody);

const Melody THE_LICK = { {NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_G4,8, NOTE_E4,4, NOTE_C4,8, NOTE_D4,2}, 178, 7};
const Melody SWITCH_ON = { {NOTE_G3,16, NOTE_D4,8, REST,16, NOTE_G6,8}, 200, 4};
const Melody SWITCH_OFF = { {NOTE_G6,16, NOTE_D4,8, REST,16, NOTE_G3,8}, 200, 4};
const Melody BUTTON_ON = { {NOTE_E2,12, NOTE_G2,8}, 200, 2};
const Melody BUTTON_OFF = { {NOTE_G2,8, NOTE_E2,16}, 200, 2};
const Melody FEEDBACK = { {NOTE_E2,8}, 200, 1};
const Melody RESET = { {NOTE_AS1,16, NOTE_AS1,8}, 110, 2};