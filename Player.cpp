#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;


//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}
class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);// : Player(nm, g) {}
    virtual ~HumanPlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board &b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};
HumanPlayer::HumanPlayer(string nm, const Game& g) : Player(nm, g)
{

}
HumanPlayer::~HumanPlayer()
{

}
bool HumanPlayer::isHuman() const
{
    return true;
}
bool HumanPlayer::placeShips(Board &b)
{
    int shipsPlaced = 0;
    cout << name() << " the human must place " << game().nShips() << " ships." << endl;
    for (int i = 0; i < game().nShips(); i++)
    {
        char direction;
        int r = 0;
        int c = 0;
        b.display(false);
        cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
        //either h or v
        cin >> direction;
        //somehow have to add a loop to keep asking the user to input until its valid
        bool validInp = false;

        while (validInp == false){
            if (direction != 'h' && direction != 'v')
            {
                cout << "Direction must be h or v." << endl;
                cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
                cin >> direction;
            }
            else if (direction == 'h')
            {
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                getLineWithTwoIntegers(r, c);
                cin.clear();
                if (b.placeShip(Point(r,c), i, HORIZONTAL) == false)
                {
                    cout << "The ship can not be placed there." << endl;
                }
                //WHAT HAPPENS IF THEY ENTER NOT 2 COORDS??
                else
                {
                    b.placeShip(Point(r,c), i, HORIZONTAL);
                    validInp = true;
                    shipsPlaced++;
                }
            }
            else //direction == 'v'
            {
                cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                getLineWithTwoIntegers(r, c);
                cin.clear();
                if (b.placeShip(Point(r,c), i, VERTICAL) == false)
                {
                    cout << "The ship can not be placed there." << endl;
                }
                else
                {
                    b.placeShip(Point(r,c), i, VERTICAL);
                    validInp = true;
                    shipsPlaced++;
                }
            }
        }

    }
    b.display(false);
    //if all ships have been successfully placed
    if (game().nShips() == shipsPlaced)
        return true;
;
    return false;
}
Point HumanPlayer::recommendAttack()
{
    //have to add here if there is a problem with coordinates entered (NOT DONE)
    int r = 0;
    int c = 0;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    getLineWithTwoIntegers(r, c);
    Point point(r,c);
    return point;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
}
void HumanPlayer::recordAttackByOpponent(Point p)
{
}


//typedef AwfulPlayer HumanPlayer;

