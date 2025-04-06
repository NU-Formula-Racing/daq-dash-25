#ifndef __SONGS_H__
#define __SONGS_H__

#include "sound.h"

static const std::vector<Note> cMajor = {
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_D, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_E, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_F, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_G, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_B, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER, 5),
    Note(NotePitch::N_B, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_G, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_F, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_E, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_D, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER),
};

static const std::vector<Note> goU = {
    // line 1
    Note(NotePitch::N_E, NoteDuration::ND_WHOLE),
    Note(NotePitch::N_F, NoteDuration::ND_HALF),
    Note(NotePitch::N_FS, NoteDuration::ND_HALF),
    Note(NotePitch::N_G, NoteDuration::ND_WHOLE),
    Note(NotePitch::N_E, NoteDuration::ND_WHOLE, 5),
    Note(NotePitch::N_D, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_C, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_C, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER, 5),
    Note(NotePitch::N_G, NoteDuration::ND_WHOLE),
    Note(NotePitch::N_G, NoteDuration::ND_WHOLE),
    // line 2
    Note(NotePitch::N_A, NoteDuration::ND_HALF),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_B, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_C, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_A, NoteDuration::ND_HALF),
    Note(NotePitch::N_G, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_E, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_E, NoteDuration::ND_QUARTER, 5),
    Note(NotePitch::N_B, NoteDuration::ND_HALF),
    Note(NotePitch::N_C, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_D, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_A, NoteDuration::ND_HALF),
    Note(NotePitch::N_E, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_EB, NoteDuration::ND_HALF, 5),
    Note(NotePitch::N_D, NoteDuration::ND_QUARTER, 5),
    Note(NotePitch::N_REST, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_B, NoteDuration::ND_HALF),
    Note(NotePitch::N_A, NoteDuration::ND_HALF),
    Note(NotePitch::N_G, NoteDuration::ND_HALF),
    // line 3
};

#endif  // __SONGS_H__