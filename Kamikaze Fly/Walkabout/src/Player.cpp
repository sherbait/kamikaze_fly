#include "Player.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\gui\ScreenGUI.h"
#include "Box2D\Box2D.h"
#include "WalkaboutGame.h"

Player::Player()
{
	health = HEALTH; lives = 2;hit= false;timer = 0;
	ammo = AMMO;
	Game* game = Game::getInstance();
	ScreenGUI* playerGUI = game->getGUI()->getScreen(GS_GAME_IN_PROGRESS);

	int imageID = game->getGraphics()->getGUITextureManager()->loadTexture(W_HEART_PATH);
	for (int i = 0; i < HEALTH; i++)
	{
		OverlayImage *heart = new OverlayImage(imageID,10 + i*30,10,0,255,26,26);
		healthImages[i] = heart;
		playerGUI->addOverlayImage(heart);
	}

	imageID = game->getGraphics()->getGUITextureManager()->loadTexture(W_BULLET_PATH);
	for (int i = 0; i < AMMO; i++)
	{
		OverlayImage *bullet = new OverlayImage(imageID,10 + i*30,36,0,255,26,26);
		ammoImages[i] = bullet;
		playerGUI->addOverlayImage(bullet);
	}

	collisionType = CollisionType::PLAYER;
}

void Player::update()
{
	float vX = getPhysicalProperties()->rigidBody->GetLinearVelocity().x;
	float vY = getPhysicalProperties()->rigidBody->GetLinearVelocity().y;

	Game* game = Game::getInstance();

	list<Goal*>::iterator goalsIterator = game->getGSM()->getSpriteManager()->getGoalsIterator();
	list<Goal*>::iterator endOfGoalsIterator = game->getGSM()->getSpriteManager()->getEndOfGoalsIterator();
	if(goalsIterator == endOfGoalsIterator)
	{
		Game::getInstance()->getGSM()->goToWinScreen();
	}
	else if(health < 1 && !getIsDying())
	{
		bool gameIsLost = false;
		while (goalsIterator != endOfGoalsIterator)
		{			
			Goal *goal = (*goalsIterator);
			if(!goal->getIsDying() && !goal->getIsDead())
			{
				gameIsLost = true;
			}
			goalsIterator++;
		}

		if(gameIsLost)
		{
			game->getGSM()->gameOver();
		}
	}

	for (int i = 0; i < AMMO; i++)
	{
		if (i < ammo)
		{
			ammoImages[i]->alpha = 255;
		}
		else
		{
			ammoImages[i]->alpha = 0;
		}
	}

	if(vX < 0)
	{
		b2Vec2 velocity;
		velocity.Set(PLAYER_SPEED,vY);
		getPhysicalProperties()->rigidBody->SetLinearVelocity(velocity);
	}

	--timer;

	if(!dyingLastFrame && this->getIsDying())
	{
		health--;

		for (int i = 0; i < HEALTH; i++)
		{
			if (i < health)
			{
				healthImages[i]->alpha = 255;
			}
			else
			{
				healthImages[i]->alpha = 0;
			}
		}

		dyingLastFrame = true;
	}
	else if(!this->getIsDying())
	{
		dyingLastFrame = false;
	}
}

void Player::handlePreSolve(b2Contact* contact, CollidableObject* otherCollidableObject)
{

}

void Player::handlePostSolve(b2Contact* contact,const b2ContactImpulse* impulse,CollidableObject* otherCollidableObject)
{

}