// COMP710 GP 2D Framework 2018

// This includes:
#include "game.h"
#include "PlayerShip.h"
#include "Enemy.h"
#include "Bullet.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "explosion.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "animatedsprite.h"


// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <vector>
#include <string>
//#include <fmod_errors.h>


// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
	: m_pBackBuffer(0)
	, m_pInputHandler(0)
	, m_looping(true)
	, m_executionTime(0)
	, m_elapsedSeconds(0)
	, m_frameCount(0)
	, m_FPS(0)
	, m_numUpdates(0)
	, m_lastTime(0)
	, m_lag(0)
	, m_playerShipXpos(400)
	, m_playerShipYPos(500)
	
	, m_playerShip(0)
	, m_enemyShipXpos(0)
	,m_enemyShipYpos(0)
	
{
	
}

Game::~Game()
{
	delete m_playerShip;
	m_playerShip = 0;
	std::vector<Enemy*>::iterator it;
	for (it = m_enemys.begin(); it != m_enemys.end(); ++it)
	{
		Enemy* enamy = *it;
		delete enamy;
		enamy = 0;

	}
	std::vector<Bullet*>::iterator bullet_delelte_it;
	for (bullet_delelte_it = m_bullets.begin(); bullet_delelte_it != m_bullets.end(); ++bullet_delelte_it)
	{
		Bullet* bullet = *bullet_delelte_it;
		delete bullet;
		bullet = 0;
	}

	std::vector<Explosion*>::iterator explotion_delete_it;
	for (explotion_delete_it = m_explosions.begin(); explotion_delete_it != m_explosions.end(); ++explotion_delete_it)
	{
		Explosion* explotion = *explotion_delete_it;
		delete explotion;
		explotion = 0;
	}

	delete m_pInputHandler;
	m_pInputHandler = 0;
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	result = m_bulletfire->release();
	result = m_bulletVsEnemy->release();

	result = system->close();
	
	result = system->release();
	

	
}

bool 
Game::Initialise()
{
	

	result = FMOD::System_Create(&system);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = system->createSound("assets\\bullet.wav", FMOD_CREATESAMPLE, 0, &m_bulletfire);
	result = system->createSound("assets\\hit.wav", FMOD_CREATESAMPLE, 0, &m_bulletVsEnemy);

	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// W02.1: Load the player ship sprite.
	// For example: Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	
	// W02.1: Create the player ship instance.
	m_playerShip = new PlayerShip();
	m_playerShip->Initialise(pPlayerSprite);
	m_playerShip->SetPositionX(m_playerShipXpos);
	m_playerShip->SetPositionY(m_playerShipYPos);



	// W02.2: Spawn four rows of 14 alien enemies.
	
	int spawnYpos = 25;
	for (int row = 0; row < 5; ++row)
	{
		int spawnXpos = 45;
		for (int colom = 0; colom < 14; ++colom)
		{
			Game::SpawnEnemy(spawnXpos, spawnYpos);
			spawnXpos += 50;
		}
		spawnYpos += 40;
	}

	

	// W02.2: Fill the container with these new enemies.

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;
	// fmod INIT
	//FMOD_RESULT result;


	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}

//	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:

	// Ex003.5: Process each alien enemy in the container.

	// W02.3: Process each bullet in the container.
	std::vector<Bullet*>::iterator bullet_iter;
	for (bullet_iter = m_bullets.begin(); bullet_iter != m_bullets.end();)
	{
		Bullet* bullet = *bullet_iter;
		bullet->Process(deltaTime);
		if (bullet->GetPositionY() <= 0)
		{		
			bullet_iter = m_bullets.erase(bullet_iter);
			delete bullet;
			bullet = 0;

			//bullet->SetDead(true);	
		}
		else
		{
			++bullet_iter;
		}
	}

	// W02.1: Update player...
	m_playerShip->Process(deltaTime);

	// W02.3: Check for bullet vs alien enemy collisions...
	std::vector<Bullet*>::iterator bullet_iter2;
	for (bullet_iter2 = m_bullets.begin(); bullet_iter2 != m_bullets.end();)
	{
		Bullet* bullet_coliding = *bullet_iter2;
		std::vector<Enemy*>::iterator enemy_iter;
		for (enemy_iter = m_enemys.begin(); enemy_iter != m_enemys.end(); )
		{
			Enemy* enemy_coliding = *enemy_iter;
			if (bullet_coliding->IsCollidingWith(*enemy_coliding))
			{
				bullet_coliding->SetDead(true);
				enemy_coliding->SetDead(true);
				if (enemy_coliding->IsDead())
				{
					result = system->playSound(m_bulletVsEnemy, 0, false, &channel);
					SpawnExplosion(static_cast<int>(enemy_coliding->GetPositionX()),static_cast<int>( enemy_coliding->GetPositionY()));
					std::string test = std::to_string(enemy_coliding->GetPositionX());
					std::string test2 = std::to_string(enemy_coliding->GetPositionY());

					LogManager::GetInstance().Log(test.c_str());
					LogManager::GetInstance().Log(test2.c_str());
					enemy_iter = m_enemys.erase(enemy_iter);
					delete enemy_coliding;
					enemy_coliding = 0;
				}
				
			}
			else
			{
				++enemy_iter;
			}
		}
		if (bullet_coliding->IsDead())
		{
			bullet_iter2 = m_bullets.erase(bullet_iter2);
			delete bullet_coliding;
			bullet_coliding = 0;

		}
		else
		{
			++bullet_iter2;
		}

	}
	
	system->update();
