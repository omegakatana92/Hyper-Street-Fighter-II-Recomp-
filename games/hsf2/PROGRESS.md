# Hyper Street Fighter II: progress

What is known of *Hyper Street Fighter II: The Anniversary Edition* (USA 040202)'s memory, and what has
been built on it. Every address here was confirmed by experiment on the running board with the toolkit's
own tools (`docs/FEATURES.md`); several were first reached for from the two published cheat collections for
this same ROM set (MAME's and FinalBurn Neo's), which is said where it is so, and then each was checked on
the board itself. Nothing is a dump of the game's data, and no game data is written down.

## The ROM set

The set is MAME's **`hsf2`** (USA 040202), the parent of `hsf2a`, `hsf2j`, `hsf2j1` and `hsf2j2`. It was
told apart from the clones, and its files listed, from the set's own contents against MAME's ROM
definition for `hsf2` (Capcom, 2004, board `cps2_2p6b`, 2 players, 6 buttons):

- the program is eight 512 KiB ROMs — `hs2u.03`, `hs2u.04`, `hs2.05` … `hs2.10` — 4 MiB in all, so
  `code_end = 400000`;
- the graphics are four 8 MiB ROMs — `hs2.13m`, `hs2.15m`, `hs2.17m`, `hs2.19m` — side by side, and the
  graphics region is populated from 0 (no leading hole, unlike Street Fighter Alpha);
- the sound program is `hs2.01`, `hs2.02`; the QSound samples are `hs2.11m` (8 MiB);
- the key is `hsf2.key`.

Every file is checked by CRC-32 (`games/hsf2/game.cfg`), so only this revision is accepted.

## How the game's own memory is found

The tools used, in the order they are reached for (all in `docs/FEATURES.md`):

- `--menu FRAME:KEYS` drives the in-game menu with no window: a coin goes in without a hand.
- `--movie FILE` replays a hand-written input movie, and `--poke ADDR=BYTE` holds a byte at a guess; the
  two together are how the game is driven to a known place and a thought tried.
- `--dump-ram FRAME:FILE` writes the 64 KiB of work RAM after a frame; `--trace LOW-HIGH:FILE` appends a
  stretch of work RAM (or all of it) after every frame; `--shot-every N:PFX` writes pictures of the board.
- `cps2recomp/tools/ramfind.py` lists the addresses that held a known value in each of several dumps;
  `cps2recomp/tools/tracechanges.py` prints a trace where a value changes.
- The round's two digits are small in a screenshot and the health bars sit over them, so pictures were
  cropped and enlarged with a few lines of Python over the port's own PNG writer rather than read whole.

**Getting to a match.** The board boots into an attract show and a demo fight. Two coins make one credit
(this game's coinage), so a movie that presses Coin, Coin, then Start (`3800 0400`) reaches a real,
player-controlled match at about frame 5600. That movie, and a variant that then flails at random so hits
land, is what every trace below is taken over.

**The clock.** A `--trace` of the whole of work RAM over a match was searched for a byte counting down a
step at a time; `FF8BFC` answered, a BCD byte (`0x99` is "99") ticking down about every 30 frames on turbo
speed. Holding it with `--poke` at `0x99` stopped the digits on "99", which fixed it. MAME's collection
names the same address (and its neighbour beside it).

**The player records.** Dumping work RAM mid-round and outside a round showed a 0x400-byte record whose
bytes fall as a player is hit and read 0 outside a round; player 2's is the same record 0x400 on. The two
independent cheat collections put this game's health at `FF8367`/`FF8767`, which is `+0x27` in each record:
a `--trace` of those two bytes over a match showed the first falling 0x90 → 0x3D → 0x05 → 0xFF as player 1
was beaten while player 2's stood at 0x7B, and holding player 1's pair at 0 ended the round at once
(`ROUND 2` seen at a frame where the board was still in round 1).

**The health the bars are drawn from** is the same value again at `+0x1B9` (`FF84F9`/`FF88F9`): it trails
the first by a step or two as a hit is taken, which is the bar being drawn down. Both cheat collections set
both bytes together; holding both (as the cheats do) is what empties the bar.

**The LEVEL (SUPER) bar.** Holding the byte at `+0x2B0` (`FF85F0`/`FF89F0`) at `0x30` with `--poke` turned
the bar full and lit "SUPER" beside it, so a super is always ready. Both collections name the same address
and value.

**A player's place across the stage** is the word at `+0x02` (`FF8342`/`FF8742`): over a movie that walks
and jumps it ramps up and down with the walk, and holding it at `0x0300` moved the character to the right of
the other, which fixed both the address and its direction (larger is further right).

**Which way a player faces** is taken from those two places rather than from a flag byte of its own: a byte
at `+0x0E` that reads 1 for player 1 and 0 for player 2 at the round's start matched player 1's facing over a
7600-frame trace perfectly (1 exactly while its place was the smaller), but player 2's did not, so the two
places are the safer source. `hsf2.h` keeps only the places.

**The credits** are a byte at `FF830A`: holding it at 9 with `--poke` changed the title from "INSERT 2
COINS" to "PRESS START", and a movie that presses Start with **no** coin at all reaches a match once free
play holds it. FinalBurn Neo names the same address.

## The memory so far

| Address | Size | What | How it was found |
| --- | --- | --- | --- |
| `FF830A` | byte | **Credits**: what the title reads before it offers "PRESS START"; two coins make one credit | Holding it at 9 with `--poke` turned "INSERT 2 COINS" into "PRESS START"; a match then starts with no coin. Also in FinalBurn Neo's collection. |
| `FF8BFC` | word (high byte BCD) | **The round's clock**: the high byte counts down in BCD (`0x99` is "99") and the low byte carries the rest; 0 outside a round | `--trace`d work RAM over a match for a byte counting down; holding it at `0x99` stood the digits still. MAME's collection names the same address. |
| `FF8340` (+`FF8740`) | record | **Player 1 (player 2)'s record**, the two 0x400 apart | Found with the health bytes below: a 0x400-byte record whose bytes fall on a hit and read 0 outside a round. |
| `+0x27` (`FF8367`/`FF8767`) | byte | **Player 1 (player 2) health**: 0x90 at a round's start, steps down as hits land, 0xFF once beaten, 0 outside a round | `--trace`d a match: it fell 0x90 → 0x05 → 0xFF as player 1 was beaten while player 2's stood. A `--poke` of it to 0 ended the round at once. Both cheat collections name the address. |
| `+0x1B9` (`FF84F9`/`FF88F9`) | byte | The same health again: **what the bars are drawn from**, draining a step at a time | Trails the health byte on every hit; poking both to 0 empties the bar. Both cheat collections set both together. |
| `+0x2B0` (`FF85F0`/`FF89F0`) | byte | **The LEVEL (SUPER) bar**: `0x30` turns it full and lights "SUPER" | `--poke`d a byte at `0x30` and watched the bar fill; both cheat collections name the same address and value. |
| `+0x02` (`FF8342`/`FF8742`) | word | **Player 1 (player 2)'s place across the stage**: larger is further right | `--trace`d a walk-and-jump movie (the word ramps with the walk) and `--poke`d it to `0x0300` (the character moved to the other's right). |
| `+0x0E` (`FF834E`/`FF874E`) | byte | **Which way a player faces** (1: opponent to its right): a partial finding, matched for player 1 only | Compared it against the two records' places over a 7600-frame trace: perfect for player 1, not for player 2, so the sources use the places instead. |

