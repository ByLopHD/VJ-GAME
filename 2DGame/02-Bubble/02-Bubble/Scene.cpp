#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "FallingBamboo.h"
#include "PlayerHealth.h"
#include "SnakeEnemy.h"
#include "GreenEnemy.h"
#include "MovingLog.h"
#include "DynamicObjects.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10


Scene::Scene()
{
	map = NULL;
	player = NULL;
	dynamicObjects = new DynamicObjects();
	//bambooSpawnTimer = 0.f;
	srand(static_cast<unsigned int>(time(0)));
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	/*
	for (auto bamboo : bamboos)
		delete bamboo;
	bamboos.clear();
	*/
	if (dynamicObjects != NULL)
		delete dynamicObjects;
		
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Mapa_orignial_Terra.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	tileSize = float(map->getTileSize());

	camPosX = (7 + 6) * tileSize; //Posicio inicial de la camera
	camPosY = (7 + 1.5) * tileSize;

	projX = 15 * tileSize; //Tamany de la projeccio
	projY = 15 * tileSize;

	dynamicObjects->init(texProgram);

	configCam();

	loadGreenSpawns();
	loadSnakeSpawns();
	loadBeardSpawns();
	loadBambooSpawns();
	//spawnGreenEnemy();

	// Marges de pantalla
	glm::ivec2 heartScreenPos(left + 8, top + 8);  // 8 píxels de marge
	playerHealth.init(heartScreenPos, texProgram);
}

void Scene::loadBambooSpawns() {
	int tileSize = map->getTileSize();

	BambooSpawnPoint spawn1;
	spawn1.position = glm::ivec2(69 * tileSize, 0 * tileSize);
	spawn1.respawnInterval = 1250;
	bambooSpawnPoints.push_back(spawn1);

	BambooSpawnPoint spawn2;
	spawn2.position = glm::ivec2(81 * tileSize, 0 * tileSize);
	spawn2.respawnInterval = 1250;
	bambooSpawnPoints.push_back(spawn2);

	BambooSpawnPoint spawn3;
	spawn3.position = glm::ivec2(83 * tileSize, 0 * tileSize);
	spawn3.respawnInterval = 1000;
	bambooSpawnPoints.push_back(spawn3);

	BambooSpawnPoint spawn4;
	spawn4.position = glm::ivec2(88 * tileSize, 0 * tileSize);
	spawn4.respawnInterval = 1050;
	bambooSpawnPoints.push_back(spawn4);

	BambooSpawnPoint spawn5;
	spawn5.position = glm::ivec2(93 * tileSize, 0 * tileSize);
	spawn5.respawnInterval = 1200;
	bambooSpawnPoints.push_back(spawn5);

	BambooSpawnPoint spawn6;
	spawn6.position = glm::ivec2(120 * tileSize, 0 * tileSize);
	spawn6.respawnInterval = 1150;
	bambooSpawnPoints.push_back(spawn6);

	BambooSpawnPoint spawn7;
	spawn7.position = glm::ivec2(123 * tileSize, 0 * tileSize);
	spawn7.respawnInterval = 1100;
	bambooSpawnPoints.push_back(spawn7);

	BambooSpawnPoint spawn8;
	spawn8.position = glm::ivec2(201 * tileSize, 30 * tileSize);
	spawn8.respawnInterval = 1000;
	bambooSpawnPoints.push_back(spawn8);

	BambooSpawnPoint spawn9;
	spawn9.position = glm::ivec2(200 * tileSize, 30 * tileSize);
	spawn9.respawnInterval = 1000;
	bambooSpawnPoints.push_back(spawn9);
}


void Scene::loadBeardSpawns() {
	int tileSize = map->getTileSize();
	beardSpawnPoints.push_back({ glm::ivec2(110 * tileSize, 7 * tileSize), false });
	beardSpawnPoints.push_back({ glm::ivec2(140 * tileSize, 99 * tileSize), false });

}

void Scene::loadSnakeSpawns() {
	int tileSize = map->getTileSize();

	snakeSpawnPoints.push_back({ glm::ivec2(153 * tileSize, 101 * tileSize), false});
	snakeSpawnPoints.push_back({ glm::ivec2(167 * tileSize, 101 * tileSize), false});
}


