#include <Arduino.h>
#include <SoundPlayer.h>

void playTheLick()
{
    playMelody(theLick);
}

void playSwitchOn()
{
    playMelody(switchOn);
}

void playSwitchOff()
{
    playMelody(switchOff);
}

void playButtonOn()
{
    playMelody(buttonOn);
}

void playButtonOff()
{
    playMelody(buttonOff);
}

void playReset()
{
    playMelody(reset);
}

int speaker;

void initializeSpeaker(int speakerPin)
{
    speaker = speakerPin;
}

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

    tone(speaker, notes[thisNote], noteDuration*0.9);
    delay(noteDuration);
    noTone(speaker);
    }
}