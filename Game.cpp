#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <algorithm>
//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================
#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};


using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int mRows;
    int mCols;
    
    struct Ship{
    //reminder: default for a struct is public
        int mLength;
        char mSymbol;
        string mName;
        int mShipID;
        //Constructor
        Ship(int length, char symbol, string name)
            : mLength(length), mSymbol(symbol), mName(name)
        {}
    };
    //holding all the ships that are in the game
    vector<Ship>mShips;
    
};
//DONE
void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

//DONE
GameImpl::GameImpl(int nRows, int nCols)
{
    mRows = nRows;
    mCols = nCols;
}
//DONE
int GameImpl::rows() const
{
    return mRows;  // This compiles but may not be correct
}
//DONE
int GameImpl::cols() const
{
    return mCols;  // This compiles but may not be correct
}
//DONE
bool GameImpl::isValid(Point p) const
{
    //make sure its in the grid
    if (p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols())
        return true;
    return false;
}
//DONE
Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    //Add a new type of ship to the game
    //Requirements: length > 0, must fit on board (call to isValid)
    //when board is displayed, symbol for the ship type will appear
    //if the arguments in a call to addShip satisfy the constraints, the call results in a new ship type being added to the game, and the call returns true
    
    //does not fit the length requirement
    //if (length < 0 || length > mRows || length > mCols)
      //  return false;
    
    //else then create a new ship object
    Ship ship = Ship(length, symbol, name);
    
    //I might want to use a board member function (place Ship) to add a ship???
    mShips.push_back(ship);
    return true;
}

//DONE
int GameImpl::nShips() const
{
    int numShips = 0;
    for (int i = 0; i < mShips.size(); i++)
    {
        numShips++;
    }
    return numShips;
   // return mShips.size();
   
}
//DONE?
int GameImpl::shipLength(int shipId) const
{
    return mShips[shipId].mLength;
}
//DONE?
char GameImpl::shipSymbol(int shipId) const
{
    
    return mShips[shipId].mSymbol;
}

//DONE?
string GameImpl::shipName(int shipId) const
{
    
    return mShips[shipId].mName;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
//    Timer t; 
    //placeShips function may fail, so account for that -- if placeShips == false, return false
    if ((p1->placeShips(b1)) == false)
        return nullptr;
    if ((p2->placeShips(b2)) == false)
        return nullptr;
    //else if the players have successfully placed a ship 
    //waitForEnter();
    //make a temp pointer to p1, p2 and boards to reduce lines of code
    Player* temp1;
    Player* temp2;
    Board* bd;
    
    bool p1Playing = true; //to hold who's turn it is
    

    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        if (p1Playing == true)
        {
            temp1 = p1;
            temp2 = p2;
            //player 1 will be attacking player2, making changes to board 2
            bd = &b2;
        }
        else
        {
            temp1 = p2;
            temp2 = p1;
            bd = &b1;
        }
        //Temp1's turn
        cout << temp1->name()<< "'s turn. Board for " << temp2->name() << ": " <<endl;
        //Display Temp1's board
        //If Temp1 is human, display(true) -- will use . to show undamaged
        if (temp1->isHuman() == true)
        {
            bd->display(true);
        }
        //If Temp1 is not human, display(false) -- will use ships symbol
        else
        {
            bd->display(false);
        }
        //attack
        Point p = temp1->recommendAttack();
        
        bool shotHit = false;
        bool shipDestroyed = false;
        int shipId = 0;
        //record the attack
        if (bd->attack(p, shotHit, shipDestroyed, shipId) == true)
        {
            temp1->recordAttackResult(p, true, shotHit, shipDestroyed, shipId);
        }
        else
        {
            temp1->recordAttackResult(p, false, shotHit, shipDestroyed, shipId);
        }
        //tell the player the result of the game
        //destroyed a ship
        if (shotHit == true && shipDestroyed == true)
        {
            cout << temp1->name() << " attacked " << "(" << p.r<< ','<< p.c << ")" << " and destroyed the " << temp2->game().shipName(shipId)<< ", resulting in: " << endl;
        }
        //wasted a shot
        else if (shotHit == false && shipDestroyed == true)
        {
            cout << temp1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
        }
        //missed
        else if (shotHit == false)
        {
            cout << temp1->name() << " attacked (" << p.r << ',' << p.c << ") and missed, resulting in: " << endl;
        }
        //hit but not destroyed
        else
        {
            cout << temp1->name() << " attacked (" << p.r << ',' << p.c << ") and hit something, resulting in: " << endl;
        }
        //display the board after the attack
        //If Temp2 is human, display(true) -- will use '.' for undamaged
        if (temp1->isHuman() == true)
        {
            bd->display(true);
        }
        //If Temp1 is not human, display(false) -- will use ships symbol
        else
        {
            bd->display(false);
        }
        //now to p2's turn
        if (p1Playing == true)
        {
            p1Playing = false;
        }
        else
        {
            p1Playing = true;
        }
        //if all ships destroyed, means one player won, and we break out of the loop
        if (b1.allShipsDestroyed() == true || b2.allShipsDestroyed() == true)
        {
            break;
        }
        if (shouldPause == true || b1.allShipsDestroyed() == true || b2.allShipsDestroyed() == true)
        {
            waitForEnter();
        }
        
        
//        if (t.elapsed() > 4000)
//        {
//            cout << "THAT'S TOUGH" << endl;
//        }
    }
    if (b1.allShipsDestroyed() == true)
    {
        cout << p2->name() << " wins!" << endl;
        return p2;
    }
    else
    {
        cout << p1->name() << " wins!" << endl;
        return p1;
    }
    
    
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

