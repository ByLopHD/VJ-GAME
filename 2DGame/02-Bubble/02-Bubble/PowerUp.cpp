#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "PowerUp.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>


#define FALL_STEP 2					//Cor gran

#define RISE_STEP 0.5					//Cor petit
#define OSCILLATION_AMPLITUDE 2		//Cor petit

const glm::ivec2 HITBOX_SIZE_HEART_BIG = glm::ivec2(16, 16);
const glm::ivec2 HITBOX_OFFSET_HEART_BIG = glm::ivec2(0, 0); // subimos 4px la hitbox

enum PowerUpType {
    HEART_SMALL,
    HEART_BIG,
    PUMPKIN,
    DEER_SHIRT,
    FLINT_TIP
};

void PowerUp::init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram, int type) {
	spritesheet.loadFromFile("images/powerup.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(HEART_SMALL, 8);
	sprite->addKeyframe(HEART_SMALL, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(HEART_BIG, 8);
	sprite->addKeyframe(HEART_BIG, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(PUMPKIN, 8);
	sprite->addKeyframe(PUMPKIN, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(DEER_SHIRT, 8);
	sprite->addKeyframe(DEER_SHIRT, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(FLINT_TIP, 8);
	sprite->addKeyframe(FLINT_TIP, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(type);

	tileMapDispl = screenPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerUp.x), float(tileMapDispl.y + posPowerUp.y)));
	active = true;
}

void PowerUp::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void PowerUp::setPosition(const glm::vec2& pos) {
	posPowerUp = glm::ivec2(pos);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerUp.x), float(tileMapDispl.y + posPowerUp.y)));
}

void PowerUp::setType(int type) {
	// Assigna animació segons tipus
	sprite->changeAnimation(type);
}

int PowerUp::getType() const {
	if (sprite->animation() == HEART_SMALL) return 0;
	if (sprite->animation() == HEART_BIG) return 1;
	if (sprite->animation() == PUMPKIN) return 2;
	if (sprite->animation() == DEER_SHIRT) return 3;
	if (sprite->animation() == FLINT_TIP) return 4;
}

bool PowerUp::isActive() const {
	return active;
}

void PowerUp::setActive(bool value) {
	active = value;
}

void PowerUp::update(int deltaTime) {
	if (sprite->animation() == HEART_BIG || sprite->animation() == PUMPKIN || sprite->animation() == DEER_SHIRT) {
		if (!active) return;

		sprite->update(deltaTime);

		if (!onGround) {
			posPowerUp.y += FALL_STEP;

			if (map && map->collisionMoveDown(posPowerUp + HITBOX_OFFSET_HEART_BIG, HITBOX_SIZE_HEART_BIG, &posPowerUp.y)) {
				onGround = true;
				posPowerUp.y -= FALL_STEP; // Evita tremolor
			}
		}
	}

	if (sprite->animation() == HEART_SMALL) {
		static int anchorX = 0;
		static bool anchorSet = false;
		static bool movingRight = true;
		if (!active) {
			// Si se ha desactivado (p.ej. recogido), reiniciamos estado y salimos
			anchorSet = false;
			return;
		}
		sprite->update(deltaTime);
		if (!anchorSet) {
			// Fijar posición inicial de oscilación al aparecer
			anchorX = posPowerUp.x;
			anchorSet = true;
			movingRight = true;  // Comienza moviéndose hacia la derecha
		}
		// Movimiento ascendente
		posPowerUp.y -= RISE_STEP;
		// Movimiento horizontal oscilatorio
		if (movingRight) {
			posPowerUp.x += 1;
			if (posPowerUp.x >= anchorX + OSCILLATION_AMPLITUDE)
				movingRight = false;  // Alcanzó extremo derecho, invertir dirección
		}
		else {
			posPowerUp.x -= 1;
			if (posPowerUp.x <= anchorX - OSCILLATION_AMPLITUDE)
				movingRight = true;   // Alcanzó extremo izquierdo, invertir dirección
		}
		// Si surt per dalt de la pantalla, eliminar-lo
	}

	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerUp.x), float(tileMapDispl.y + posPowerUp.y)));
}

void PowerUp::render() {
	if (active) sprite->render();
}

bool PowerUp::checkCollision(const glm::ivec2& playerPos, const glm::ivec2& playerSize) {
	glm::ivec2 size = glm::ivec2(16, 16);
	return playerPos.x < posPowerUp.x + size.x &&
		playerPos.x + playerSize.x > posPowerUp.x &&
		playerPos.y < posPowerUp.y + size.y &&
		playerPos.y + playerSize.y > posPowerUp.y;
}