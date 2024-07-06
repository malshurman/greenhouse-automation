#include <Arduino.h>
#include <SoundPlayer.h>
#include <Pins.h>

void playMelody(const Melody melody)
{
    const int* notes = melody.notes;
    int tempo = melody.tempo;
    int numberOfNotes = melody.numberOfNotes;
    int wholenote = (60000 * 4) / tempo;
    int divider = 0, noteDuration = 0;
    for (int thisNote = 0; thisNote < numberOfNotes * 2; thisNote = thisNote + 2) {
        divider = notes[thisNote + 1];
        if (divider > 0) {
            noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
            noteDuration = (wholenote) / abs(divider);
        }

        tone(SPEAKER, notes[thisNote], noteDuration*0.9);
        delay(noteDuration);
        noTone(SPEAKER);
    }
}