//*********************************************************************
//  MediocrePlayer
//*********************************************************************
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual ~MediocrePlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board &b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    struct ships{
        int mShipID;
        int mLength;
        char mSymbol;
    };
    //this is the function that will try to place the ship
    bool recursivePlacing (int shipId, Board &b);
    void getGameInfo();
    int mState = 1;
    vector<Point>::iterator it;
    vector<Point>attackablePoints;
    void findAttackablePoints(Board& b);
    Point lastPoint;
    Point randomS2Point(Point p);
    vector<Point>s2Points;
    bool isAttackable(Point p);
    char grid[MAXROWS][MAXCOLS];
};
//DONE
MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g){}
MediocrePlayer::~MediocrePlayer(){}
bool MediocrePlayer::isHuman() const
{
    return false;
}
bool MediocrePlayer::placeShips(Board &b)
{
    findAttackablePoints(b);
    //RECURSION up to 50 tries -
    for (int i  = 0; i < 50; i++)
    {
        b.block();
        if (recursivePlacing(0, b))
        {
            b.unblock();
            return true;
        }
        else
            b.unblock();
    }
    
    //return true if all the ships were succesfully placed
    //after 50 tries, if it doesn't work, return false
    return false;
}
bool MediocrePlayer::recursivePlacing(int shipId, Board& b)
{
    //if this is the last ship, return true;
    if (shipId == game().nShips())
    {
        return true;
    }
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++ )
        {
            if (b.placeShip(Point (r,c), shipId, HORIZONTAL))
            {
               // b.display(false);
                
                if (recursivePlacing(shipId + 1, b))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(Point (r,c), shipId, HORIZONTAL);
                }
            }
            if (b.placeShip(Point (r,c), shipId, VERTICAL))
            {
              //  b.display(false);
                if (recursivePlacing(shipId + 1, b))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(Point (r,c), shipId, VERTICAL);
                }
            }
        }
    }
    
    return false;
}
//DONE????
Point MediocrePlayer::recommendAttack()
{
    int vectorSize = 0;
    for (int i = 0; i < attackablePoints.size(); i++)
    {
        vectorSize++;
    }
    if (mState == 1)
    {
        //choose random point to attack
        Point p = attackablePoints[randInt(vectorSize)];
        //once that point is chosen, erase it from attackablePoints vector
        it = attackablePoints.begin();
        
        while (it != attackablePoints.end())
        {
            if (it->r == p.r && it->c == p.c)
            {
                break;
            }
            it++;
        }
        attackablePoints.erase(it);
        lastPoint = p;
        return p;
    }
    //mState == 2
    else
    {
        Point p2 = randomS2Point(lastPoint);
        it = attackablePoints.begin();
        while (it != attackablePoints.end())
        {
            //if it never hits this condition below, then randomS2Point isn't an attackable point
            if (it->r == p2.r && it->c == p2.c)
            {
                break;
            }
            it++;
        }
        //this means it's an attackable point, we attack and then erase it
        attackablePoints.erase(it);
        return p2;
    }
}
void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //if the game has ships of length 6 or more, it's possible that every position no more than 4 steps from (r,c) has been previously chosen. in that case switch to state 1
    if (mState == 2 && game().shipLength(shipId) >= 6)
    {
        mState = 1;
    }
    if (mState == 2 && s2Points.empty())
    {
        mState = 1;
    }
    //in state 1, misses, stay in state 1
    if (shotHit == false && mState == 1)
    {
        mState = 1;
        grid[p.r][p.c] = 'o';
    }
    //in state 1, hits a ship, destroys ship, stay in state 1
    else if (shotHit == true && shipDestroyed == true && mState == 1)
    {
        mState = 1;
        grid[p.r][p.c] = 'X';
    }
    //in state 1, hits a ship, does not destroy it, switch to 2
    else if (shotHit == true && shipDestroyed == false && mState == 1)
    {
        mState = 2;
        grid[p.r][p.c] = 'X';
    }
    //in state 2, misses, stays in state 2
    else if (shotHit == false && mState == 2)
    {
        mState = 2;
        grid[p.r][p.c] = 'o';
    }
    //in state 2, hits a ship, does not destroy it, stays in state 2
    else if (shotHit == true && shipDestroyed == false && mState == 2)
    {
        mState = 2;
        grid[p.r][p.c] = 'X';
    }
    //in state 2, hits a ship, destroys it, switch to state 1
    else if (shotHit == true && shipDestroyed == true && mState == 2)
    {
        mState = 1;
        grid[p.r][p.c] = 'X';
    }
    
}
//DONE
void MediocrePlayer::recordAttackByOpponent(Point p){}
//DONE
void MediocrePlayer::findAttackablePoints(Board& b)
{
    //this is to establish the original vector of attackable points
    b.clear();
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            attackablePoints.push_back(Point (r,c));
            grid[r][c] = '.';
        }
    }
}

Point MediocrePlayer::randomS2Point(Point p) //should return a random point within the spec for state 2
{
    //clear the vector before adding to it
    s2Points.clear();
    //UP
    for (int i = 1; i < 5; i++)
    {
        if (p.r - i >= 0 && isAttackable(Point (p.r - i, p.c)))
        {
            s2Points.push_back(Point (p.r - i, p.c));
        }
    }
    //DOWN
    for (int i = 1; i < 5; i++)
    {
        if (p.r + i <= game().rows() && isAttackable(Point (p.r + i, p.c)))
        {
            s2Points.push_back(Point (p.r + i, p.c));
        }
    }
    //RIGHT
    for (int i = 1; i < 5; i++)
    {
        if (p.c + i <= game().cols() && isAttackable(Point (p.r, p.c + i)))
        {
            s2Points.push_back(Point (p.r, p.c + i));
        }
    }
    //LEFT
    for (int i = 1; i < 5; i++)
    {
        if (p.c - i >= 0 && isAttackable(Point (p.r, p.c - i)))
        {
            s2Points.push_back(Point (p.r, p.c - i));
        }
    }
    
    int vectorSize = 0;
    for (int i = 0; i < s2Points.size(); i++)
    {
        vectorSize++;
    }
    //what happens if s2Points is empty?
    if (vectorSize == 0)
    {
        //essentially go to state 1
        int vectorSize2 = 0;
        for (int i = 0; i < attackablePoints.size(); i++)
        {
            vectorSize2++;
        }
        Point p = attackablePoints[randInt(vectorSize2)];
        return p;
    }
    else
    {
        Point pt = s2Points[randInt(vectorSize)];
        return pt;
    }
    
}
bool MediocrePlayer::isAttackable(Point p)
{
    it = attackablePoints.begin();
    
    while (it != attackablePoints.end())
    {
        if (it->r == p.r && it->c == p.c)
        {
            break;
        }
        it++;
    }
    //this belows means that you can attack this point because it's a valid point and has not been attacked yet
    if (it != attackablePoints.end() && grid[it->r][it->c] == '.')
    {
        return true;
    }
    
    return false;
}


