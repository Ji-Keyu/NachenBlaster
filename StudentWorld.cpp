#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), numNeeded(6+4*getLevel()) {}

int StudentWorld::init()
{
	m_player = new NachenBlaster(this);
	for (int i = 0; i < 30; i++)
		m_list.push_back(new Star);
	numDestroyed = 0;
	numNeeded = 6 + 4 * getLevel();
	numAlien = 0;
    return GWSTATUS_CONTINUE_GAME;  
}

int StudentWorld::move()
{
	m_player->doSomething();
 	for (list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->doSomething();
			if (!m_player->isAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (numNeeded == 0)
			{
				playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
	
	// √ delete dead game objects 
	list<Actor*>::iterator it = m_list.begin();
	while (it != m_list.end())
	{
		if (!(*it)->isAlive())
		{
			delete *it;
			it = m_list.erase(it);
		}
		else
			it++;
	}
	
	updateDisplayText(); 
	if (randInt(1, 15) == 1)
		m_list.push_back(new Star(VIEW_WIDTH - 1));
	int p = 4 + 0.5 * getLevel();
	if (numAlien < numNeeded && numAlien < p)
	{
		int S1 = 60;
		int S2 = 20 + getLevel() * 5;
		int S3 = 5 + getLevel() * 10;
		int S = S1 + S2 + S3;
		int rand = randInt(1, S);
		if (rand <= S1)
			m_list.push_back(new Smallgon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
		else if (rand <= S1+S2)
			m_list.push_back(new Smoregon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
		else if (rand <= S1+S2+S3)
			m_list.push_back(new Snagglegon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this));
		numAlien++;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if (m_list.size() != 0)
	{
		delete m_player;
		m_player = nullptr;
	}
	list<Actor*>::iterator it = m_list.begin();
	while (it != m_list.end())
	{
		delete *it;
		it = m_list.erase(it);
	}
}

	

void StudentWorld::newActor(int name, double startX, double startY, int ID)
{
	switch (name)
	{
	case newCabbage:
		m_list.push_back(new Cabbage(m_player->getX() + 12, m_player->getY(), this));
		break;
	case newTurnip:
		m_list.push_back(new Turnip(startX, startY, this));
		break;
	case newTorpedo:
		m_list.push_back(new Torpedo(startX, startY, this, ID));
		break;
	case newExplosion:
		m_list.push_back(new Explosion(startX, startY));
		break;
	case newExtraLife:
		m_list.push_back(new ExtraLife(startX, startY, this));
		break;
	case newRepair:
		m_list.push_back(new Repair(startX, startY, this));
		break;
	case newTorpedoGoodie:
		m_list.push_back(new TorpedoGoodie(startX, startY, this));
		break;
	}
}

void StudentWorld::decAlien()
{
	numAlien--;
}

void StudentWorld::AlienKilled()
{
	numDestroyed++;
	numNeeded--;
}

NachenBlaster* StudentWorld::getPlayer()
{
	return m_player;
}

void StudentWorld::updateDisplayText()
{
//Lives: 3 Health : 100 % Score : 24530 Level : 3 Cabbages : 80 % Torpedoes : 4
	ostringstream oss;  // oss is a name of our choosing.
	oss << "Lives: " << getLives() << "  ";
	oss << "Health: " << m_player->getHealth() * 2 << "%" << "  ";
	oss << "Score: " << getScore() << "  ";
	oss << "Level: " << getLevel() << "  ";
	oss << "Cabbages: " << m_player->getCabbage() * 100 / 30 << "%" << "  ";
	oss << "Torpedoes: " << m_player->getTorpedo();
	string s = oss.str();
	setGameStatText(s);
}

int StudentWorld::isCollided(double x, double y, double R, int ID, int damage)
{
	list<Actor*>::iterator it = m_list.begin();
	while (it != m_list.end())
	{
		if ((*it)->getID() == ID && (*it)->isAlive())
			if (sqrt(pow(((*it)->getX() - x), 2) + pow(((*it)->getY() - y), 2)) < .75 * (R + (*it)->getRadius()))
			{
				(*it)->sufferDamage(damage);
				if (ID == enemy)
					return (*it)->isAlive();
				else
					return (*it)->getDamage();
			}
		it++;
	}
	return -1;
}

bool StudentWorld::isCollided(double x, double y, double R, int damage)
{
	if (sqrt(pow((m_player->getX() - x), 2) + pow((m_player->getY() - y), 2)) < .75 * (R + m_player->getRadius()))
	{
		m_player->sufferDamage(damage);
		return true;
	}
	return false;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}