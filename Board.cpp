#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    //will need to access m_game's nShips()
    int mRows;
    int mCols;
    char displayGrid[MAXROWS][MAXCOLS];
    //want to use a vector to dynamically allocate instead of an array
    struct shipsInPlay{
        int mShipID;
        int mLength;
        Point starting_pt;
        char mSymbol;
    };
    vector<shipsInPlay>shipsOnBoard;
    vector<int>IDs;
    vector<int>::iterator it;
   // vector<int>shipsDestroyed;
};
//DONE
BoardImpl::BoardImpl(const Game& g)
: m_game(g) //game should take in rows, cols
{
    mRows = m_game.rows();
    mCols = m_game.cols();
    
    //initialize the board
    for (int r = 0; r < mRows; r++)
    {
        for (int c = 0; c < mCols; c++)
        {
            //initialize every place on the board
            displayGrid[r][c] = '.';
        }
    }
}
//DONE
void BoardImpl::clear()
{
    for (int r = 0; r < mRows; r++)
    {
        for (int c = 0; c < mCols; c++)
        {
            displayGrid[r][c] = '.';
        }
    }
}
//DONE
void BoardImpl::block()
{
    int blocked = 0;
    while (blocked < (m_game.rows()*m_game.rows())/2 - 1)
    {
        Point p = Point(randInt(m_game.rows()), randInt(m_game.cols()));
        if (displayGrid[p.r][p.c] == '.')
        {
            displayGrid[p.r][p.c] = '@';
            blocked++;
        }
      
    }
}
//DONE
void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (displayGrid[r][c] == '@')
                displayGrid[r][c] = '.';
        }
}

//DONE
bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    //return false if shipID is invalid
    //ship would be partly or fully outside the board
    //ship would overlap one or more positions that were blocked by a previous call to the block function
    //the ship with that ship ID has previously been placed on this Board and not yet ben unplaced since its most recent placement
    
    if (shipId < 0 || shipId > m_game.nShips() - 1)
    {
        return false;
    }
    if (dir == HORIZONTAL)
    {
        //ship would be partly or fully outside of the board
        if (topOrLeft.c + m_game.shipLength(shipId) - 1 >= mCols)
            return false;
        //the ship would overlap an already placed ship or a blocked position
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (displayGrid[topOrLeft.r][topOrLeft.c + i] != '.')
                return false;
        }
        //if not, add
        for (int c = 0; c < m_game.shipLength(shipId); c++)
        {
            //is this the correct way to add a ship??
            displayGrid[topOrLeft.r][topOrLeft.c + c] = m_game.shipSymbol(shipId);
            IDs.push_back(shipId);
        }
    }
    if (dir == VERTICAL)
    {
        //ship would be partly or fully outside of the board
        if (topOrLeft.r + m_game.shipLength(shipId) - 1 >= mRows)
            return false;
        //the ship would overlap an already placed ship or a blocked position
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (displayGrid[topOrLeft.r + i][topOrLeft.c] != '.')
                return false;
        }
        //if not, add
        for (int r = 0; r < m_game.shipLength(shipId); r++)
        {
            displayGrid[topOrLeft.r + r][topOrLeft.c] = m_game.shipSymbol(shipId);
            IDs.push_back(shipId);
        }
    }

    //if the ship with that shipId has previously been placed on this board and not unplaced
    /*for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (displayGrid[topOrLeft.r + r][topOrLeft.c + c] == m_game.shipSymbol(shipId))
                return false;
        }
    }*/
    //if all true, after adding it into the board, hold it in the vector and return true
    shipsInPlay my_ship = {shipId, m_game.shipLength(shipId), topOrLeft, m_game.shipSymbol(shipId)};
    shipsOnBoard.push_back(my_ship);
    return true; // This compiles, but may not be correct
}