//TODO:  You need to replace this with a real class declaration and implementation.
//typedef AwfulPlayer MediocrePlayer;
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************
class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer();
    virtual bool isHuman() const;
    virtual bool placeShips(Board &b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool recursivePlacing (int shipId, Board &b);
    int mState = 1;
    vector<Point>::iterator it;
    vector<Point>attackablePoints;
    void findAttackablePoints(Board& b);
    Point lastPoint;
    vector<Point>s1Points;
    vector<Point>s2Points;
    bool isAttackable(Point p);
    char grid[MAXROWS][MAXCOLS];
    void finds1Points(Board& b);
    string direction;
    Point nextPoint;
    void removePoint(Point p);
};
void GoodPlayer::removePoint(Point p)
{
    it = attackablePoints.begin();

    while (it != attackablePoints.end())
    {
        if (it->r == p.r && it->c == p.c)
        {
            break;
        }
        it++;
    }
    attackablePoints.erase(it);
    it = s1Points.begin();
    while (it != s1Points.end())
    {
        if (it->r == p.r && it->c == p.c)
        {
            break;
        }
        it++;
    }
    if (it != s1Points.end())
        s1Points.erase(it);
}
//DONE
GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g){}
GoodPlayer::~GoodPlayer(){}
bool GoodPlayer::isHuman() const
{
    return false;
}

