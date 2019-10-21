#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;
/*
GameWorld:
unsigned int getLevel() const;
unsigned int getLives() const;
void decLives();
void incLives();
unsigned int getScore() const; // after death does score zero?
void increaseScore(unsigned int howMuch);
void setGameStatText(string text);
bool getKey(int& value);
void playSound(int soundID);
*/
const int newCabbage = 11;
const int newTurnip = 12;
const int newTorpedo = 13;
const int newExplosion = 14;
const int newExtraLife = 15;
const int newRepair = 16;
const int newTorpedoGoodie = 17;

class Actor;
class NachenBlaster;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetDir);
    virtual int init();
    virtual int move(); 
    virtual void cleanUp();
	void newActor(int name, double startX = 0, double startY = 0, int ID = 0);
	void decAlien();
	void AlienKilled();
	NachenBlaster* getPlayer();
	void updateDisplayText();
	int isCollided(double x, double y, double R, int ID, int damage);
	bool isCollided(double x, double y, double R, int damage = 0);
	~StudentWorld();
private:
	list<Actor*> m_list;
	NachenBlaster* m_player = nullptr;
	int numDestroyed = 0;
	int numNeeded;
	int numAlien = 0;
};

#endif // STUDENTWORLD_H_
