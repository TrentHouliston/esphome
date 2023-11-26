## Pulse Filter Truth table

| Filter | Latch | From | To  | Diagram | Action          |            |
| ------ | ----- | ---- | --- | ------- | --------------- | ---------- |
| T      | 1     | 0    | 0   | `‾\_¦ ` | Reset           | Ghost high |
| T      | 1     | 0    | 1   | `‾\_/ ` | Reset           | Long low   |
| T      | 1     | 1    | 0   | `‾\/‾\` | -               | Long high  |
| T      | 1     | 1    | 1   | `‾\/‾¦` | -               | Ghost low  |
| T      | 0     | 1    | 1   | `_/‾¦`  | Set and Publish | Ghost low  |
| T      | 0     | 1    | 0   | `_/‾\ ` | Set and Publish | Long high  |
| T      | 0     | 0    | 1   | `_/\_/` | Rising Edge     | Long low   |
| T      | 0     | 0    | 0   | `_/\_¦` | -               | Ghost high |
| F      | 1     | 0    | 0   | `‾\¦  ` | -               | Ghost high |
| F      | 1     | 0    | 1   | `‾\/  ` | -               | Short low  |
| F      | 1     | 1    | 0   | `‾\/\ ` | -               | Short high |
| F      | 1     | 1    | 1   | `‾¦   ` | -               | Ghost low  |
| F      | 0     | 1    | 1   | `_/¦  ` | Rising Edge     | Ghost low  |
| F      | 0     | 1    | 0   | `_/\  ` | -               | Short high |
| F      | 0     | 0    | 1   | `_/\/ ` | Rising Edge     | Short low  |
| F      | 0     | 0    | 0   | `_¦ `   | -               | Ghost high |

## Startup

On startup the filter should not consider whatever state it is in as valid so it does not count a strange pulse.
There are two possible starting configurations, either the pin is high or the pin is low.
If the pin is high, the subsequent falling edge should not count as a pulse as we never saw the rising edge.
Therefore we start in the latched state.
If the pin is low, the subsequent rising edge can be counted as the first pulse.
Therefore we start in the unlatched state.
