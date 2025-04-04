#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "BeardEnemy.h"
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
	MOVE_LEFT, MOVE_RIGHT, ATACK_LEFT, ATACK_RIGHT
};
void BeardEnemy::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
	facingRight = true;
	waitTimer = 0;
	bJumping = false;

	spritesheet.loadFromFile("images/PandaEnemy_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.5f));

	sprite->setAnimationSpeed(ATACK_LEFT, 8);
	sprite->addKeyframe(ATACK_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(ATACK_LEFT, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(ATACK_LEFT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(ATACK_RIGHT, 8);
	sprite->addKeyframe(ATACK_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(ATACK_RIGHT, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(ATACK_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(0);


	facingRight = true;
	facingRightBefore = true;
	waitTimer = 0;
	pendingDirectionChange = false;
	directionChanged = false;

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void BeardEnemy::update(int deltaTime) {
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
		if (sprite->animation() != (facingRight ? ATACK_RIGHT : ATACK_LEFT))
			sprite->changeAnimation(facingRight ? ATACK_RIGHT : ATACK_LEFT);


	}

	// Si está en el suelo
	else {
		posEnemy.y += FALL_STEP;

		if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {

			// Animació de espera segons la direcció anterior
			if (sprite->animation() != (facingRight ? MOVE_RIGHT : MOVE_LEFT))
				sprite->changeAnimation(facingRight ? MOVE_RIGHT : MOVE_LEFT);

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

void BeardEnemy::render()
{
	sprite->render();
}

void BeardEnemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void BeardEnemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void BeardEnemy::setPlayerPosition(const glm::ivec2& pos) {
	posPlayer = pos;
}

