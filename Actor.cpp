#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(int imageID, StudentWorld * pointer, double startX, double startY, int startDirection, double size, int depth, int ID)
	: GraphObject(imageID, startX, startY, startDirection, size, depth), m_pointer(pointer), m_ID(ID) {}

int Actor::getID()
{
	return m_ID;
}

bool Actor::isAlive()
{
	return m_alive;
}

void Actor::becomeDead()
{
	m_alive = false;
}

void Actor::sufferDamage(int damage)
{
	becomeDead();
}

StudentWorld* Actor::getWorld()
{
	return m_pointer;
}




NachenBlaster::NachenBlaster(StudentWorld* pointerToWorld)
	:Actor(IID_NACHENBLASTER, pointerToWorld, 0, 128) {}

void NachenBlaster::doSomething()
{
	if (!isAlive())
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			if (m_cabbage >= 5)
			{
				m_cabbage -= 5;
				getWorld()->newActor(newCabbage);
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
			}
			break;
		case KEY_PRESS_TAB:
			if (m_torpedo > 0)
			{
				m_torpedo -= 1;
				getWorld()->newActor(newTorpedo, getX() + 12, getY(), allyproj);
				getWorld()->playSound(SOUND_TORPEDO);
			}
		case KEY_PRESS_UP:
			if (getY() + 6 < VIEW_HEIGHT)
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_DOWN:
			if (getY() - 6 >= 0)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_LEFT:
			if (getX() - 6 >= 0)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() + 6 < VIEW_WIDTH)
				moveTo(getX() + 6, getY());
			break;
		}
	}
	if (m_cabbage < 30)
		m_cabbage++;
	if (m_hitpoint <= 0)
	{
		becomeDead();
		return;
	}
	
}

void NachenBlaster::incHP(int point)
{
	m_hitpoint += point;
	if (m_hitpoint > 50)
		m_hitpoint = 50;
}

void NachenBlaster::sufferDamage(int damage)
{
	m_hitpoint -= damage;
	if (m_hitpoint <= 0)
		becomeDead();
}

void NachenBlaster::incTorpedo()
{
	m_torpedo += 5;
}

int NachenBlaster::getHealth()
{
	return m_hitpoint;
}

int NachenBlaster::getCabbage()
{
	return m_cabbage;
}

int NachenBlaster::getTorpedo()
{
	return m_torpedo;
}




Star::Star()
	:Actor(IID_STAR, nullptr, randInt(0,255), randInt(0, 255), 0, static_cast<double>(randInt(5, 50)) / 100, 3, neutral) {}

Star::Star(int newStar)
	:Actor(IID_STAR, nullptr, newStar, randInt(0, 255), 0, static_cast<double>(randInt(5, 50)) / 100, 3, neutral) {}

void Star::doSomething()
{
	moveTo(getX() - 1, getY());
	if (getX() < 0)
		becomeDead();
}

Explosion::Explosion(double startX, double startY)
	:Actor(IID_EXPLOSION, nullptr, startX, startY, 0, 1, 0, neutral) {}

void Explosion::doSomething()
{
	setSize(1.5*getSize());
	if (getSize() == 1.5*1.5*1.5*1.5)
		becomeDead();
}





Projectile::Projectile(int imageID, StudentWorld * pointer, double startX, double startY, int ID, double damageAmt = 2)
	:Actor(imageID, pointer, startX, startY, 0, 0.5, 1, ID), m_damage(damageAmt) {}

int Projectile::getDamage()
{
	return m_damage;
}




Cabbage::Cabbage(double startX, double startY, StudentWorld* pointer)
	: Projectile(IID_CABBAGE, pointer, startX, startY, allyproj) {}

