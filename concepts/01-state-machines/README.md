# Concept 1: State machines, from if/else chains to switch/case

**Level:** intermediate · **Board:** Arduino Uno (any Arduino works) · **Time:** 45–60 minutes

## Goal

Write a device's behaviour as a **state machine**: a fixed set of states, the actions taken on entering each one, and the events that move it from one state to the next. Build the same traffic light twice, first as an `if/else` chain and then as a `switch/case` machine with an extra state and a button, and see why the second form keeps working as the device grows.

## Parts

- 4 LEDs (red, yellow, green, and white or blue for "walk") + 4 × 220 Ω resistors
- 1 push-button
- Breadboard and jumper wires

## Wiring

```
 D4 ─[220 Ω]─ RED ────┐
 D5 ─[220 Ω]─ YELLOW ─┤
 D6 ─[220 Ω]─ GREEN ──┼── GND
 D7 ─[220 Ω]─ WALK ───┘          (only used by the switch/case version)

 D2 ──┤├── GND                   push-button, internal pull-up: pressed = LOW
```

## The two sketches

| Sketch | States | Inputs | Structure |
|---|---|---|---|
| [`traffic-light-if-else/`](traffic-light-if-else/) | green, yellow, red | none | `int state` + one `if/else if` branch per transition |
| [`traffic-light-switch/`](traffic-light-switch/) | GREEN, YELLOW, RED, WALK | pedestrian button | `enum State` + `enterState()` + one `case` per state |

Both are **non-blocking**. Neither calls `delay()`. Each pass of `loop()` computes how long the current state has lasted, `millis() - stateStart`, and moves on only when it's time. The board keeps reading the button while the light is green. With `delay(10000)`, a press during those ten seconds would simply be missed.

## The machine, drawn

```
                  time ≥ 10 s, or button pressed and time ≥ 3 s
       ┌───────┐ ─────────────────────────────────────────► ┌────────┐
  ───► │ GREEN │                                            │ YELLOW │
       └───────┘ ◄──────────── time ≥ 2 s, no request ──┐   └────────┘
           ▲                                            │        │ time ≥ 2 s
           │ time ≥ 6 s                                 │        ▼
       ┌───────┐                                        │   ┌────────┐
       │ WALK  │ ◄──── time ≥ 2 s, request waiting ─────┴── │  RED   │
       └───────┘                                            └────────┘
```

Every arrow is one line in the `switch` version's `loop()`. Every box is one `case` in `enterState()`.

## Why switch/case scales better

Add the pedestrian button to the `if/else` version and see what each branch has to become:

```cpp
if (state == 0 && (inState >= GREEN_MS || (walkRequested && inState >= MIN_GREEN_MS))) { ... }
else if (state == 1 && inState >= YELLOW_MS) { ... }
else if (state == 2 && inState >= RED_MS && walkRequested) { ... walkRequested = false; ... }
else if (state == 2 && inState >= RED_MS) { ... }
else if (state == 3 && inState >= WALK_MS) { ... }
```

Now compare the two forms:

| | if/else chain | switch/case machine |
|---|---|---|
| **Where is state X's logic?** | Scattered: every branch mixes "which state?" with "which event?" | In one `case X:`, and only there |
| **Order sensitivity** | Branch order matters: the two `state == 2` branches only work in that order | Each case is independent |
| **Naming** | `state == 2` means red only if you remember | `case RED:`, so a misspelled state doesn't compile |
| **Entry actions** | Copied into every branch that enters a state (lights, timer reset, log) | Written once, in `enterState()` |
| **Forgotten states** | Silently do nothing | With compiler warnings on (*File → Preferences → Compiler warnings: All*), a `switch` on an enum warns about any state without a `case` |
| **Adding a state** | Edit several branches and re-check their order | One enum value, one entry action, one case |

Performance isn't the reason. For a handful of states, the compiler produces similar code either way. The gain is that the program's structure matches the diagram, so you can check one against the other line by line.

## Try this

1. **Blinking walk signal:** in the `WALK` case, make the walk LED blink during the last 2 seconds (`inState >= WALK_MS - 2000`). Where does that code go? It isn't an entry action, so it belongs in `loop()`'s `case WALK:`.
2. **Night mode:** add a `FLASHING_YELLOW` state, entered when the button is held for 3 seconds, and left by another 3-second hold. Count the places you had to edit.
3. **Break the if/else version:** add the pedestrian button to `traffic-light-if-else` exactly as shown above, then swap the order of the two `state == 2` branches. What happens?
4. **State names in the log:** write `const char* stateName(State s)` with its own `switch`, and use it in `enterState()`.

## Why this matters

Almost every embedded device is a state machine, whether or not its code admits it. A washing machine, a 3-D printer's heating sequence, a battery charger (pre-charge → constant current → constant voltage → done), a communication protocol handshake, an instrument's start-up self-test: each has distinct modes, actions on entering them, and rules for leaving them. Written explicitly, the machine can be drawn, reviewed and tested state by state. Hidden inside nested `if`s and flags, the same logic produces the bugs that are hardest to reproduce: the ones that only happen when two events arrive in an unusual order.

The next steps up from this pattern are a **transition table** (an array of *state + event → next state* rows, handy when the machine gets large) and **hierarchical state machines**, where states contain sub-states. [`../../projects/mash-controller/`](../../projects/mash-controller/) and [`../../projects/system-controller/`](../../projects/system-controller/) in this repo are both small state machines. Compare their structure with the one here.
