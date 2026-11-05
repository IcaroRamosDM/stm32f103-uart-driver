# Command parser

| Command | Effect |
|---|---|
| help | Print supported commands |
| status | Print the demo status |
| led on | Turn the LED on |
| led off | Turn the LED off |
| blink | Run a blocking LED blink sequence |

CR and LF terminate a command. Backspace and DEL remove the last buffered
character. The 32-byte command buffer holds up to 31 characters plus its terminator;
additional characters are ignored until a line ending. Unknown commands receive
an explicit response.

Blink and serial output block the application. RX interrupts continue, but queued
bytes can be lost if the application does not drain the 127-byte receive capacity.
This is a command demonstration, not a production terminal or scheduler.
