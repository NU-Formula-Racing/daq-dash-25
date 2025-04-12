#ifndef __SOUND_H__
#define __SOUND_H__

#include <Arduino.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

#include "define.h"  // Assuming additional definitions exist here

//--------------------------------------------------
// Note Duration & Lookup Table
//--------------------------------------------------

enum NoteDuration : int8_t {
    ND_WHOLE,
    ND_HALF,
    ND_QUARTER,
    ND_EIGHTH,
    ND_SIXTEENTH,
    ND_HALF_TRIPLET,
    ND_QUARTER_TRIPLET,
    ND_EIGHTH_TRIPLET,
    ND_SIXTEENTH_TRIPLET,
    ND_COUNT
};

static const std::array<float, ND_COUNT> __lutDuration = {
    1.0f,
    0.5f,
    0.25f,
    0.125f,
    0.0625f,
    1.0f / 3.0f,
    1.0f / 6.0f,
    1.0f / 12.0f,
    1.0f / 24.0f};

//--------------------------------------------------
// Note Pitch & Frequency Lookup Table
//--------------------------------------------------

// NotePitch enum includes enharmonic duplicates.
enum NotePitch : int8_t {
    N_C,   // C (semitone 0)
    N_CS,  // C# (semitone 1)
    N_DB,  // Db (semitone 1)
    N_D,   // D (semitone 2)
    N_DS,  // D# (semitone 3)
    N_EB,  // Eb (semitone 3)
    N_E,   // E (semitone 4)
    N_F,   // F (semitone 5)
    N_FS,  // F# (semitone 6)
    N_GB,  // Gb (semitone 6)
    N_G,   // G (semitone 7)
    N_GS,  // G# (semitone 8)
    N_AB,  // Ab (semitone 8)
    N_A,   // A (semitone 9) -> defined as 440 Hz
    N_AS,  // A# (semitone 10)
    N_BB,  // Bb (semitone 10)
    N_B,   // B (semitone 11)
    N_REST,
    N_PITCH_COUNT
};

// Frequencies (in Hz) for the 4th octave (A4 = 440Hz).
static const std::array<float, N_PITCH_COUNT> __lutPitch = {
    261.63f,  // C4
    277.18f,  // C#4
    277.18f,  // Db4
    293.66f,  // D4
    311.13f,  // D#4
    311.13f,  // Eb4
    329.63f,  // E4
    349.23f,  // F4
    369.99f,  // F#4
    369.99f,  // Gb4
    392.00f,  // G4
    415.30f,  // G#4
    415.30f,  // Ab4
    440.00f,  // A4
    466.16f,  // A#4
    466.16f,  // Bb4
    493.88f,   // B4
    0.0f // rest
};

//--------------------------------------------------
// Helper Functions for Pitch Transposition
//--------------------------------------------------

// Returns the canonical semitone (0-11) for a given NotePitch.
static int8_t getSemitone(NotePitch pitch) {
    switch (pitch) {
        case N_C:
            return 0;
        case N_CS:
            return 1;
        case N_DB:
            return 1;
        case N_D:
            return 2;
        case N_DS:
            return 3;
        case N_EB:
            return 3;
        case N_E:
            return 4;
        case N_F:
            return 5;
        case N_FS:
            return 6;
        case N_GB:
            return 6;
        case N_G:
            return 7;
        case N_GS:
            return 8;
        case N_AB:
            return 8;
        case N_A:
            return 9;
        case N_AS:
            return 10;
        case N_BB:
            return 10;
        case N_B:
            return 11;
        default:
            return 0;
    }
}

// Maps a semitone (0-11) to a canonical NotePitch.
// We choose the "sharp" names for accidentals.
static NotePitch semitoneToNotePitch(int8_t semitone) {
    switch (semitone) {
        case 0:
            return N_C;
        case 1:
            return N_CS;
        case 2:
            return N_D;
        case 3:
            return N_DS;
        case 4:
            return N_E;
        case 5:
            return N_F;
        case 6:
            return N_FS;
        case 7:
            return N_G;
        case 8:
            return N_GS;
        case 9:
            return N_A;
        case 10:
            return N_AS;
        case 11:
            return N_B;
        default:
            return N_C;
    }
}

//--------------------------------------------------
// Note Class
//--------------------------------------------------

class Note {
   public:
    NotePitch pitch;
    NoteDuration duration;
    int8_t octave;  // Relative to the 4th octave (0 means octave 4)

    Note(NotePitch pitch, NoteDuration duration, int8_t octave = 4)
        : pitch(pitch), duration(duration), octave(octave - 4) {}
};

//--------------------------------------------------
// Driver Event Structure
//--------------------------------------------------

struct DriverEvent {
    uint32_t time;    // Time in milliseconds from the song start
    uint16_t frequency; // the frequency
};

