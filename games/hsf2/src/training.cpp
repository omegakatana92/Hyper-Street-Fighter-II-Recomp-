// SPDX-License-Identifier: GPL-3.0-only
//
// Hyper Street Fighter II: the training aids, put together from the toolkit's fight/ parts (fight/fight.h).
// The input display needs only what the port read off the panel and which way each player faces, so it is
// the first one; the others (frame data, the combo readout, the dummy, the hit boxes) wait on more of the
// game's own memory being known: the players' records are found (hsf2.h), with their health and places
// picked out, but what a player is doing is not yet told apart (PROGRESS.md).
#include "hsf2.h"

#include "fight.h"
#include "machine.h"
#include "state.h"

namespace cps2recomp {

namespace {
    using namespace hsf2;

    fight::InputHistory g_inputs;

    // A round is being fought when both players' health is a sensible amount (hsf2.h): outside a round the
    // record holds 0, and a beaten player's reads 0xFF.
    bool InRound() {
        for (int n = 0; n < 2; n++) {
            const uint8_t health = Ram8(Player(n) + kHealth);
            if (health == 0 || health == 0xFF) {
                return false;
            }
        }
        return true;
    }

    // Which way a player faces: the other is towards it when its place across the stage is the smaller (the
    // two records hold the same scale). It is only meaningful while a round is fought, so a player outside
    // one is taken to face right and the notation is left as it is.
    bool FacingLeft(int n) {
        if (!InRound()) {
            return false;
        }
        return Ram16(Player(n) + kPositionX) > Ram16(Player(1 - n) + kPositionX);
    }

    void ReadInputs(Machine &machine) {
        for (int n = 0; n < 2; n++) {
            g_inputs.Push(n, machine.players[n], FacingLeft(n));
        }
    }
} // namespace

void TrainingBeforeFrame(Machine &machine) {
    ReadInputs(machine);
}

void TrainingFrame(Machine &) {
    std::vector<HudLine> lines;
    if (FeatureOn("input_display")) {
        g_inputs.Hud(0, lines);
        g_inputs.Hud(1, lines);
    }
    SetHud(lines);
}

void TrainingState(Archive &ar) {
    // (fight::InputHistory's own frame counts are not part of the save state: a load starts the display over)
    if (!ar.Saving()) {
        g_inputs.Clear();
    }
}

void RegisterTraining() {
    Game &game = TheGame();
    game.features.push_back({"input_display", "Training", "Input display",
                             "What each player holds, and for how long, at their side of the picture."});
    TrainingAids({"input_display"});
}

} // namespace cps2recomp