void Cabbage::doSomething()
{
	if (!isAlive())
		return;
	if (getX() >= VIEW_WIDTH)
	{
		becomeDead();
		return;
	}
	int p = getWorld()->isCollided(getX(), getY(), getRadius(), enemy, 2);
	if (p == 0 || p == 1)
	{
		becomeDead();
		if (p == 0)
			getWorld()->playSound(SOUND_DEATH);
		else
			getWorld()->playSound(SOUND_BLAST);
		return;
	}
	moveTo(getX() + 8, getY());
	setDirection(getDirection() + 20);
	p = getWorld()->isCollided(getX(), getY(), getRadius(), enemy, 2);
	if (p == 0 || p == 1)
	{
		becomeDead();
		if (p == 0)
			getWorld()->playSound(SOUND_DEATH);
		else
			getWorld()->playSound(SOUND_BLAST);
		return;
	}
}

Turnip::Turnip(double startX, double startY, StudentWorld * pointer)
	: Projectile(IID_TURNIP, pointer, startX, startY, enemyproj) {}

void Turnip::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0)
	{
		becomeDead();
		return;
	}
	if (getWorld()->isCollided(getX(), getY(), getRadius(), 2))
	{
		becomeDead();
		getWorld()->playSound(SOUND_BLAST);
		return;
	}
	moveTo(getX() - 6, getY());
	setDirection(getDirection() + 20);
	if (getWorld()->isCollided(getX(), getY(), getRadius(), 2))
	{
		becomeDead();
		getWorld()->playSound(SOUND_BLAST);
		return;
	}
}

Torpedo::Torpedo(double startX, double startY, StudentWorld * pointer, int ID)
	: Projectile(IID_TORPEDO, pointer, startX, startY, ID, 8)
{
	if (getID() == enemyproj)
		setDirection(180);
}

void Torpedo::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0 || getX() >= VIEW_WIDTH)
	{
		becomeDead();
		return;
	}
	if (getID() == allyproj)
	{
		int p = getWorld()->isCollided(getX(), getY(), getRadius(), enemy, 8);
		if (p == 0 || p == 1)
		{
			becomeDead();
			if (p == 0)
				getWorld()->playSound(SOUND_DEATH);
			else
				getWorld()->playSound(SOUND_BLAST);
			return;
		}
		moveTo(getX() + 8, getY());
		p = getWorld()->isCollided(getX(), getY(), getRadius(), enemy, 8);
		if (p == 0 || p == 1)
		{
			becomeDead();
			if (p == 0)
				getWorld()->playSound(SOUND_DEATH);
			else
				getWorld()->playSound(SOUND_BLAST);
			return;
		}
	}
	if (getID() == enemyproj)
	{
		if (getWorld()->isCollided(getX(), getY(), getRadius(), 8))
		{
			becomeDead();
			getWorld()->playSound(SOUND_BLAST);
			return;
		}
		moveTo(getX() - 8, getY());
		if (getWorld()->isCollided(getX(), getY(), getRadius(), 8))
		{
			becomeDead();
			getWorld()->playSound(SOUND_BLAST);
			return;
		}
	}
}





Goodie::Goodie(int imageID, StudentWorld * pointer, double startX, double startY)
	:Actor(imageID, pointer, startX, startY, 0, 0.5, 1, neutral) {}

bool Goodie::collisionCheck()
{
	if (getWorld()->isCollided(getX(), getY(), getRadius()))
	{
		getWorld()->increaseScore(100);
		becomeDead();
		getWorld()->playSound(SOUND_GOODIE);
		return true;
	}
	else
		return false;
}

ExtraLife::ExtraLife(double startX, double startY, StudentWorld * pointer)
	: Goodie(IID_LIFE_GOODIE, pointer, startX, startY) {}

void ExtraLife::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0 || getY() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
	{
		getWorld()->incLives();
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	if (collisionCheck())
	{
		getWorld()->incLives();
		return;
	}
}

Repair::Repair(double startX, double startY, StudentWorld * pointer)
	: Goodie(IID_REPAIR_GOODIE, pointer, startX, startY) {}

void Repair::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0 || getY() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
	{
		getWorld()->getPlayer()->incHP(10);
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	if (collisionCheck())
	{
		getWorld()->getPlayer()->incHP(10);
		return;
	}
}

