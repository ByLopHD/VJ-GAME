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
#include "BossHealth.h"
#include "Gui.h"
#include "SnakeEnemy.h"
#include "GreenEnemy.h"
#include "MovingLog.h"
#include "DynamicObjects.h"
#include "PowerUp.h"
#include "BeardEnemy.h"
#include "Boss.h"
#include "Lanza.h"


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
	
	
	loadPowerUpSpawns();

	//spawnGreenEnemy();

	// Marges de pantalla
	glm::ivec2 heartScreenPos(left + 8, top + 8);  // 8 píxels de marge
	playerHealth.init(heartScreenPos, texProgram);

	glm::ivec2 bossHeartScreenPos(left + 32, top + 8);  // 8 píxels de marge
	bossHealth.init(bossHeartScreenPos, texProgram);

	glm::ivec2 guiPos(left + 8, top + 8);  // Posició inicial basada en la càmera
	gui.init(guiPos, texProgram);         // Inicialitza GUI
}

void Scene::loadPowerUpSpawns() {
	int tileSize = map->getTileSize();
	//powerUpSpawns.push_back({ glm::ivec2(20 * tileSize, 11 * tileSize), 0, nullptr, false, false }); //COR PETIT
	powerUpSpawns.push_back({ glm::ivec2(155 * tileSize, 99 * tileSize), 0, nullptr, false, false }); //COR PETIT
	
	powerUpSpawns.push_back({ glm::ivec2(125 * tileSize, 3 * tileSize), 1, nullptr, false, false }); //COR GRAN
	powerUpSpawns.push_back({ glm::ivec2(202 * tileSize, 89 * tileSize), 1, nullptr, false, false }); //COR GRAN


	powerUpSpawns.push_back({ glm::ivec2(11 * tileSize, 6 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	
	powerUpSpawns.push_back({ glm::ivec2(63 * tileSize, 4 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	
	powerUpSpawns.push_back({ glm::ivec2(117 * tileSize, 1 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(142 * tileSize, 20 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(143 * tileSize, 85 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(166 * tileSize, 98 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(195 * tileSize, 109 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	
	powerUpSpawns.push_back({ glm::ivec2(211 * tileSize, 37 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(215 * tileSize, 37 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(217 * tileSize, 37 * tileSize), 2, nullptr, false, false }); //PUMPKIN
	powerUpSpawns.push_back({ glm::ivec2(130 * tileSize, 27 * tileSize), 2, nullptr, false, false }); //PUMPKIN

	powerUpSpawns.push_back({ glm::ivec2(142 * tileSize, 99 * tileSize), 3, nullptr, false, false }); //DEER_SHIRT

}

void Scene::spawnBossBamboo(glm::ivec2 pos) {
	FallingBamboo* b = new FallingBamboo();
	b->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	b->setTileMap(map);
	b->setPosition(pos);
	bossBamboos.push_back({ glm::ivec2(pos), b, false });
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
	spawn8.position = glm::ivec2(198 * tileSize, 30 * tileSize);
	spawn8.respawnInterval = 1200;
	bambooSpawnPoints.push_back(spawn8);

	BambooSpawnPoint spawn9;
	spawn9.position = glm::ivec2(200 * tileSize, 30 * tileSize);
	spawn9.respawnInterval = 1000;
	bambooSpawnPoints.push_back(spawn9);
}


void Scene::loadBeardSpawns() {
	int tileSize = map->getTileSize();
	beardSpawnPoints.push_back({ glm::ivec2(110 * tileSize, 7 * tileSize), false });
	//beardSpawnPoints.push_back({ glm::ivec2(140 * tileSize, 99 * tileSize), false });

}

void Scene::loadSnakeSpawns() {
	int tileSize = map->getTileSize();

	snakeSpawnPoints.push_back({ glm::ivec2(153 * tileSize, 101 * tileSize), false});
	snakeSpawnPoints.push_back({ glm::ivec2(167 * tileSize, 101 * tileSize), false});
	snakeSpawnPoints.push_back({ glm::ivec2(177 * tileSize, 101 * tileSize), false});
	snakeSpawnPoints.push_back({ glm::ivec2(183 * tileSize, 101 * tileSize), false});
}


void Scene::loadGreenSpawns() {
	int tileSize = map->getTileSize();

	greenSpawnPoints.push_back({ glm::ivec2(17 * tileSize, 2 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(30 * tileSize, 2 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(45 * tileSize, 0 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(54 * tileSize, 0 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(201 * tileSize, 40 * tileSize), false});

	greenSpawnPoints.push_back({ glm::ivec2(133 * tileSize, 6 * tileSize), false});
	greenSpawnPoints.push_back({ glm::ivec2(139 * tileSize, 18 * tileSize), false });
	greenSpawnPoints.push_back({ glm::ivec2(136 * tileSize, 95 * tileSize), false });

}

void Scene::spawnLeaf(const glm::vec2& center, float angleDeg) {
	Leaf* leaf = new Leaf();
	leaf->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	leaf->setTileMap(map);
	leaf->setInitialPosition(center);
	leaf->setAngle(angleDeg);
	leaves.push_back(leaf);
}


void Scene::checkBossSpawn() {
	if (boss != nullptr) return; // Evita crear més d’un cop

	int x = 248 * tileSize;
	int y = 36 * tileSize;

	bool visible = (right - 2 * tileSize >= x && left - 2 * tileSize <= x &&
		top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);

	if (visible) {
		boss = new Boss();  // ✅ Ara sí, usem la variable de la classe
		boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		boss->setPosition(glm::vec2(x, y));
		boss->setTileMap(map);
		boss->setSceneReference(this);
	}
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

void Scene::checkPowerUpSpawns() {
	for (auto& sp : powerUpSpawns) {
		int x = sp.position.x;
		int y = sp.position.y;

		bool visible = (right - 2 * tileSize >= x && left - 2 * tileSize <= x &&
			top - 1 * tileSize <= y && bottom - 1 * tileSize >= y);

		if (visible && !sp.wasVisibleLastFrame && sp.powerup == nullptr && !sp.collected) {
			PowerUp* p = new PowerUp();
			p->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, sp.type);
			p->setTileMap(map);
			p->setType(sp.type);
			p->setPosition(glm::vec2(x, y));
			powerUps.push_back(p);
			sp.powerup = p;
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
	checkPowerUpSpawns();
	checkBossSpawn();

	// GOD MODE toggle
	static bool gPressed = false;
	if (Game::instance().getKey('G')) {
		if (!gPressed) {
			godMode = !godMode;
			std::cout << "God Mode: " << (godMode ? "ACTIVAT" : "DESACTIVAT") << std::endl;
			gPressed = true;
		}
	}
	else {
		gPressed = false;
	}

	// HEAL key
	static bool hPressed = false;
	if (Game::instance().getKey('H')) {
		if (!hPressed) {
			playerHealth.restoreFull();   // Cors al màxim
			playerHealth.setLives(3);     
			std::cout << "Healed: vida plena i 2 vides" << std::endl;
			hPressed = true;
		}
	}
	else {
		hPressed = false;
	}

	// 64 160
	if (playerHealth.getLives() == 0) {
		player->setPosition(glm::vec2(64, 160));
	}

	for (int i = 0; i < playerHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 8, top + 8 + i * 12); // espaiat vertical de 12
		playerHealth.setHeartPosition(i, heartPos);
	}

	for (int i = 0; i < bossHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 32, top + 8 + i * 12); // espaiat vertical de 12
		bossHealth.setHeartPosition(i, heartPos);
	}


	glm::vec2 guiPos(left + 8, top + 8); // potser una mica més amunt o avall si cal
	gui.setPosition(guiPos);
	gui.setLives(playerHealth.getLives());

	playerHealth.render();
	gui.render();
	bossHealth.render();

	glm::vec2 ppos = player->getPosition();

	if(boss) boss->update(deltaTime);


	// Actualitza les fulles
	for (int i = 0; i < leaves.size(); ) {
		Leaf* leaf = leaves[i];
		leaf->update(deltaTime);

		if (!leaf->isActive() || bossVisible) {
			delete leaf;
			leaves.erase(leaves.begin() + i);
		}
		else {
			++i;
		}
	}


	//power ups
	for (int i = 0; i < powerUps.size(); ) {
		PowerUp* p = powerUps[i];
				
		glm::ivec2 posen = p->getPosition();
		bool isVisible = (right - 2 * tileSize >= posen.x && left - 3 * tileSize <= posen.x &&
			top - 2 * tileSize <= posen.y && bottom - 2 * tileSize >= posen.y);
		
		if (!isVisible) {
			for (auto& sp : powerUpSpawns)
				if (sp.powerup == p) {
					sp.collected = true;
					sp.powerup = nullptr;
				}
			delete p;
			powerUps.erase(powerUps.begin() + i);
		}
		else {
			p->update(deltaTime);

			glm::ivec2 ppos = player->getPosition();
			glm::ivec2 size = glm::ivec2(23, 32);
			glm::ivec2 offset = glm::ivec2(2, 0);
			glm::ivec2 hitPos = ppos + offset;

			if (p->isActive() && p->checkCollision(hitPos, size)) {
				if (p->getType() == 1) {
					playerHealth.restoreFull();
				}
				else if (p->getType() == 0) {
					std::cout << "cor petit" << std::endl;
					playerHealth.restoreOneHeart();
				}
				else if (p->getType() == 2) {
					std::cout << "🍂 Calabassa recollida" << std::endl;
					playerHealth.addPumpkin(texProgram);
				}
				else if (p->getType() == 3) {
					playerInvulTime += 5000; // 5 segons
				}

				// Marcar com recollit i eliminar
				for (auto& sp : powerUpSpawns) {
					if (sp.powerup == p) {
						sp.collected = true;
						sp.powerup = nullptr;
					}
				}
				delete p;
				powerUps.erase(powerUps.begin() + i);
			}
			else {
				++i;
			}
		}

	}


	//Spawn bamboos segons el timer
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

	// Update dels bambús del boss
	for (int i = 0; i < bossBamboos.size(); ) {
		auto& bb = bossBamboos[i];
		FallingBamboo* b = bb.bamboo;
		if (!b->isActive()) {
			delete b;
			bossBamboos.erase(bossBamboos.begin() + i);
		}
		else {
			b->setPlayerPosition(player->getPosition());
			b->update(deltaTime);

			// Collisions amb el jugador
			glm::ivec2 bambooPos = b->getPosition();
			glm::ivec2 bambooSize = glm::ivec2(8, 16);
			glm::ivec2 bambooOffset = glm::ivec2(0, 0);
			glm::ivec2 bHitPos = bambooPos + bambooOffset;

			glm::ivec2 ppos = player->getPosition();
			glm::ivec2 playerSize = glm::ivec2(23, 32);
			glm::ivec2 playerOffset = glm::ivec2(2, 0);
			glm::ivec2 pHitPos = ppos + playerOffset;

			bool overlapX = pHitPos.x < bHitPos.x + bambooSize.x &&
				pHitPos.x + playerSize.x > bHitPos.x;
			bool overlapY = pHitPos.y < bHitPos.y + bambooSize.y &&
				pHitPos.y + playerSize.y > bHitPos.y;

			int anim = player->getCurrentAnimation();
			bool isBlockingUp = (anim == 12 || anim == 13);

			if (!bb.blocked && isBlockingUp && overlapX && overlapY) {
				b->onBlocked();
				bb.blocked = true; // marquem com a bloquejat
			}
			else if (playerInvulTime <= 0 && overlapX && overlapY && !bb.blocked) {
				playerHealth.takeDamage(0.33);
				playerInvulTime = 2000;
				player->animacioDamage();
			}

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

		if (playerInvulTime <= 0 && overlapX && overlapY && !godMode) {
			playerHealth.takeDamage(0.33);
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
			playerHealth.takeDamage(0.33);
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
			playerHealth.takeDamage(0.33);
			playerInvulTime = 2000;
			player->animacioDamage();
			break;
		}
	}


	
	for (int i = 0; i < leaves.size(); ++i) {
		Leaf* leaf = leaves[i];
		glm::ivec2 enemyPos = leaf->getPosition();
		glm::ivec2 enemySize = glm::ivec2(8, 8);
		glm::ivec2 enemyOffset = glm::ivec2(0, 0);
		glm::ivec2 eHitPos = enemyPos + enemyOffset;
		bool overlapX = pHitPos.x < eHitPos.x + enemySize.x &&
			pHitPos.x + playerSize.x > eHitPos.x;
		bool overlapY = pHitPos.y < eHitPos.y + enemySize.y &&
			pHitPos.y + playerSize.y > eHitPos.y;
		if (playerInvulTime <= 0 && overlapX && overlapY) {
			playerHealth.takeDamage(0.33);
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
		if (playerInvulTime <= 0 && overlapX && overlapY && !b->isBlocked()) {
			playerHealth.takeDamage(0.33);
			playerInvulTime = 2000;
			player->animacioDamage();
			break;
		}
	}



	// Comprovació de col·lisió de la llança amb els enemics verds
	if (player != nullptr) {
		lanza = player->getLanza();
	}

	if (lanza != nullptr) {

		glm::ivec2 lanzaPos = lanza->getPosition();



		glm::ivec2 playerSize = glm::ivec2(23, 32);
		glm::ivec2 playerOffset = glm::ivec2(2, 0);
		glm::ivec2 pHitPos = glm::ivec2(ppos) + playerOffset;




		glm::ivec2 lanzaSize = glm::ivec2(32, 6); // ajusta si cal
		glm::ivec2 lanzaOffset = glm::ivec2(0, 13);
		glm::ivec2 lHitPos = glm::ivec2(lanzaPos) + lanzaOffset;

		pair<glm::ivec2, glm::ivec2> pLanza = lanza->getHitbox();

		for (int i = 0; i < greenEnemies.size(); ) {
			GreenEnemy* enemy = greenEnemies[i];
			
			pair<glm::ivec2, glm::ivec2> pEnemic = enemy->getHitbox();

			/*
			glm::ivec2 enemyPos = enemy->getPosition();
			glm::ivec2 enemySize = glm::ivec2(16, 32);
			glm::ivec2 enemyOffset = glm::ivec2(8, 0);
			glm::ivec2 eHitPos = enemyPos + enemyOffset;


			
			bool overlapX = lHitPos.x < eHitPos.x + enemySize.x &&
				lHitPos.x + lanzaSize.x > eHitPos.x;
			
			bool overlapY = lHitPos.y < eHitPos.y + enemySize.y &&
				lHitPos.y + lanzaSize.y > eHitPos.y;


			std::cout << "Llança: pos(" << lHitPos.x << "," << lHitPos.y << ") mida("
				<< lanzaSize.x << "," << lanzaSize.y << ")\n";
			std::cout << "Enemic: pos(" << eHitPos.x << "," << eHitPos.y << ") mida("
				<< enemySize.x << "," << enemySize.y << ")\n";

			*/


			if (enemy->checkCollision(pLanza, pEnemic)) {
				std::cout << "colisiono amb enemigo" << endl;
				enemy->takeDamage(0.25f);
				if (enemy->isDead()) {
					// Marquem el spawn com inactiu
					for (auto& sp : greenSpawnPoints) {
						if (sp.enemy == enemy) {
							sp.enemy = nullptr;
							break;
						}
					}

					delete enemy;
					greenEnemies.erase(greenEnemies.begin() + i);
					continue; // no incrementem i
				}
			}

			++i;
		}

		for (int i = 0; i < snakeEnemies.size(); ) {
			SnakeEnemy* enemy = snakeEnemies[i];
			glm::ivec2 enemyPos = enemy->getPosition();
			glm::ivec2 enemySize = glm::ivec2(24, 16);
			glm::ivec2 enemyOffset = glm::ivec2(4, 0);
			glm::ivec2 eHitPos = enemyPos + enemyOffset;



			bool overlapX = lHitPos.x < eHitPos.x + enemySize.x &&
				lHitPos.x + lanzaSize.x > eHitPos.x;

			bool overlapY = lHitPos.y < eHitPos.y + enemySize.y &&
				lHitPos.y + lanzaSize.y > eHitPos.y;

			
			if (overlapX && overlapY) {
				enemy->takeDamage(0.25f);
				if (enemy->isDead()) {
					// Marquem el spawn com inactiu
					for (auto& sp : snakeSpawnPoints) {
						if (sp.enemy == enemy) {
							sp.enemy = nullptr;
							break;
						}
					}
					delete enemy;
					snakeEnemies.erase(snakeEnemies.begin() + i);
					continue; // no incrementem i
				}
			}
			++i;
		}

		for (int i = 0; i < beardEnemies.size(); ) {
			BeardEnemy* enemy = beardEnemies[i];
			glm::ivec2 enemyPos = enemy->getPosition();
			glm::ivec2 enemySize = glm::ivec2(32, 32);
			glm::ivec2 enemyOffset = glm::ivec2(0, 0);
			glm::ivec2 eHitPos = enemyPos + enemyOffset;
			bool overlapX = lHitPos.x < eHitPos.x + enemySize.x &&
				lHitPos.x + lanzaSize.x > eHitPos.x;
			bool overlapY = lHitPos.y < eHitPos.y + enemySize.y &&
				lHitPos.y + lanzaSize.y > eHitPos.y;
			if (overlapX && overlapY) {
				enemy->takeDamage(0.25f);
				if (enemy->isDead()) {
					// Marquem el spawn com inactiu
					for (auto& sp : beardSpawnPoints) {
						if (sp.enemy == enemy) {
							sp.enemy = nullptr;
							break;
						}
					}
					delete enemy;
					beardEnemies.erase(beardEnemies.begin() + i);
					continue; // no incrementem i
				}
			}
			++i;
		}

		/*
		//Daño al boss final
		glm::ivec2 enemyPos = boss->getPosition();
		glm::ivec2 enemySize = glm::ivec2(32, 32);
		glm::ivec2 enemyOffset = glm::ivec2(0, 0);
		glm::ivec2 eHitPos = enemyPos + enemyOffset;
		bool overlapX = lHitPos.x < eHitPos.x + enemySize.x &&
			lHitPos.x + lanzaSize.x > eHitPos.x;
		bool overlapY = lHitPos.y < eHitPos.y + enemySize.y &&
			lHitPos.y + lanzaSize.y > eHitPos.y;
		if (overlapX && overlapY) {
			bossHealth.takeDamage(0.25f);
			if (bossHealth.isDead()) {
				// Marquem el spawn com inactiu
				delete boss;
			}
		}
		*/

	}





	if (playerInvulTime > 0)
		playerInvulTime -= deltaTime;

}

void Scene::setBossVisible(bool b) {
	bossVisible = b;
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

	for (auto& powerup : powerUps)
		powerup->render();

	if (boss) {
		if (boss->isInitialized()) boss->render();
	}

	/*
	if (lanza != nullptr)
		lanza->render();
	*/

	for (auto& bb : bossBamboos)
		if (bb.bamboo != nullptr)
			bb.bamboo->render();

	// Renderitzar les fulles
	for (auto& leaf : leaves)
		leaf->render();

	// Reposicionar els cors segons la càmera
	
	for (int i = 0; i < playerHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 8, top + (8 + 24) + i * 12); // marges i separació vertical
		playerHealth.setHeartPosition(i, heartPos);
	}

	for (int i = 0; i < bossHealth.getMaxHearts(); ++i) {
		glm::vec2 heartPos(left + 32, top + (8 + 24) + i * 12); // marges i separació vertical
		bossHealth.setHeartPosition(i, heartPos);
	}
	

	// Mostrar els cors
	playerHealth.render();
	
	
	glm::vec2 posPlayer = player->getPosition();

	if (posPlayer.x + tileSize > 248 * tileSize) bossHealth.render();


	gui.render();

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