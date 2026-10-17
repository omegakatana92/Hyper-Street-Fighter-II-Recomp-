// SPDX-License-Identifier: GPL-3.0-only
//
// Hyper Street Fighter II: cheats and free play. Optional features (the launcher's Features page and the
// in-game menu switch them); they work on the game's memory after every frame. With every feature off the
// game runs exactly as the board does.
#include "hsf2.h"

#include "state.h"

namespace cps2recomp {

namespace {
    using namespace hsf2;

    // A round is being fought when both players' health is a sensible amount: outside a round (and in the
    // attract show) the game keeps 0 there, and a beaten player's reads 0xFF, which is what tells the cheats
    // not to touch it.
    bool Fighting() {
        for (int n = 0; n < 2; n++) {
            const uint8_t health = Ram8(Player(n) + kHealth);
            if (health == 0 || health == 0xFF) {
                return false;
            }
        }
        return true;
    }

    // What the round began with: the full health of each player (learned the first frames it is seen), and
    // the clock the round began at. 0: no round has been seen yet.
    uint8_t healthAtStart[2] = {0, 0};
    uint16_t clockAtStart = 0;

    void ForgetRound() {
        healthAtStart[0] = healthAtStart[1] = 0;
        clockAtStart = 0;
    }

    void Frame(Machine &) {
        // (two credits: a second player can always join. It is what the title waits for, so the attract
        // show gives way to it, as on a board with free play set.)
        if (FeatureOn("free_play") && Ram8(kCredits) < kCreditsFree) {
            SetRam8(kCredits, kCreditsFree);
        }
        if (!Fighting()) {
            ForgetRound();
            return;
        }
        // (learn the round's full health while the bars are still up)
        for (int n = 0; n < 2; n++) {
            const uint8_t health = Ram8(Player(n) + kHealth);
            if (health > healthAtStart[n]) {
                healthAtStart[n] = health;
            }
        }
        if (FeatureOn("infinite_time")) {
            // The clock stands still: keep it at what it read when this round began.
            if (clockAtStart == 0) {
                clockAtStart = Ram16(kClock);
            }
            if (clockAtStart != 0) {
                SetRam16(kClock, clockAtStart);
            }
        }
        static const char *const kNoDamage[2] = {"no_damage", "no_damage_2"};
        for (int n = 0; n < 2; n++) {
            if (FeatureOn(kNoDamage[n]) && healthAtStart[n] != 0) {
                // Put the health back where the round began, both the value and what is drawn.
                SetRam8(Player(n) + kHealth, healthAtStart[n]);
                SetRam8(Player(n) + kHealthShown, healthAtStart[n]);
            }
        }
        // One-hit knockouts: the opponent (player 2) is left with a single point of health, so any hit
        // ends the round. (A hit does at least one.)
        if (FeatureOn("one_hit")) {
            SetRam8(Player(1) + kHealth, 1);
            SetRam8(Player(1) + kHealthShown, 1);
        }
        // The LEVEL (SUPER) bars: both players' bars are held full, so a super is always ready.
        if (FeatureOn("full_gauge")) {
            for (int n = 0; n < 2; n++) {
                SetRam8(Player(n) + kGauge, kGaugeFull);
            }
        }
    }
} // namespace

void CheatsFrame(Machine &machine) {
    Frame(machine);
}

void CheatsState(Archive &ar) {
    ar.Pod(healthAtStart[0]);
    ar.Pod(healthAtStart[1]);
    ar.Pod(clockAtStart);
}

void RegisterCheats() {
    Game &game = TheGame();
    game.features.push_back({"free_play", "Game", "Free play", "Start works without a coin.", kFeatureEnhanced});
    game.features.push_back({"no_damage", "Cheats", "No damage", "Player 1's health does not go down."});
    game.features.push_back(
        {"no_damage_2", "Cheats", "No damage, player 2", "Player 2's health does not go down (the opponent's, in a one-player game)."});
    game.features.push_back({"infinite_time", "Cheats", "Infinite time", "The round's clock stands still."});
    game.features.push_back(
        {"one_hit", "Cheats", "One-hit knockouts", "Player 2 (the opponent, in a one-player game) goes down with the first hit."});
    game.features.push_back({"full_gauge", "Cheats", "Full SUPER bars",
                             "Both players' LEVEL (SUPER) bars are held full, so a super is always ready."});
}

} // namespace cps2recomp