TorpedoGoodie::TorpedoGoodie(double startX, double startY, StudentWorld * pointer)
	: Goodie(IID_TORPEDO_GOODIE, pointer, startX, startY) {}

void TorpedoGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0 || getY() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
	{
		getWorld()->getPlayer()->incTorpedo();
		return;
	}
	moveTo(getX() - 0.75, getY() - 0.75);
	if (collisionCheck())
	{
		getWorld()->getPlayer()->incTorpedo();
		return;
	}
}




Alien::Alien(double startX, double startY, const int ID, StudentWorld * pointer, double speed)
	: Actor(ID, pointer, startX, startY, 0, 1.5, 1, enemy), m_speed(speed) {}

void Alien::becomeDead()
{
	Actor::becomeDead();
	getWorld()->decAlien();
}

void Alien::initHP(int n)
{
	m_hitpoint = n * 5 * (1 + .1*(getWorld()->getLevel() - 1));
}

int Alien::getFPlan()
{
	return m_flightplan;
}

void Alien::decFPlan()
{
	m_flightplan--;
}

void Alien::setFPlan(int length)
{
	m_flightplan = length;
}

double Alien::getSpeed()
{
	return m_speed;
}

void Alien::setSpeed(double speed)
{
	m_speed = speed;
}

int Alien::getTravelDirection()
{
	return m_travelDirection;
}

void Alien::setTravelDirection(int direction)
{
	m_travelDirection = direction;
}

void Alien::sufferDamage(int damage)
{
	m_hitpoint -= damage;
	if (m_hitpoint <= 0)
	{
		becomeDead();
		getWorld()->AlienKilled();
		getWorld()->increaseScore(250);
		getWorld()->newActor(newExplosion, getX(), getY());
		getWorld()->playSound(SOUND_DEATH);
	}
}

bool Alien::collisionCheck()
{
	// collide with ally projectile
	int damage = getWorld()->isCollided(getX(), getY(), getRadius(), allyproj, 0);
	if (damage != -1)
	{
		sufferDamage(damage);
		if (!isAlive())
			return true;
		else
		{
			getWorld()->playSound(SOUND_BLAST);
			return false;
		}
	}
	// collide with nachenblaster
	if (getWorld()->isCollided(getX(), getY(), getRadius(), 0))
	{
		getWorld()->getPlayer()->sufferDamage(5);
		sufferDamage(999);
		return true;
	}
	return false;
}




Smallgon::Smallgon(double startX, double startY, StudentWorld * pointer)
	: Alien(startX, startY, IID_SMALLGON, pointer) {
	initHP();
}

void Smallgon::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
		return;
	if (getFPlan() == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1)
	{
		if (getY() >= VIEW_HEIGHT - 1)
			setTravelDirection(225);
		else if (getY() <= 0)
			setTravelDirection(135);
		else if (getFPlan() == 0)
		{
			int c = randInt(1, 3);
			switch (c)
			{
			case 1:
				setTravelDirection(135);
				break;
			case 2:
				setTravelDirection(180);
				break;
			case 3:
				setTravelDirection(225);
				break;
			}
		}
		setFPlan(randInt(1, 32));
	}
	if (getWorld()->getPlayer()->getX() < getX() &&
		getWorld()->getPlayer()->getY() >= getY() - 4 &&
		getWorld()->getPlayer()->getY() <= getY() + 4)
	{
		if (randInt(1, 20 + 5 * getWorld()->getLevel()) <= getWorld()->getLevel())
		{
			getWorld()->newActor(newTurnip, getX() - 14, getY());
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return;
		}
	}
	int d = getTravelDirection();
	switch (d)
	{
	case 135:
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		break;
	case 180:
		moveTo(getX() - getSpeed(), getY());
		break;
	case 225:
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		break;
	}
	decFPlan();
	if (collisionCheck())
		return;
}

Smoregon::Smoregon(double startX, double startY, StudentWorld * pointer)
	: Alien(startX, startY, IID_SMOREGON, pointer) {
	initHP();
}

