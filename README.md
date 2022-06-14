# CS32Project3

For our UCLA CS 32 Spring 2022 Project 3, we were given a skeleton of files and specifications to create a Battleship game, including it's very own AI players.

Here are the specifications for each of the classes we had to implement:

1. A Board class: This class maintains the board data structure. During a game,
there will be an instance of the Board class for each player, to record the locations
of and damage to that player's ships. You will need to implement most of this
class’s member functions.

2. A Player abstract base class: This base class specifies the interface for all
players. You will be given this class. A Player has two main tasks:
  a. It must place the ships on the board using a specified algorithm.
  b. It must make attacks against its opponent in a game of Battleship.
  
3. An AwfulPlayer class, derived from the Player class: We provide this class as an
example of a concrete class derived from Player. It plays the game legally, but
very stupidly.

4. A HumanPlayer class, derived from the Player class: You will implement a
Human Player class. The functions in this class will prompt the user to decide
where to place ships, and where to attack. (This way, the user can play against
the computer.)

5. A MediocrePlayer class, derived from the Player class: We will provide a set of
requirements so you can design and build a Mediocre Battleship Player. You will need to implement most of this class’s member functions. 
Your Mediocre Player must work exactly as specified later in this specification.

6. A GoodPlayer class, derived from the Player class: You will be asked to
implement your own Battleship player algorithms. Your goal is to build an
artificially intelligent player class that can beat the Mediocre Player and ideally,
your fellow students' GoodPlayers.

7. A Point class: This simple class is used to specify a (row,column) coordinate.
You don’t have to write any code for this class! 

8. A Game class: This class is used to run a complete game between two players.

9. A main function allowing the user to create and play a game. We will not look at
your main function, so you can use it for testing purposes.