bool GoodPlayer::placeShips(Board &b)
{
    findAttackablePoints(b);
    finds1Points(b);
    //RECURSION up to 50 tries -
    for (int i  = 0; i < 50; i++)
    {
        b.block();
        if (recursivePlacing(0, b))
        {
            b.unblock();
            return true;
        }
        else
            b.unblock();
    }
    return false;
 
}
bool GoodPlayer::recursivePlacing(int shipId, Board& b)
{
    //if this is the last ship, return true;
    if (shipId == game().nShips())
    {
        return true;
    }
    int randomRows[10] = {0,9,1,8,2,7,3,6,4,5};
    int randomCols[10] = {0,9,1,8,2,7,3,6,4,5};
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++ )
        {
            Point p = Point (randomRows[r], randomCols[c]);
            if (b.placeShip(p, shipId, HORIZONTAL))
            {
               // b.display(false);
                if (recursivePlacing(shipId + 1, b))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                }
            }
            if (b.placeShip(p, shipId, VERTICAL))
            {
              //  b.display(false);
                if (recursivePlacing(shipId + 1, b))
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

Point GoodPlayer::recommendAttack()
{
    int vectorSize = 0;
    for (int i = 0; i < attackablePoints.size(); i++)
    {
        vectorSize++;
    }
    if (mState == 1)
    {
        //choose random point to attack within s1Points, if its not, then choose from attackablePoints
        
        if (!s1Points.empty())
        {
            int vectorSize = 0;
            for (int i = 0; i < s1Points.size(); i++)
            {
                vectorSize++;
            }
            Point p = s1Points[randInt(vectorSize)];
            //delete this point from attackablePoints and s1Points
            removePoint(p);
            lastPoint = p;
            return p;
        }
        else
        {
            Point p = attackablePoints[randInt(vectorSize)];
            //once that point is chosen, erase it from attackablePoints vector
            removePoint(p);
            lastPoint = p;
            return p;
        }
    }
    //mState == 2
    else if (mState == 2)
    {
        int r = lastPoint.r;
        int c = lastPoint.c;
        if (direction == "NORTH")
        {
            if (r - 1 >= 0 && isAttackable(Point (r - 1, c)))
            {
                nextPoint = Point (r-1, c);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
                direction = "SOUTH";
        }
        if (direction == "SOUTH")
        {
            if (r + 1 < game().rows()  && isAttackable(Point (r + 1, c)))
            {
                nextPoint = Point (r+1, c);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
                direction = "WEST";
        }
        if (direction == "WEST")
        {
            if (c - 1 >= 0 && isAttackable(Point (r, c - 1)))
            {
                nextPoint = Point (r, c-1);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
                direction = "EAST";
        }
        if (direction == "EAST")
        {
            if (c + 1 < game().cols() && isAttackable(Point (r, c + 1)))
            {
                nextPoint = Point (r, c+1);
                removePoint(nextPoint);
                return nextPoint;
            }
        }
        //in case all the points are taken, go to state 1
        if (!s1Points.empty())
        {
            int vectorSize = 0;
            for (int i = 0; i < s1Points.size(); i++)
            {
                vectorSize++;
            }
            Point p = s1Points[randInt(vectorSize)];
            //delete this point from attackablePoints and s1Points
            removePoint(p);
            lastPoint = p;
            return p;
        }
        else
        {
            Point p = attackablePoints[randInt(vectorSize)];
            //once that point is chosen, erase it from attackablePoints vector
            removePoint(p);
            lastPoint = p;
            return p;
        }
        
       
    }
    else //mState == 3
    {
        //take the nextPoint, and attack in the same direction
        
        if (direction == "NORTH")
        {
            int r = nextPoint.r;
            int c = nextPoint.c;
            if (r - 1 >= 0 && isAttackable(Point (r - 1, c)))
            {
                nextPoint = Point (r-1, c);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
            {
                direction = "SOUTH";
                nextPoint = lastPoint;
            }
        }
        if (direction == "SOUTH")
        {
            int r = nextPoint.r;
            int c = nextPoint.c;
            if (r + 1 < game().rows() && isAttackable(Point (r + 1, c)))
            {
                nextPoint = Point (r+1, c);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
            {
                direction = "WEST";
                nextPoint = lastPoint;
            }
        }
        if (direction == "WEST")
        {
            int r = nextPoint.r;
            int c = nextPoint.c;
            if (c - 1 >= 0 && isAttackable(Point (r , c - 1)))
            {
                nextPoint = Point (r, c-1);
                removePoint(nextPoint);
                return nextPoint;
            }
            else
            {
                direction = "EAST";
                nextPoint = lastPoint;
            }
        }
        if (direction == "EAST")
        {
            int r = nextPoint.r;
            int c = nextPoint.c;
            if (c + 1 < game().cols() && isAttackable(Point (r, c + 1)))
            {
                nextPoint = Point (r, c+1);
                removePoint(nextPoint);
                return nextPoint;
            }
        }
        //in case all the points are taken, go to state 1
        if (!s1Points.empty())
        {
            int vectorSize = 0;
            for (int i = 0; i < s1Points.size(); i++)
            {
                vectorSize++;
            }
            Point p = s1Points[randInt(vectorSize)];
            //delete this point from attackablePoints and s1Points
            removePoint(p);
            lastPoint = p;
            return p;
        }
        else
        {
            Point p = attackablePoints[randInt(vectorSize)];
            //once that point is chosen, erase it from attackablePoints vector
            removePoint(p);
            lastPoint = p;
            return p;
        }
    }
}
void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    //in state 1, misses, stay in state 1
    if (shotHit == false && mState == 1)
    {
        mState = 1;
        grid[p.r][p.c] = 'o';
    }
    //in state 1, hits a ship, destroys ship, stay in state 1
    else if (shotHit == true && shipDestroyed == true && mState == 1)
    {
        mState = 1;
        grid[p.r][p.c] = 'X';
    }
    //in state 1, hits a ship, does not destroy it, switch to 2
    else if (shotHit == true && shipDestroyed == false && mState == 1)
    {
        mState = 2;
        direction = "NORTH";
        grid[p.r][p.c] = 'X';
    }
    //in state 2, misses, stays in state 2, but change directions
    else if (shotHit == false && mState == 2)
    {
        mState = 2;
        if (direction == "NORTH")
            direction = "SOUTH";
        else if (direction == "SOUTH")
            direction = "WEST";
        else if (direction == "WEST")
            direction = "EAST";
        grid[p.r][p.c] = 'o';
 
    }
    //in state 2, hits a ship, does not destroy it, change to state 3
    else if (shotHit == true && shipDestroyed == false && mState == 2)
    {
       // mState = 2;
        //don't change direction
        mState = 3;
        grid[p.r][p.c] = 'X';
    }
    //in state 2, hits a ship, destroys it, switch to state 1
    else if (shotHit == true && shipDestroyed == true && (mState == 2 || mState == 3))
    {
        mState = 1;
        grid[p.r][p.c] = 'X';
    }
    else if (shotHit == true && shipDestroyed == false && mState == 3)
    {
        mState = 3;
        grid[p.r][p.c] = 'X';
    }
    else //if (shotHit == false && mState == 3)
    {
        mState = 2;
        if (direction == "NORTH")
            direction = "SOUTH";
        else if (direction == "SOUTH")
            direction = "WEST";
        else if (direction == "WEST")
            direction = "EAST";
        grid[p.r][p.c] = 'o';
    }

    
}
//DONE
void GoodPlayer::recordAttackByOpponent(Point p){}
//DONE
void GoodPlayer::findAttackablePoints(Board& b)
{
    //this is to establish the original vector of attackable points
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            attackablePoints.push_back(Point (r,c));
            grid[r][c] = '.';
        }
    }
}
void GoodPlayer::finds1Points(Board& b)
{
    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            if ((r+c)%2 == 0)
            {
                s1Points.push_back(Point(r,c));
            }
        }
    }
}
 
bool GoodPlayer::isAttackable(Point p)
{
    it = attackablePoints.begin();
    
    while (it != attackablePoints.end())
    {
        if (it->r == p.r && it->c == p.c)
        {
            break;
        }
        it++;
    }
    //this belows means that you can attack this point because it's a valid point and has not been attacked yet
    if (it != attackablePoints.end() && grid[it->r][it->c] == '.')
    {
        return true;
    }
    return false;
}



// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}