void Smoregon::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
	{
		if (randInt(1, 3) == 1)
			if (randInt(1, 2) == 1)
				getWorld()->newActor(newRepair, getX(), getY());
			else
				getWorld()->newActor(newTorpedoGoodie, getX(), getY());
		return;
	}
	if (getFPlan() == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1)
	{
		if (getY() >= VIEW_HEIGHT - 1)
			setTravelDirection(225);
		else if (getY() <= 0)
			setTravelDirection(135);
		else if (getFPlan() == 0)
		{
			int c = randInt(1, 3);
			switch (c)
			{
			case 1:
				setTravelDirection(135);
				break;
			case 2:
				setTravelDirection(180);
				break;
			case 3:
				setTravelDirection(225);
				break;
			}
		}
		setFPlan(randInt(1, 32));
	}
	if (getWorld()->getPlayer()->getX() < getX() &&
		getWorld()->getPlayer()->getY() >= getY() - 4 &&
		getWorld()->getPlayer()->getY() <= getY() + 4)
	{
		if (randInt(1, 20 + 5 * getWorld()->getLevel()) <= getWorld()->getLevel())
		{
			getWorld()->newActor(newTurnip, getX() - 14, getY());
			getWorld()->playSound(SOUND_ALIEN_SHOOT);
			return;
		}
		if (randInt(1, 20 + 5 * getWorld()->getLevel()) <= getWorld()->getLevel())
		{
			setTravelDirection(180);
			setFPlan(VIEW_WIDTH);
			setSpeed(5);
		}
	}
	int d = getTravelDirection();
	switch (d)
	{
	case 135:
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		break;
	case 180:
		moveTo(getX() - getSpeed(), getY());
		break;
	case 225:
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		break;
	}
	decFPlan();
	if (collisionCheck())
	{
		if (randInt(1, 3) == 1)
			if (randInt(1, 2) == 1)
				getWorld()->newActor(newRepair, getX(), getY());
			else
				getWorld()->newActor(newTorpedoGoodie, getX(), getY());
		return;
	}
}

Snagglegon::Snagglegon(double startX, double startY, StudentWorld * pointer)
	: Alien(startX, startY, IID_SNAGGLEGON, pointer) {
	initHP(2);
	setSpeed(1.75);
}

void Snagglegon::doSomething()
{
	if (!isAlive())
		return;
	if (getX() < 0)
	{
		becomeDead();
		return;
	}
	if (collisionCheck())
	{
		if (randInt(1, 6) == 1)
			getWorld()->newActor(newExtraLife, getX(), getY());
		return;
	}
	if (getY() <= 0 || getY() >= VIEW_HEIGHT - 1)
	{
		if (getY() >= VIEW_HEIGHT - 1)
			setTravelDirection(225);
		else if (getY() <= 0)
			setTravelDirection(135);
	}
	if (getWorld()->getPlayer()->getX() < getX() &&
		getWorld()->getPlayer()->getY() >= getY() - 4 &&
		getWorld()->getPlayer()->getY() <= getY() + 4)
	{
		if (randInt(1, 15 + 10 * getWorld()->getLevel()) <= getWorld()->getLevel())
		{
			getWorld()->newActor(newTorpedo, getX() - 14, getY(), enemyproj);
			getWorld()->playSound(SOUND_TORPEDO);
			return;
		}
	}
	int d = getTravelDirection();
	switch (d)
	{
	case 135:
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		break;
	case 225:
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		break;
	}
	if (collisionCheck())
	{
		if (randInt(1, 6) == 1)
			getWorld()->newActor(newExtraLife, getX(), getY());
		return;
	}
}

bool Snagglegon::collisionCheck()
{
	if (Alien::collisionCheck())
	{
		getWorld()->increaseScore(750);
		return true;
	}
	else
		return false;
}

void Snagglegon::sufferDamage(int damage)
{
	Alien::sufferDamage(damage);
	if (getWorld()->isCollided(getX(), getY(), getRadius(), 0))
		getWorld()->getPlayer()->sufferDamage(10);
	if (!isAlive())
		getWorld()->increaseScore(750);
}