The health bytes are 0 outside a round (and in the attract show) and 0xFF once a player is beaten, which is
what the cheats use to tell a round being fought from everything else.

## Features built on it (`src/`)

| Feature | Category | What it does |
| --- | --- | --- |
| `free_play` | Game (Enhanced) | Holds two credits, so Start works without a coin. It is what the title waits for, so the attract show gives way to it. |
| `no_damage` | Cheats | Puts player 1's health back to the round's full value every frame (`+0x27` and its drawn copy `+0x1B9`). |
| `no_damage_2` | Cheats | The same for player 2 (the opponent's, in a one-player game). |
| `infinite_time` | Cheats | Holds the round's clock word where it stood when the round began, so it never runs out. |
| `one_hit` | Cheats | Leaves the opponent (player 2) a single point of health, so any hit ends the round. |
| `full_gauge` | Cheats | Holds both players' LEVEL (SUPER) bars full (`+0x2B0`), so a super is always ready. |
| `input_display` | Training | What each player holds, and for how long, at their side of the picture: the toolkit's `fight::InputHistory`, fed from the port's own panel bits and the game's facing (taken from the two players' places across the stage), so the notation is turned to the player's side. |

Each was checked with no window against a run of the board, with `--trace` of the relevant bytes and
`--shot-every` pictures:

- `free_play`: a movie with **no** coin press reaches character select and a match (the title reads "PRESS
  START"), where without the feature it stays on the title.
- `no_damage`: over the random-play movie the health byte stays 0x90 all round, where the same movie without
  it drains it 0x40 → 0x05 → 0xFF.
- `no_damage_2`: the mirror run — player 1 drains, player 2 stays 0x90.
- `infinite_time`: the round's digits stand still on "99" while the board runs on.
- `one_hit`: player 2's health byte is held at 1 all round.
- `full_gauge`: "SUPER" lights at both players' feet and the LEVEL bars read full.
- `input_display`: the HUD shows each hold and for how long (e.g. `100 6`, `100 4`, `20 8` for player 1's
  right, left and up presses — turned so that "6" is always towards the way the player faces).

## Next

- **The rest of the player record.** The state word (what a player is doing: standing, walking, crouching,
  in the air, attacking, reeling) is not yet told apart; the toolkit's training aids want exactly that
  (`fight::StepView`) for the frame data, the combo readout, the dummy and the hit boxes, and the second
  thing they want is the attack boxes themselves. That is the next thing the training aids are built from.
- **The widescreen picture**: the picture is drawn from the board's own 384 columns; a wider picture needs
  the camera and the stage scrollers found and a `widescreen.cpp` written (the toolkit's `Video::wide`).
- A **rewrite of the damage rule** (`docs/READABLE.md`) for a cleaner `one_hit` than holding the health at
  one point, as the other ports have.
