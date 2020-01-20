# kuangNan_robertsonMic_shauEri

# TRES

By Nancy Kuang, Michael Robertson, and Eric Shau (period 5)

Team Name: Tres Ring Circus

We hope to implement the card game TRES, THE WORLD’S THIRD BEST CARD GAME, as played on local computers. TRES has all of the same features as UNO except for having to say “Uno” and “Uno out.” Players will take turns playing cards from their hand onto the pile, if they match the color or value. The game ends when only one player has cards remaining.

# User Description:

The card’s color will be shown as the text’s color. 
The text will say what kind of card it is (i.e. “Eight” or “Reverse”).
The user’s hand will be shown as a list of cards on the screen (i.e. Your hand: Eight Reverse )
The top card of the pile will be displayed above the player’s hand.
The user will specify a card (in their hand) to play when prompted.

```
[Top card]		[Draw Pile (X cards)]		[Discard Pile (X cards)]
Console Command: [Player Y] played [top card]. It is [Player X]’s turn. 
Your hand: Red One, Yellow Four, Green Eight, Blue Nine
*It refreshes each turn
```

# Topics Covered in Class (edited):

- Shared Memory
  - Players will SSH into a single device (so no actual networking)
  - Used to show the top card of the pile, keep track of turns, and do pseudo-networking

- Semaphore
  - Used to control access to the file representing the pile of cards

- Working with Files
  - Write to a file, representing the pile of played cards, from STDIN

- Processes
  - Forking and exec-ing to clear the terminal

# What data structures you will be using and how.

- Structs
  - Store card information with two chars (Color, Type)
- Linked List (as in the song list project)
  - Store cards in a deck with FIFO properties
- Array
  - Represents the user’s hand

# What algorithms you will be using and how.

- A simple sorting algorithm for organizing cards in the hand first by number/type, then by color

# Timeline (still needs to be changed):

Fri 1/3: Making the Makefile and the header file (All)
- Figuring out which c and header files are needed.

Tues, 1/7: Implementing a single-user game with the following features:
- Drawing cards (Michael / Eric)
  - Cards will be randomly generated using srand (NULL)
  - 7 initial cards will be dealt out.
  - One card will be drawn when prompted by the user typing the word “draw”.
  - All cards will be generated randomly from two arrays: one with the color and the other with the type of the card (0-9, skip, reverse, +2, +4, wild)
- Playing cards (Nancy)
  - When prompted, the user will type in the card(s) that they wish to play, in the order that they wish to play the cards in (top card last). If the user is unable to (or does not wish to) play any cards, the user may enter “draw” to draw a card.
  - Cards will only be able to be placed if they have the same color or the same type. If a wild or +4 card is the last card played, then the user is prompted for a color. 
    - By this deadline, when the following non-regular cards are played (reverses, skips, +2, +4), their “special effects” (ex. drawing cards) will not be implemented.
  - When the user has no cards remaining, the game will end.
- Displaying the user’s hand (Eric)
  - Each card is a structure with a char for the color and a char for the type of card.
  - It will be displayed to the player as “[Color][Type of Card]”
  - Cards will be separated by a space.

Thurs 1/9: 
- Implement reverses and draws with shared memory (Eric)
- We will have a turn counter with modulus. Reverse subtracts, normal adds. Skip adds 2. (Michael)
- Implement clearing and updating the screen after a turn (using fork and exec) (Nancy)

Mon, 1/13:
- Begin networking stuff for implementing multiplayer (TBD) (All)
(We don’t know what is required for this yet)
- Winning conditions

Wed, 1/15: 
- Finish networking stuff (TBD) (All)
- Create a prettier display for all the cards (ex: colored text) (Michael, Eric)

# Extra implementations:

- Make colored ASCII art for the cards
- Sort cards based on type, then color