void Scene::loadGreenSpawns() {
	int tileSize = map->getTileSize();

	greenSpawnPoints.push_back({ glm::ivec2(17 * tileSize, 2 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(30 * tileSize, 2 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(45 * tileSize, 0 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(54 * tileSize, 0 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(201 * tileSize, 40 * tileSize), false});
}



void Scene::checkBambooSpawn() {
	for (auto& sp : bambooSpawnPoints) {
		int x = sp.position.x;
		int y = sp.position.y;
		bool visible = (right - 2 * tileSize >= x && left - 2 * tileSize <= x &&
			top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);
		if (!sp.disable && visible && !sp.wasVisibleLastFrame && sp.enemy == nullptr) {
			FallingBamboo* bamboo = new FallingBamboo();
			bamboo->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			bamboo->setPosition(glm::vec2(x, y));
			bamboo->setTileMap(map);
			bamboos.push_back(bamboo);
			sp.enemy = bamboo;
		}
		sp.wasVisibleLastFrame = visible;
	}
}


void Scene::checkBeardSpawn() {
	for (auto& sp : beardSpawnPoints) {
		int x = sp.position.x;
		int y = sp.position.y;
		bool visible = (right - 2 * tileSize >= x && left - 2 * tileSize <= x &&
			top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);
		if (visible && !sp.wasVisibleLastFrame && sp.enemy == nullptr) {
			BeardEnemy* beard = new BeardEnemy();
			beard->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			beard->setPosition(glm::vec2(x, y));
			beard->setTileMap(map);
			beardEnemies.push_back(beard);
			sp.enemy = beard;
		}
		sp.wasVisibleLastFrame = visible;
	}
}

void Scene::checkSnakeSpawn() {
	for (auto& sp : snakeSpawnPoints) {
		int x = sp.position.x;
		int y = sp.position.y;

		bool visible = (right - 2 * tileSize >= x && left - 2 * tileSize <= x &&
			top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);

		if (visible && !sp.wasVisibleLastFrame && sp.enemy == nullptr) {
			SnakeEnemy* snake = new SnakeEnemy();
			snake->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			snake->setPosition(glm::vec2(x, y));
			snake->setTileMap(map);
			snake->setSceneReference(this);
			snakeEnemies.push_back(snake);
			sp.enemy = snake;
		}
		sp.wasVisibleLastFrame = visible;
	}
}


void Scene::checkGreenSpawn() {
	glm::vec2 posPlayer = player->getPosition();
	
	glm::vec2 camPos = map->getMinCoords();

	for (auto& sp : greenSpawnPoints) {
		int x = sp.position.x;
		int y = sp.position.y;

		//bool visible = (posPlayer.x + 8 * tileSize >= x && posPlayer.x - 8*tileSize <= x);
		bool visible = (right - 2 * tileSize >= x && left - (2 * tileSize) <= x && top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);


		if (visible &&!sp.wasVisibleLastFrame && sp.enemy == nullptr) {
			// Spawn!
			GreenEnemy* enemy = new GreenEnemy();
			enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			enemy->setPosition(glm::vec2(x, y));
			enemy->setTileMap(map);
			greenEnemies.push_back(enemy);
			sp.enemy = enemy;;
		}
		sp.wasVisibleLastFrame = visible;
	}
}


void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	bambooSpawnTimer += deltaTime / 1000.f;

	
	dynamicObjects->update(deltaTime);
	glm::ivec2& playerPos = player->getPositionRef();
	bool onMovingPlatform = dynamicObjects->isOnMovingPlatform(playerPos);
	if (onMovingPlatform)
		dynamicObjects->applyPlatformMovement(playerPos);
	player->setOnMovingLog(onMovingPlatform);

	player->update(deltaTime);


	configCam();
	checkGreenSpawn();
	checkSnakeSpawn();
	checkBeardSpawn();
	checkBambooSpawn();

	for (int i = 0; i < playerHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 8, top + 8 + i * 12); // espaiat vertical de 12
		playerHealth.setHeartPosition(i, heartPos);
	}

	playerHealth.render();

	glm::vec2 ppos = player->getPosition();

	// 🟢 Spawn bamboos segons el timer
	for (auto& sp : bambooSpawnPoints) {
		if (sp.enemy == nullptr && sp.timer >= 0.f) {
			sp.timer += deltaTime;
			if (sp.timer >= sp.respawnInterval) {
				FallingBamboo* b = new FallingBamboo();
				b->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				b->setTileMap(map);
				b->setPosition(glm::vec2(static_cast<float>(sp.position.x), static_cast<float>(sp.position.y)));

				bamboos.push_back(b);
				sp.enemy = b;
				sp.timer = 0.0f;
			}
		}
	}

	// Iterar sobre bamboos actius
	for (int i = 0; i < bamboos.size(); ) {
		FallingBamboo* b = bamboos[i];
		if (b == nullptr) {
			++i;
			continue;
		}
		glm::ivec2 posen = b->getPosition();
		bool isVisible = (right - 2 * tileSize >= posen.x && left - 3 * tileSize <= posen.x &&
			top - 2 * tileSize <= posen.y && bottom - 2 * tileSize >= posen.y);

		if (!isVisible) {
			for (auto& sp : bambooSpawnPoints)
				if (sp.enemy == b) sp.enemy = nullptr;

			delete b;
			bamboos.erase(bamboos.begin() + i);
		}
		else {
			b->setPlayerPosition(player->getPosition());
			b->update(deltaTime);
			++i;
		}
	}

	for (int i = 0; i < beardEnemies.size(); ) {
		BeardEnemy* b = beardEnemies[i];
		glm::ivec2 posen = b->getPosition();
		bool isVisible = (right - 2 * tileSize >= posen.x && left - 3 * tileSize <= posen.x &&
			top - 2 * tileSize <= posen.y && bottom - 2 * tileSize >= posen.y);
		if (!isVisible) {
			for (auto& sp : beardSpawnPoints) {
				if (sp.enemy == b)
					sp.enemy = nullptr;
			}
			delete b;
			beardEnemies.erase(beardEnemies.begin() + i);
		}
		else {
			b->setPlayerPosition(player->getPosition());
			b->update(deltaTime);
			++i;
		}
	}

	for (int i = 0; i < snakeEnemies.size(); ) {
		SnakeEnemy* s = snakeEnemies[i];
		glm::ivec2 posen = s->getPosition();

		bool isVisible = (right - 2 * tileSize >= posen.x && left - 3 * tileSize <= posen.x &&
			top - 2 * tileSize <= posen.y && bottom - 2 * tileSize >= posen.y && posen.x >= 144*tileSize && posen.x <= 191*tileSize);

		if (!isVisible) {
			for (auto& sp : snakeSpawnPoints) {
				if (sp.enemy == s)
					sp.enemy = nullptr;
			}
			delete s;
			snakeEnemies.erase(snakeEnemies.begin() + i);
		}
		else {
			s->update(deltaTime);
			++i;
		}
	}

	for (int i = 0; i < greenEnemies.size(); ) {
		GreenEnemy* e = greenEnemies[i];
		glm::ivec2 posen = e->getPosition();

		bool isVisible = (right - 2 * tileSize >= posen.x && left - 3 * tileSize <= posen.x && top - 2 * tileSize <= posen.y && bottom - 2 * tileSize >= posen.y);

		if (!isVisible) {
			for (auto& sp : greenSpawnPoints) {
				if (sp.enemy == e)
					sp.enemy = nullptr;
			}
			delete e;
			greenEnemies.erase(greenEnemies.begin() + i);
		}
		else {
			e->setPlayerPosition(player->getPosition());
			e->update(deltaTime);
			++i;
		}
	}


	glm::ivec2 playerSize = glm::ivec2(23, 32);
	glm::ivec2 playerOffset = glm::ivec2(2, 0);
	glm::ivec2 pHitPos = glm::ivec2(ppos) + playerOffset;

	for (auto& enemy : greenEnemies) {
		glm::ivec2 enemyPos = enemy->getPosition();
		glm::ivec2 enemySize = glm::ivec2(16, 32);
		glm::ivec2 enemyOffset = glm::ivec2(8, 0);
		glm::ivec2 eHitPos = enemyPos + enemyOffset;

		bool overlapX = pHitPos.x < eHitPos.x + enemySize.x &&
			pHitPos.x + playerSize.x > eHitPos.x;

		bool overlapY = pHitPos.y < eHitPos.y + enemySize.y &&
			pHitPos.y + playerSize.y > eHitPos.y;

		if (playerInvulTime <= 0 && overlapX && overlapY) {
			playerHealth.takeDamage(0.25f);
			playerInvulTime = 2000;

			player->animacioDamage();
			break;
		}
	}

	for (auto& enemy : snakeEnemies) {
		glm::ivec2 enemyPos = enemy->getPosition();
		glm::ivec2 enemySize = glm::ivec2(24, 16);
		glm::ivec2 enemyOffset = glm::ivec2(4, 0); // subimos 4px la hitbox
		glm::ivec2 eHitPos = enemyPos + enemyOffset;

		bool overlapX = pHitPos.x < eHitPos.x + enemySize.x &&
			pHitPos.x + playerSize.x > eHitPos.x;

		bool overlapY = pHitPos.y < eHitPos.y + enemySize.y &&
			pHitPos.y + playerSize.y > eHitPos.y;

		if (playerInvulTime <= 0 && overlapX && overlapY) {
			playerHealth.takeDamage(0.25f);
			playerInvulTime = 2000;

			player->animacioDamage();
			break;
		}
	}

	for (auto& enemy : beardEnemies) {
		glm::ivec2 enemyPos = enemy->getPosition();
		glm::ivec2 enemySize = glm::ivec2(32, 32);
		glm::ivec2 enemyOffset = glm::ivec2(0, 0);
		glm::ivec2 eHitPos = enemyPos + enemyOffset;
		bool overlapX = pHitPos.x < eHitPos.x + enemySize.x &&
			pHitPos.x + playerSize.x > eHitPos.x;
		bool overlapY = pHitPos.y < eHitPos.y + enemySize.y &&
			pHitPos.y + playerSize.y > eHitPos.y;
		if (playerInvulTime <= 0 && overlapX && overlapY) {
			playerHealth.takeDamage(0.25f);
			playerInvulTime = 2000;
			player->animacioDamage();
			break;
		}
	}

	for (auto& b : bamboos) {
		glm::ivec2 bambooPos = b->getPosition();
		glm::ivec2 bambooSize = glm::ivec2(8, 16);
		glm::ivec2 bambooOffset = glm::ivec2(0, 0);
		glm::ivec2 bHitPos = bambooPos + bambooOffset;

		bool overlapX = pHitPos.x < bHitPos.x + bambooSize.x &&
			pHitPos.x + playerSize.x > bHitPos.x;
		bool overlapY = pHitPos.y < bHitPos.y + bambooSize.y &&
			pHitPos.y + playerSize.y > bHitPos.y;

		// Bloqueig amb llança cap amunt
		int anim = player->getCurrentAnimation();
		bool isBlockingUp = (anim == 12 || anim == 13);

		if (isBlockingUp && overlapX && overlapY) {
			// Bloqueja el bamboo
			b->onBlocked();

			// Troba spawn i elimina
			for (auto& sp : bambooSpawnPoints) {
				if (sp.enemy == b) {
					sp.disable = true;
					sp.enemy = nullptr;
					sp.timer = -1.f;  // no respawn
				}
			}
			break;
		}

		// Si no bloqueja, fa mal
		if (playerInvulTime <= 0 && overlapX && overlapY && !isBlockingUp) {
			playerHealth.takeDamage(0.25f);
			playerInvulTime = 2000;
			player->animacioDamage();
			break;
		}
	}






	/*
	if (ppos.x / tileSize >= 65 && ppos.x / tileSize <= 125 && bambooSpawnTimer > 0.8f) {
		int numBamboos = 1 + rand() % 2; // Entre 1 y 3 bamb�s
		for (int i = 0; i < numBamboos; ++i) {
			spawnBamboo();
		}
		bambooSpawnTimer = 0.f;
	}

	for (auto& bamboo : bamboos) {
		bamboo->setPlayerPosition(glm::ivec2(ppos));
		bamboo->update(deltaTime);
		if (bamboo->checkCollisionWithPlayer()) {
			std::cout << "Jugador golpeado por bamb�!" << std::endl;
		}
	}
	bamboos.erase(std::remove_if(bamboos.begin(), bamboos.end(), [](FallingBamboo* b) {
		bool inactive = !b->isActive();
		if (inactive) delete b;
		return inactive;
		}), bamboos.end());


	*/
	if (playerInvulTime > 0)
		playerInvulTime -= deltaTime;

}

void Scene::configCam() {
	glm::vec2 pos = player->getPosition();
	float aspectR = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	float viewW = projY * aspectR;
	float viewH = projY;
	//float viewH = projY * aspectR;

	if (pos.x + tileSize < 8 * tileSize) {
		//float camX = 8 * tileSize;
		projection = glm::ortho(float(2 * tileSize), float(18 * tileSize), float(16 * tileSize), float(1 * tileSize));
		left = 2 * tileSize;
		right = 18 * tileSize;
		bottom = 16 * tileSize;
		top = 1 * tileSize;
 	}

	else if (pos.x + tileSize > 248 * tileSize) {
		projection = glm::ortho(float(242 * tileSize), float(258 * tileSize), float(46 * tileSize), float(31 * tileSize));
		left = 242 * tileSize;
		right = 258 * tileSize;
		bottom = 46 * tileSize;
		top = 31 * tileSize;
	}

	else if ((pos.x + tileSize > 129 * tileSize && pos.x + tileSize < 145 * tileSize)) {
		if (pos.y + tileSize < (7.5 + 2) * tileSize) {
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), float(16 * tileSize), float(1 * tileSize));
			left = 130 * tileSize;
			right = 146 * tileSize;
			bottom = 16 * tileSize;
			top = 1 * tileSize;
		}
		else if (pos.y + tileSize > (97.5 + 2) * tileSize) {
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), float(106 * tileSize), float(91 * tileSize));
			left = 130 * tileSize;
			right = 146 * tileSize;
			bottom = 106 * tileSize;
			top = 91 * tileSize;
		}
		else {
			float targetY = pos.y;
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), targetY + (float(7.5 * tileSize)), targetY - (float(7.5 * tileSize)));
			left = 130 * tileSize;
			right = 146 * tileSize;
			bottom = targetY + (float(7.5 * tileSize));
			top = targetY - (float(7.5 * tileSize));
		}
	}

	else if (pos.x + tileSize > 193 * tileSize && pos.x + tileSize < 208 * tileSize) {

		if (pos.y + tileSize < (37.5 + 2) * tileSize) {
			projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), float(46 * tileSize), float(31 * tileSize));
			left = 194 * tileSize;
			right = 209 * tileSize;
			bottom = 46 * tileSize;
			top = 31 * tileSize;
		}

		else if (pos.y + tileSize > (97.5 + 2) * tileSize) {
			projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), float(106 * tileSize), float(91 * tileSize));
			left = 194 * tileSize;
			right = 209 * tileSize;
			bottom = 106 * tileSize;
			top = 91 * tileSize;
		}
		else {
			float targetY = pos.y;
			projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), targetY + (float(7.5 * tileSize)), targetY - (float(7.5 * tileSize)));
			left = 194 * tileSize;
			right = 209 * tileSize;
			bottom = targetY + (float(7.5 * tileSize));
			top = targetY - (float(7.5 * tileSize));
		}
	}

	else {
		// Centrar el personatge a la pantalla amb 7 tiles a l'esquerra i 7 a la dreta
		if (pos.x + tileSize < 145 * tileSize) camPosY = (7 + 1.5) * tileSize;
		else if (pos.x + tileSize < 208 * tileSize) camPosY = (97 + 1.5) * tileSize;
		else camPosY = (37 + 1.5) * tileSize;

		float targetX = pos.x + (3 * tileSize);

		projection = glm::ortho(targetX - (float(8 * tileSize)), targetX + (float(8 * tileSize)), camPosY + projY / 2.0f, camPosY - projY / 2.0f);

		left = targetX - (float(8 * tileSize));
		right = targetX + (float(8 * tileSize));
		bottom = camPosY + projY / 2.0f;
		top = camPosY - projY / 2.0f;
		

		//camPosX = targetX + viewH / 2.0f;

	}
	currentTime = 0.0f;
}

float clamp(float value, float minVal, float maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}


void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	dynamicObjects->render();
	player->render();
	for (auto& bamboo : bamboos)
		bamboo->render();

	for (auto& enemy : greenEnemies)
		enemy->render();

	for (auto& snake : snakeEnemies)
		snake->render();

	for (auto& beard : beardEnemies)
		beard->render();

	// Reposicionar els cors segons la càmera
	for (int i = 0; i < playerHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 8, top + 8 + i * 12); // marges i separació vertical
		playerHealth.setHeartPosition(i, heartPos);
	}

	// Mostrar els cors
	playerHealth.render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}