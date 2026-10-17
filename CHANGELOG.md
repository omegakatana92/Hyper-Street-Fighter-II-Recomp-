# Hyper Street Fighter II Recomp — changelog

## 0.1.0-beta.1

First build. The 68000 and Z80 programs of *Hyper Street Fighter II: The Anniversary Edition*
(USA 040202) are translated to C++ ahead of time and run on cps2recomp's own machine. The launcher,
the in-game menu, save states, run-ahead, the display options and every control are remappable.

The game's own features are in, from addresses found on the running board (each recorded in
[games/hsf2/PROGRESS.md](games/hsf2/PROGRESS.md) with how it was found):

- **Free play**, so Start works without a coin;
- the cheats: **No damage** for either player, **Infinite time**, **One-hit knockouts** and **Full
  SUPER bars**;
- the training mode's first aid: an **Input display** at each player's side, turned to the way they
  face.

Frame data, the combo readout, the dummy and the hit boxes are still to come; they wait on the state
that says what a player is doing. See [ROADMAP.md](ROADMAP.md).