//maybe done???
bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    //shipId should go from 0 - 4 for each of the 5 ships in alphabetical order
    if (shipId < 0 || shipId > m_game.nShips() - 1)
    {
        return false;
    }
    //if the first point isn't in the board
    if (topOrLeft.r < 0 || topOrLeft.r > m_game.rows() - 1)
        return false;
    if (topOrLeft.c < 0 || topOrLeft.c > m_game.cols() - 1)
        return false;
    //if the ship is in the board
    if (dir == VERTICAL)
    {
        //the board does not contain the entire ship at the indicated locations
        int length_count = 0;
        for (int r = 0; topOrLeft.r + r < m_game.rows(); r++)
        {
            if (displayGrid[topOrLeft.r + r][topOrLeft.c] == m_game.shipSymbol(shipId))
                length_count++;
        }
        if (length_count != m_game.shipLength(shipId))
            return false;
        //else remove it
        for (int r = 0; r < m_game.shipLength(shipId); r++)
        {
            if (displayGrid[topOrLeft.r + r][topOrLeft.c] == m_game.shipSymbol(shipId))
            {
                displayGrid[topOrLeft.r + r][topOrLeft.c] = '.';
            }
        }
    }
    if (dir == HORIZONTAL)
    {
        int length_count = 0;
        for (int c = 0; topOrLeft.c + c < m_game.cols(); c++)
        {
            if (displayGrid[topOrLeft.r][topOrLeft.c + c] == m_game.shipSymbol(shipId))
                length_count++;
        }
        if (length_count != m_game.shipLength(shipId))
            return false;
        //else remove it
        for (int c = 0; c < m_game.shipLength(shipId); c++)
        {
            if (displayGrid[topOrLeft.r][topOrLeft.c + c] == m_game.shipSymbol(shipId))
            {
                displayGrid[topOrLeft.r][topOrLeft.c + c] = '.';
            }
        }
    }
    int vectorIndex = 0;
   // bool inVector = false;
    /*int vectorSize = 0;
    for (int j = 0; j < shipsOnBoard.size() - 1; j++)
    {
        vectorSize++;
    }*/
    for (int i = 0; i < shipsOnBoard.size(); i++)
    {

        if (shipsOnBoard[i].mShipID == shipId)
        {
            break;
        }
        vectorIndex++;
    }
    
    //remove from ships on board vector
    shipsOnBoard.erase(shipsOnBoard.begin() + vectorIndex);
    //remove all of them from IDs vector
    for (int i = 0; i < m_game.shipLength(shipId); i++)
    {
        it = find(IDs.begin(), IDs.end(), shipId);
        IDs.erase(it);
    }

    return true; // This compiles, but may not be correct

}
//DONE
void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for (int c = 0; c < m_game.cols(); c++)
    {
        cout << c;
    }
    cout << endl;
    for (int r = 0; r < m_game.rows(); r++)
    {
        cout << r << ' ';
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (displayGrid[r][c] == '.' || displayGrid[r][c] == 'X' || displayGrid[r][c] == 'o')
            {
                cout << displayGrid[r][c];
            }
            else
            {
                if (shotsOnly == true)
                {
                    //IS THIS CORRECT?????
                    cout << '.';
                }
                //if shotsOnly == true, show a period to display an undamaged ship segment
                else
                    cout << displayGrid[r][c];
            }
        }
        cout << endl;
    }
}
//NOT DONE 
bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //if not a valid attack point or already attacked once on the board, wasted a shot
    if (p.r < 0 || p.r > m_game.rows() - 1 || p.c < 0 || p.c > m_game.cols() - 1 || displayGrid[p.r][p.c] == 'o' || displayGrid[p.r][p.c] == 'X')
    {
        shotHit = false;
        shipDestroyed = true;
        return false;
    }
    //if attacking water, missed a shot
    if (displayGrid[p.r][p.c] == '.')
    {
        shotHit = false;
        shipDestroyed = false;
        displayGrid[p.r][p.c] = 'o';
        return true;
    }
    //else successfully attacking a segment of a ship
    if (displayGrid[p.r][p.c] != '.')
    {
        shotHit = true;
        
        //before replacing it, look for the ID
        int vectorIndex = 0;
        for (int i = 0; i < shipsOnBoard.size(); i++)
        {
            if (shipsOnBoard[i].mSymbol == displayGrid[p.r][p.c])
            {
                break;
            }
            vectorIndex++;
        }
        //need to update the shipId here
        shipId = shipsOnBoard[vectorIndex].mShipID;
        
        displayGrid[p.r][p.c] = 'X';
 
        //delete the one ID from the IDs vector
        it = find(IDs.begin(), IDs.end(), shipId);

        IDs.erase(it);
        

        //check if the ship is destroyed...look through vector of IDs to see if there are any left after you deleted
        
        //then the ID exists here
        if (find(IDs.begin(), IDs.end(), shipId) != IDs.end())
        {
            shipDestroyed = false;
            return true;
        }
        else
        {
            shipDestroyed = true;
            //should only add the ID once to shipsDestroyed
           // shipsDestroyed.push_back(shipId);
            return true;
        }
        
    }
    return false;
        
}
//DONE
bool BoardImpl::allShipsDestroyed() const
{
    if (IDs.empty())
    {
        return true;
    }
    return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}



