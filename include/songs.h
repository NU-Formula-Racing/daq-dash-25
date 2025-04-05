#ifndef __SONGS_H__
#define __SONGS_H__

#include "sound.h"

static const std::vector<Note> cMajor = {
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_D, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_E, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_F, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_G, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER, 1),
    Note(NotePitch::N_B, NoteDuration::ND_QUARTER, 1),
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER, 1),
    Note(NotePitch::N_B, NoteDuration::ND_QUARTER, 1),
    Note(NotePitch::N_A, NoteDuration::ND_QUARTER, 1),
    Note(NotePitch::N_G, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_F, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_E, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_D, NoteDuration::ND_QUARTER),
    Note(NotePitch::N_C, NoteDuration::ND_QUARTER),
};

#endif  // __SONGS_H__