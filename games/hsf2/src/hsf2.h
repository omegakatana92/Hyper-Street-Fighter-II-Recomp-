// SPDX-License-Identifier: GPL-3.0-only
//
// Hyper Street Fighter II: The Anniversary Edition (USA 040202): names for what is known of the game's
// memory. Every address here was found by experiment on the running board with the toolkit's own tools
// (--menu, --movie, --dump-ram, --poke, --trace) and checked against two independent cheat collections for
// the same ROM set (MAME's and FinalBurn Neo's), as PROGRESS.md tells in full. No game data is written down
// here, only where the game keeps it.
#pragma once

#include "game.h"

#include <cstdint>

namespace hsf2 {

// The credits the game is holding, a byte: the title shows it as "CREDIT : n", and two coins make one
// credit (this game's coinage, unlike Street Fighter Alpha's one coin, one credit). The title waits for one
// of them before it offers "PRESS 1P OR 2P START". Found by holding a byte at 9 (--poke) and watching the
// title change from "INSERT 2 COINS" to "PRESS START" (PROGRESS.md).
constexpr uint32_t kCredits = 0xFF830A;
constexpr uint8_t kCreditsFree = 2; // (enough for both players to start)

// A player's record: the two are 0x400 apart, player 1's first.
//   +0x27   the health, a byte (0x90 at the round's start: what a hit takes bits off)
//   +0x1B9  the same health again, drawn down a step at a time: what the bars are drawn from
// Both are 0 outside a round, and in the attract show; a beaten player's read as 0xFF.
// Found with --trace over a match: both bytes fall together as a player is hit, and the second one trails
// the first as the bar is drawn down (PROGRESS.md).
constexpr uint32_t kPlayer0 = 0xFF8340, kPlayerStride = 0x400;
constexpr uint32_t kHealth = 0x27, kHealthShown = 0x1B9;
inline constexpr uint32_t Player(int n) {
    return kPlayer0 + static_cast<uint32_t>(n) * kPlayerStride;
}

// A player's place across the stage, a word (larger is further right): about 0x0128 to 0x0254 over a match,
// and the two players' records hold values the same distance apart when they stand nose to nose. Found with
// --trace over a movie that walks and jumps, and checked by holding the word at 0x0300 (--poke): the
// character moved to the right of the other. Which way a player faces is taken from these two places (the
// further one faces left), so no facing flag of its own is needed (PROGRESS.md).
constexpr uint32_t kPositionX = 0x02;

// The LEVEL (SUPER) bar at the bottom of the picture, a byte: holding it at 0x30 turns the bar full and the
// word "SUPER" lights beside it, so a super is always ready. Found by holding each byte of the record until
// the bar filled (--poke), the address then agreeing with both cheat collections (PROGRESS.md).
constexpr uint32_t kGauge = 0x2B0;
constexpr uint8_t kGaugeFull = 0x30;

// The round's clock, a word: its high byte counts down in BCD (0x99 is "99") and its low byte carries the
// rest of the countdown. Holding the word at what it read when the round began stops the clock, which is
// what the two cheat collections do with the high byte alone (0x99). 0 outside a round. Found by holding the
// byte at 0x99 and watching the clock stand still on "99" (--poke), the address having first been found by
// --trace of a round's countdown (PROGRESS.md).
constexpr uint32_t kClock = 0xFF8BFC;
constexpr uint16_t kClockFull = 0x9900; // (BCD: ninety-nine)

} // namespace hsf2
