#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "FallingBamboo.h"
#include "GreenEnemy.h"
#include "PlayerHealth.h"
#include "SnakeEnemy.h"
#include "BeardEnemy.h"
#include "MovingLog.h"
#include "DynamicObjects.h"
#include <vector>





// Scene contains all the entities of our game.
// It is responsible for updating and render them.
struct GreenSpawnPoint {
	glm::ivec2 position;
	GreenEnemy* enemy = nullptr;
	bool wasVisibleLastFrame = false;
};

struct SnakeSpawnPoint {
	glm::ivec2 position;
	SnakeEnemy* enemy = nullptr;
	bool wasVisibleLastFrame = false;
};

struct BeardSpawnPoint {
	glm::ivec2 position;
	BeardEnemy* enemy = nullptr;
	bool wasVisibleLastFrame = false;
};

struct BambooSpawnPoint {
	glm::ivec2 position;
	FallingBamboo* enemy = nullptr;
	bool wasVisibleLastFrame = false;
	float timer = 0.0f;
	int respawnInterval = 1000;
	bool disable = false;

};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void configCam();

	int getCameraLeft() const { return left; }
	int getCameraRight() const { return right; }

private:
	void initShaders();
	void spawnBamboo();
	void spawnGreenEnemy();

	void loadGreenSpawns();
	void loadSnakeSpawns();
	void loadBeardSpawns();
	void loadBambooSpawns();

	void checkSnakeSpawn();
	void checkGreenSpawn();
	void checkBeardSpawn();
	void checkBambooSpawn();

private:
	TileMap* map;
	Player* player;

	std::vector<GreenEnemy*> greenEnemies;
	std::vector<GreenSpawnPoint> greenSpawnPoints;

	std::vector<SnakeEnemy*> snakeEnemies;
	std::vector<SnakeSpawnPoint> snakeSpawnPoints;

	std::vector<BeardEnemy*> beardEnemies;
	std::vector<BeardSpawnPoint> beardSpawnPoints;

	std::vector<FallingBamboo*> bamboos;
	std::vector<BambooSpawnPoint> bambooSpawnPoints;

	std::vector<MovingLog*> woods;
	DynamicObjects* dynamicObjects;

	PlayerHealth playerHealth;

	ShaderProgram texProgram;
	float currentTime;
	float bambooSpawnTimer;
	glm::mat4 projection;

	Texture fondo;
	Sprite* liveFondo;

	float camPosX, camPosY, tileSize;
	float projX, projY;

	float left, right, top, bottom;

	int playerInvulTime = 0;  // mil·lisegons d'invulnerabilitat

};


#endif // _SCENE_INCLUDE

