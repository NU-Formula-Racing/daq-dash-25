#include <Arduino.h>

#include <algorithm>

#include "define.h"
#include "stdint.h"

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

// Define the NotePitch enum, including enharmonic duplicates.
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
    N_PITCH_COUNT
};

// Static lookup table for frequencies (in Hz) based on A4 = 440 Hz.
// These values are for the 4th octave.
static const std::array<float, N_PITCH_COUNT> __lutPitch = {
    261.63f,  // C4
    277.18f,  // C#4
    277.18f,  // Db4 (same as C#4)
    293.66f,  // D4
    311.13f,  // D#4
    311.13f,  // Eb4 (same as D#4)
    329.63f,  // E4
    349.23f,  // F4
    369.99f,  // F#4
    369.99f,  // Gb4 (same as F#4)
    392.00f,  // G4
    415.30f,  // G#4
    415.30f,  // Ab4 (same as G#4)
    440.00f,  // A4
    466.16f,  // A#4
    466.16f,  // Bb4 (same as A#4)
    493.88f   // B4
};

class Note {
   public:
    NotePitch pitch;
    NoteDuration duration;
    int8_t octave;

    Note(NotePitch pitch, NoteDuration duration, int8_t octave = 0) : pitch(pitch), duration(duration), octave(octave) {}
};

struct DriverEvent {
    uint32_t time;
    uint8_t out;
};

class Song {
    uint16_t bpm;
    std::vector<Note> notes;

    void shift(int8_t steps) {
    }

    std::vector<DriverEvent> generateEvents() const {
    }
};

enum SoundDriverState {
    S_NOT_PLAYING,
    S_PLAYING
};

class SoundDriver {
   public:
    SoundDriver(uint8_t piezoInput) : _piezoInput(piezoInput), _timePlaying(0), _state(SoundDriverState::S_NOT_PLAYING) {}

    void initialize() {
        pinMode(_piezoInput, OUTPUT);
    }

    void setSong(Song song) {
        if (S_PLAYING) {
            return;
        }
    }

    SoundDriverState getState() const { return _state; }

    void playSound(bool forceReset = true) {
        if (forceReset) _timePlaying = 0;
        _state = SoundDriverState::S_PLAYING;

        // figure out what event we are using with a binary search

        _timePlaying += millis();
    };

   private:
    uint8_t _piezoInput;
    uint32_t _timePlaying;
    SoundDriverState _state;
    std::vector<DriverEvent> _events;
};