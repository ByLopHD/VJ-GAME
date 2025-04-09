#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Boss.h"
#include "Player.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#define OSCILLATION_AMPLITUDE 4

const glm::ivec2 HITBOX_SIZE = glm::ivec2(32, 48);
const glm::ivec2 HITBOX_OFFSET = glm::ivec2(0, 8); // subimos 4px la hitbox



enum Anim {
	FLOATING, STOPPED, POS_INIT, DESCENDING
};
void Boss::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/Boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(FLOATING, 8);
	sprite->addKeyframe(FLOATING, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STOPPED, 8);
	sprite->addKeyframe(STOPPED, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(POS_INIT, 8);
	sprite->addKeyframe(POS_INIT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DESCENDING, 8);
	sprite->addKeyframe(DESCENDING, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(POS_INIT);

	stateTimer = 0.f;
	movingRight = false;  // comença anant a l'esquerra
	bambooShootTimer = 0.f;
	floatDirection = 1.f; // puja primer

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Boss::update(int deltaTime) {
	sprite->update(deltaTime);
	stateTimer += deltaTime;
	bambooShootTimer += deltaTime;

	if (sprite->animation() == POS_INIT) {
		if (stateTimer >= 1000) { // espera 1s abans de començar
			sprite->changeAnimation(FLOATING);
			stateTimer = 0.f;
		}
	}
	else if (sprite->animation() == FLOATING) {
		// Fase 1: pujar fins a posY 33*tileSize (topY)
		const float topY = 31 * 16;
		if (posEnemy.y > topY) {
			posEnemy.y -= 1.0f * (deltaTime / 16.f);
			if (posEnemy.y < topY) posEnemy.y = topY;
		}
		else {			

			// Oscil·lació en Y dins de [31 * tileSize, 35 * tileSize]
			const float minOscY = 29 * 16;
			const float maxOscY = 31 * 16;

			// Oscil·lació vertical suau
			
			


			// Desplaçament lateral
			const float leftLimit = 240 * 16;
			const float rightLimit = (256 * 16);

			if (movingRight)
				posEnemy.x += 1.f;
			else
				posEnemy.x -= 1.f;

			if (posEnemy.x <= leftLimit) movingRight = true;
			if (posEnemy.x + 32 >= rightLimit) movingRight = false;

			// Disparar bambú cap avall
			if (bambooShootTimer >= 250) { // cada 0.25 segons = 250 ms
				if (scene != nullptr) {
					scene->spawnBossBamboo(glm::vec2(posEnemy.x + 16, posEnemy.y + 24));
				}
				bambooShootTimer = 0; // reinicia el temporitzador
			}
			
		}
		

		// Després de 7 segons → propera fase
		if (stateTimer >= 5000) {
			sprite->changeAnimation(DESCENDING);
			stateTimer = 0.f;
			std::cout << "→ PASSA A FASE D'ATAC FULLA" << std::endl;
		}		
	}
	else if (sprite->animation() == DESCENDING) {
		// Fase 1: pujar fins a posY 33*tileSize (topY)
		const float topY = 36 * 16;
		if (posEnemy.y < topY) {
			posEnemy.y += 1.0f * (deltaTime / 16.f);
			if (posEnemy.y > topY) posEnemy.y = topY;
		}
		else {
			sprite->changeAnimation(STOPPED);
			std::cout << "→ Boss ha aterrat i està en STOPPED\n";
			stateTimer = 0.f;
		}
	}
	else if (sprite->animation() == STOPPED) {
		if (!leafBurstDone && stateTimer > 500) {
			if (!leavesActive) {
				leavesActive = true;
				visible = false;
				scene->setBossVisible(visible);

				leafPhaseTimer = 0.f;

				const glm::vec2 center = glm::vec2(posEnemy.x + 16, posEnemy.y + 16); // centre del boss
				for (int i = 0; i < 8; ++i) {
					float angle = float(i) * 45.f;  // 360 / 8
					scene->spawnLeaf(center, angle);
				}

				// Llencem bambús durant la fase de STOPPED
				for (int i = 0; i < 8; ++i) {
					int offset = (i * 32) + 4; // centrat
					scene->spawnBossBamboo(glm::ivec2(242 * 16 + offset, 29 * 16));
				}

				std::cout << "🌿 Inici fase fulles\n";
			}

			leafPhaseTimer += deltaTime;
			if (leafPhaseTimer >= 3000) {
				visible = true;
				scene->setBossVisible(visible);

			

				leavesActive = false;
				sprite->changeAnimation(FLOATING);
				stateTimer = 0.f;
				std::cout << "🌿 Fi fase fulles. Torna el boss\n";
			}
		}

	}



	// Update visuals
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Boss::render()
{
	sprite->render();
}

void Boss::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Boss::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Boss::setPlayerPosition(const glm::ivec2& pos) {
	posPlayer = pos;
}

void Boss::setSceneReference(Scene* s) {
	scene = s;
}


