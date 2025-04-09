#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "GreenEnemy.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50.0f
#define FALL_STEP 4
#define ATTACK_DURATION 300

//const glm::ivec2 HITBOX_SIZE = glm::ivec2(15, 32);
//const glm::ivec2 HITBOX_OFFSET = glm::ivec2(8, 0); //offsetX = (32 - 15) / 2 = 8

const glm::ivec2 HITBOX_SIZE = glm::ivec2(15, 32);
const glm::ivec2 HITBOX_OFFSET = glm::ivec2(8, 0); // subimos 4px la hitbox



enum Anim {
	STAND_LEFT, STAND_RIGHT, JUMP_LEFT, JUMP_RIGHT
};
void GreenEnemy::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
	facingRight = true;
	waitTimer = 0;
	bJumping = false;

	spritesheet.loadFromFile("images/Enemigo_verde.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);


	facingRight = true;
	facingRightBefore = true;
	waitTimer = 0;
	pendingDirectionChange = false;
	directionChanged = false;

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void GreenEnemy::update(int deltaTime) {
	sprite->update(deltaTime);

	// Si está en el aire (saltando)
	if (bJumping) {

		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false;
			posEnemy.y = startY;
			waitTimer = 500; // Esperar 0.5 segundos antes del siguiente salto
		}
		else {
			posEnemy.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (map->collisionMoveUp(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y) && jumpAngle < 90)
				jumpAngle = 180 - jumpAngle;

			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {
					bJumping = false;
					waitTimer = 500;
					startY = posEnemy.y;
				}
			}
		}

		// Movimiento horizontal automático
		int dx = facingRight ? 2 : -2;
		posEnemy.x += dx;

		if ((facingRight && map->collisionMoveRight(posEnemy + HITBOX_OFFSET, HITBOX_SIZE)) ||
			(!facingRight && map->collisionMoveLeft(posEnemy + HITBOX_OFFSET, HITBOX_SIZE))) {
			
			posEnemy.x -= dx;
			facingRight = !facingRight; // Cambia de dirección si choca

		}
		

		// Animación de salto
		if (sprite->animation() != (facingRight ? JUMP_RIGHT : JUMP_LEFT))
			sprite->changeAnimation(facingRight ? JUMP_RIGHT : JUMP_LEFT);


	}

	// Si está en el suelo
	else {
		posEnemy.y += FALL_STEP;

		if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {

			// Animació de espera segons la direcció anterior
			if (sprite->animation() != (facingRight ? STAND_RIGHT : STAND_LEFT))
				sprite->changeAnimation(facingRight ? STAND_RIGHT : STAND_LEFT);

			// Direcció cap al jugador (preparant el següent salt)
			facingRight = (posPlayer.x > posEnemy.x);

			// Espera abans de saltar
			if (waitTimer > 0) {
				waitTimer -= deltaTime;
			}
			else {
				bJumping = true;
				jumpAngle = 0;
				startY = posEnemy.y;
			}
		}
	}



	// Actualiza la posición visual
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void GreenEnemy::render()
{
	sprite->render();
}

void GreenEnemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void GreenEnemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void GreenEnemy::setPlayerPosition(const glm::ivec2& pos) {
	posPlayer = pos;
}

void GreenEnemy::takeDamage(float amount) {
	health -= amount;
	if (health <= 0.f) {
		dead = true;
	}
}

bool GreenEnemy::isDead() const {
	return dead;
}

bool GreenEnemy::checkCollision(const std::pair<glm::ivec2, glm::ivec2>& a, const std::pair<glm::ivec2, glm::ivec2>& b) {
	glm::ivec2 l1 = a.second;
	glm::ivec2 r1 = l1 + a.first;
	glm::ivec2 l2 = b.second;
	glm::ivec2 r2 = l2 + b.first;

	if (r1.x <= l2.x || r2.x <= l1.x) return false;
	if (r1.y <= l2.y || r2.y <= l1.y) return false;
	return true;
}

pair<glm::ivec2, glm::ivec2> GreenEnemy::getHitbox() {
	glm::ivec2 hitbox;
	glm::ivec2 offset;

	hitbox = glm::ivec2(16, 32);
	offset = glm::ivec2(9, 0);

	return { hitbox, posEnemy + offset };
}