//--------------------------------------------------
// Song Class
//--------------------------------------------------

class Song {
   public:
    uint16_t bpm;             // Beats per minute
    std::vector<Note> notes;  // List of notes in the song

    Song(uint16_t bpm, const std::vector<Note> &notes) : bpm(bpm), notes(notes) {}

    // Transpose the song by a number of semitone steps.
    void shift(int8_t steps) {
        // Assume that octave 0 means octave 4 in absolute terms.
        // For each note, compute the absolute semitone index, apply the shift, and update.
        for (auto &note : notes) {
            // Compute absolute semitone index: base octave (4) + note.octave gives the actual octave.
            if (note.pitch == N_REST) continue;

            int absoluteOctave = 4 + note.octave;
            int globalSemitone = absoluteOctave * 12 + getSemitone(note.pitch);
            globalSemitone += steps;

            // Compute new octave and semitone.
            int newAbsoluteOctave = globalSemitone / 12;
            int8_t newSemitone = globalSemitone % 12;

            // Update note: store octave relative to 4.
            note.octave = newAbsoluteOctave - 4;
            note.pitch = semitoneToNotePitch(newSemitone);
        }
    }

    // Generate a list of DriverEvents from the song.
    // For simplicity, each note creates two events:
    // one to turn the note on (with a computed frequency) and one to turn it off (0 output).
    std::vector<DriverEvent> generateEvents() const {
        std::vector<DriverEvent> events;
        // Base duration calculations
        float quarterDurationMs = 60000.0f / bpm;
        float wholeDurationMs = 4 * quarterDurationMs;

        uint32_t currentTime = 0;

        for (const auto &note : notes) {
            float durationMs = wholeDurationMs * __lutDuration[note.duration];

            float baseFreq = __lutPitch[note.pitch];
            float freq = baseFreq * pow(2, note.octave);

            DriverEvent event = {
                .time = currentTime,
                .frequency = static_cast<uint16_t>(freq)
            };


            events.push_back(event);

            currentTime += static_cast<uint32_t>(durationMs);
        }

        DriverEvent final = {
            .time = currentTime,
            .frequency = 0
        };

        events.push_back(final);

        return events;
    }
};

//--------------------------------------------------
// Sound Driver Classes
//--------------------------------------------------

enum SoundDriverState {
    S_NOT_PLAYING,
    S_PLAYING
};

class SoundDriver {
   public:
    SoundDriver(uint8_t piezoInput)
        : _piezoInput(piezoInput), _startTime(0), _state(S_NOT_PLAYING) {}

    void initialize() {
        pinMode(_piezoInput, OUTPUT);        
    }

    // Set the current song (if not already playing) and load its events.
    void setSong(Song song) {
        if (_state == S_PLAYING) {
            Serial.println("Cannot set song, currently playing!");
            return;
        }
        Serial.println("Setting song!");
        _events = song.generateEvents();
        Serial.println("Done setting song!");
        // Reset time when a new song is loaded.
        _startTime = millis();
        _currentIndex = 0;
        _prevIndex = 1;
    }

    SoundDriverState getState() const { return _state; }

    // Call this method repeatedly (for example, in the Arduino loop())
    // to process scheduled events.
    void playSong() {
        _state = S_PLAYING;

        uint32_t elapsedTime = millis() - _startTime;

        bool over = false;
        while (elapsedTime > _events[_currentIndex].time) {
            _currentIndex++;
            if (_currentIndex == _events.size()) {
                over = true;
                break;
            }
        }

        if (_currentIndex != 0)
            _currentIndex--;


        if (_prevIndex == _currentIndex) {
            return; // we didn't change notes
        }

        _prevIndex = _currentIndex;
            
        DriverEvent event = _events[_currentIndex];
        
        // Serial.printf("Playing %d for %dms\n", event.frequency, event.time);
        
        if (!over) {
            // Serial.printf("Progressing to index %d, time %d, next %d, note %d\n", _currentIndex, elapsedTime, _events[_currentIndex + 1].time, event.frequency);
            // tone(_piezoInput, event.frequency);
            if (event.frequency == 0)
            {
                analogWrite(_piezoInput, 0);
                return;
            }

            analogWriteFrequency(_piezoInput, event.frequency);
            analogWrite(_piezoInput, 220);
        } else {
            // we are done with the song
            _state = S_NOT_PLAYING;
            // Serial.println("Done playing!");
            _startTime = 0;
            analogWrite(_piezoInput, 0);
        }
    }

   private:
    uint8_t _piezoInput;
    uint32_t _startTime;
    SoundDriverState _state;
    std::vector<DriverEvent> _events;
    size_t _currentIndex;
    size_t _prevIndex;
};

#endif // __SOUND_H__