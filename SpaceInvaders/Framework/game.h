// COMP710 GP 2D Framework 2018
#ifndef __GAME_H__
#define __GAME_H__
// standed libaray
#include <vector>
#include "fmod.hpp"




// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class PlayerShip;
class Enemy;
class Bullet;
class Explosion;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void StopMovement();
	void FireSpaceShipBullet();

	

	void SpawnEnemy(int x, int y);
	void SpawnExplosion(int x, int y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:
	

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	float m_playerShipXpos;
	float m_playerShipYPos;
	float m_enemyShipXpos;
	float m_enemyShipYpos;

	bool m_drawDebugInfo;

	FMOD::System     *system = NULL;
	FMOD::Sound      *m_bulletfire, *m_missliefire, *m_explotion, *m_playerSpawn, *m_bulletVsEnemy ;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	// Game Entities:
	// W02.1: Add a PlayerShip field. 
	PlayerShip* m_playerShip;
	// W02.2: Add an alien enemy container field.
	std::vector<Enemy*> m_enemys; // todo make this into an object pool 

	//Enemy* m_enemys = new Enemy[40];

	// W02.3: Add a bullet container field.
	std::vector<Bullet*> m_bullets;
	// explotion container field
	std::vector<Explosion*> m_explosions;

private:

};

#endif // __GAME_H__
