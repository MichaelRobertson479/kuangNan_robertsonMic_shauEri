 # TRES

## Team Name
Tres Ring Circus

## Names

- Eric Shau (Period 5)
- Michael Robertson (Period 5)
- Nancy Kuang (Period 5)

## Description

We have implemented the card game **TRES, THE WORLD’S THIRD BEST CARD GAME**, using allocated memory, processes (fork and exec), signals, and shared memory. **TRES** has similar features and rules as UNO (valid plays explained below under **INSTRUCTIONS**). Players take turns playing cards from their hand onto the pile if they match the color or value, and drawing if they don’t. The game ends when someone wins by using up all of their cards, or if someone quits in the middle. 

Each card has a color and a type.

**Colors**:
1. R (red)
2. G (green)
3. B (blue)
4. Y (yellow)

**Types**:
- 0
- 1
- 2
- 3
- 4
- \+ (+2)
- R (reverse)
- S (skip)

The cards are displayed with the color, followed by the type.
* **Ex**: R3, B0, G+, YR

Each player is only allowed to play during their turn. During a player’s turn, the player must enter a valid sequence of cards, “draw”, or “quit”. If the player enters something illegal, then the program gets mad and tells the player to enter something valid.

## Required Libraries
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/wait.h>
```
## Instructions

**Note**: Players must ssh into the same machine and log in to the same account.

1. Compile with the ```make``` command.
2. Run with the ```make run``` command or ```./tres```
3. The first player should see:
> Welcome to the card game Tres!\
Enter "start" into the terminal at any time to start the game!
4. If you enter anything besides “start”, the program will yell:
> Tip: If you want to start the game, enter "start"!
5. The first player should wait until other players are loaded in.
6. The second, third, and other players should see:
> Welcome to the card game Tres!\
Waiting for Player 1 to start the game…
7. Once the first player enters “start”, the game should begin and say:
> Fantastic! Now the game has begun!\
You are Player [X].
8. Now, all players will receive an introductory message.
9. For Player 1, the game should say:
> It’s your turn.\
Draw Count:  <# of cards to draw>
10. For everyone else, the game should say.
> Waiting for your turn…\
It is Player 1’s turn.
11. Everyone should see underneath the statement above this:
> Top Card:  <Last Played card>\
Hand: <Cards in hand>

**Note**: If you have an absurd amount of cards, the hand will spill into a second line, but that shouldn’t affect the game.
12. Only Player 1 will see this:
> Your Move: ______

Valid plays:
- **A sequence of valid cards from the player’s hand**\
If the player must draw cards (i.e. when draw count  > 1), then + cards must be placed.
1. Cards of the same color and the same category (*Ex**: all normals (0-7), all +, all R, or all S) as the top card.
2. Cards of the same type as the top card.
- **“draw”**\
Player draws the number of cards to draw. It’s typically 1 unless the previous player (none in this case), has modified the # of cards to draw with a “+” card.
- **“quit”** \
Shuts down the game for all players with the message:
> Someone quit.\
The game is over.
- **Invalid play**\
The program yells at the player to make a valid move:
> Error: Invalid input. Check format/sequence of cards.
13. The card is played and becomes the top card on the pile, and its effects (if any) updates the game.
14. Now, for the next player (not necessarily Player 2, depending on Player 1’s move), the game should say:
> It’s your turn.\
Draw Count:  <# of cards to draw>
15. For everyone else, the game should say.
> Waiting for your turn… \
It is Player 2’s turn.
16. Everyone should see underneath the statement above this:
> Top Card:  <Last Played card>\
Hand: <Cards in hand>
17. Only Player 2 will see this:
> Your Move: ______
18. And so it continues circularly (once it gets to the last player who entered the game, and goes back to Player 1), until someone wins by playing all of their cards, or someone quits.