//	FMOD_RESULT Studio::System::update();
	
	// W02.3: For each bullet
	// W02.3: For each alien enemy
	// W02.3: Check collision between two entities.
	// W02.3: If collided, destory both and spawn explosion.
	

	// W02.3: Remove any dead bullets from the container...
	

	// W02.3: Remove any dead enemy aliens from the container...

	// W02.3: Remove any dead explosions from the container...

	std::vector<Explosion*>::iterator explotion_iter;
	for (explotion_iter = m_explosions.begin(); explotion_iter != m_explosions.end(); ++explotion_iter)
	{
		Explosion*  p = *explotion_iter;
		p->Process(deltaTime);
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	// W02.2: Draw all enemy aliens in container...
	std::vector<Enemy* >::iterator it;
	for (it = m_enemys.begin(); it != m_enemys.end(); ++it)
	{
		Enemy* enemy = *it;
		enemy->Draw(backBuffer);
	}
	
	// W02.3: Draw all bullets in container...
	std::vector<Bullet* >::iterator bullet_it;
	for (bullet_it = m_bullets.begin(); bullet_it != m_bullets.end(); ++bullet_it)
	{
		Bullet* bullet = *bullet_it;
		bullet->Draw(backBuffer);
	}

	std::vector<Explosion*>::iterator explotions_it;
	for (explotions_it = m_explosions.begin(); explotions_it != m_explosions.end(); ++explotions_it)
	{
		Explosion* explode = *explotions_it;
		explode->Draw(backBuffer);
	}
	

	// W02.1: Draw the player ship...
	m_playerShip->Draw(backBuffer);
	
	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void 
Game::MoveSpaceShipLeft()
{
	// W02.1: Tell the player ship to move left.
	m_playerShip->SetHorizontalVelocity(-100);        
}

// W02.1: Add the method to tell the player ship to move right...
void
Game::MoveSpaceShipRight()
{
	m_playerShip->SetHorizontalVelocity(100);
}

void
Game::StopMovement()
{
	m_playerShip->SetHorizontalVelocity(0);
}
// W02.3: Space a Bullet in game.
void 
Game::FireSpaceShipBullet()
{
	// W02.3: Load the player bullet sprite. 
	Sprite* pBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	result = system->playSound(m_bulletfire, 0, false, &channel);
	
	//ERRCHECK(result);

	// W02.3: Create a new bullet object.
	Bullet* bullet = new Bullet();
	bullet->Initialise(pBulletSprite);
	bullet->SetPositionX(m_playerShip->GetSpritePositionX() - bullet->GetSpritePositionX());
	bullet->SetPositionY(m_playerShip->GetSpritePositionY()- bullet->GetSpritePositionY());
	

	// W02.3: Set the bullets vertical velocity.
	bullet->SetVerticalVelocity(-200);
	// W02.3: Add the new bullet to the bullet container.
	m_bullets.push_back(bullet);
}

// 12W02.2: Spawn a Enemy in game.
void 
Game::SpawnEnemy(int x, int y)
{
	// W02.2: Load the alien enemy sprite file.
	Sprite* pEnemyShip = m_pBackBuffer->CreateSprite("assets\\alienenemy.png");
	
	

	// W02.2: Create a new Enemy object
	
	Enemy* enemyShip = new Enemy();
	enemyShip->Initialise(pEnemyShip);
	enemyShip->SetPositionX(static_cast<float>(x));
	enemyShip->SetPositionY(static_cast<float>(y));

	// W02.2: Add the new Enemy to the enemy container.
	
	m_enemys.push_back(enemyShip);
}

void
Game::SpawnExplosion(int x, int y)
{
	AnimatedSprite* pExplosion = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");
	pExplosion->AddFrame(0);
	pExplosion->AddFrame(64);
	pExplosion->AddFrame(128);
	pExplosion->AddFrame(192);
	pExplosion->AddFrame(256);
	pExplosion->SetFrameSpeed(0.2f);
	pExplosion->SetFrameWidth(64);
	Explosion* explotion = new Explosion();

	explotion->Initialise(pExplosion);
	explotion->SetPositionX(static_cast<float>(x)- explotion->GetSpritePositionX());
	explotion->SetPositionY(static_cast<float>(y) - explotion->GetSpritePositionY());
	//std::string test3 = std::to_string(x);
	//std::string test4 = std::to_string(y);

	//LogManager::GetInstance().Log(test3.c_str());
	//LogManager::GetInstance().Log(test4.c_str());

	m_explosions.push_back(explotion);
}

