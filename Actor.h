#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

/*
GraphObject(int imageID, double startX, double startY, int startDirection = 0,
double size = 1.0, int depth = 0);
double getX() const; // in pixels (0-255)
double getY() const; // in pixels (0-255)
void moveTo(double x, double y); // in pixels (0-255)
int getDirection() const; // in degrees (0-359)
void setDirection(int d); // in degrees (0-359)
void setSize(double size);
double getSize() const;
double getRadius() const; // in pixels (0-255)
*/
const int ally = 1;
const int enemy = 2;
const int neutral = 3;
const int allyproj = 4;
const int enemyproj = 5;

class StudentWorld;

class Actor : public GraphObject // other appropriate members may be added
{
public:
	Actor(int imageID, StudentWorld* pointer = nullptr, double startX = randInt(0, 255), double startY = randInt(0, 255), int startDirection = 0, double size = 1.0, int depth = 0, int ID = ally);
	virtual void doSomething() = 0;
	int getID();
	bool isAlive();
	virtual void becomeDead();
	virtual void sufferDamage(int damage);
	virtual int getDamage() { return 0; };
	StudentWorld* getWorld();
private:
	bool m_alive = true; // might be problematic
	int m_ID;
	StudentWorld* m_pointer = nullptr;
};



class NachenBlaster : public Actor
{
public:
	NachenBlaster(StudentWorld* pointer);
	virtual void doSomething();
	void incHP(int point);
	virtual void sufferDamage(int damage);
	void incTorpedo();
	int getHealth();
	int getCabbage();
	int getTorpedo();
private:
	int m_hitpoint = 50;
	int m_cabbage = 30;
	int m_torpedo = 0;
};

class Star : public Actor // complete definition + implementation
{
public:
	Star();
	Star(int newStar);
	void doSomething();
};

class Explosion : public Actor
{
public:
	Explosion(double startX, double startY);
	virtual void doSomething();
};


class Projectile : public Actor
{
public:
	Projectile(int imageID, StudentWorld * pointer, double startX, double startY, int ID, double damageAmt);
	virtual int getDamage();
	virtual void doSomething() = 0;
	// bool collisionCheck()
private:
	int m_damage;
};

class Cabbage : public Projectile
{
public:
	Cabbage(double startX, double startY, StudentWorld* pointer);
	virtual void doSomething();
};

class Turnip : public Projectile
{
public:
	Turnip(double startX, double startY, StudentWorld* pointer);
	virtual void doSomething();
};

class Torpedo : public Projectile
{
public:
	Torpedo(double startX, double startY, StudentWorld* pointer, int ID);
	virtual void doSomething();
};




class Goodie : public Actor
{
public:
	Goodie(int imageID, StudentWorld* pointer, double startX, double startY);
	bool collisionCheck();
	virtual void doSomething() = 0;
};

class ExtraLife : public Goodie
{
public:
	ExtraLife(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
};

class Repair : public Goodie
{
public:
	Repair(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
};

class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
};




class Alien : public Actor
{
public:
	Alien(double startX, double startY, const int ID, StudentWorld* pointer, double speed = 2.0);
	virtual void doSomething() = 0;
	virtual void becomeDead();
	void initHP(int n = 1);
	int getFPlan();
	void decFPlan();
	void setFPlan(int length);
	double getSpeed();
	void setSpeed(double speed);
	int getTravelDirection();
	void setTravelDirection(int direction);
	virtual void sufferDamage(int damage);
	virtual bool collisionCheck(); // return true if dead, false if not
private:
	int m_hitpoint;
	int m_flightplan = 0;
	double m_speed;
	int m_travelDirection = 225;
};

class Smallgon : public Alien
{
public:
	Smallgon(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
};

class Smoregon : public Alien
{
public:
	Smoregon(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
};

class Snagglegon : public Alien
{
public:
	Snagglegon(double startX, double startY, StudentWorld * pointer);
	virtual void doSomething();
	virtual bool collisionCheck();
	virtual void sufferDamage(int damage);
};

#endif // ACTOR_H_
