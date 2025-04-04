#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "SnakeEnemy.h"
#include "Player.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 10.0f
#define FALL_STEP 4
#define ATTACK_DURATION 300

const float MOVE_SPEED = 60.0f; // píxels per segon

const glm::ivec2 HITBOX_SIZE = glm::ivec2(24, 16);
const glm::ivec2 HITBOX_OFFSET = glm::ivec2(4, 0); // subimos 4px la hitbox


enum Anim {
	MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT
};
void SnakeEnemy::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
	facingRight = true;
	waitTimer = 0;
	bJumping = false;


	spritesheet.loadFromFile("images/SnakeEnemy2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(0);

	facingRight = true;
	facingRightBefore = true;
	waitTimer = 0;
	pendingDirectionChange = false;
	directionChanged = false;

	tileMapDispl = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void SnakeEnemy::update(int deltaTime) {
	sprite->update(deltaTime);

	// FASE 1 — Salt suau d'aparició
	if (bJumping) {

		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180) {
			bJumping = false;
			posEnemy.y = startY;
			waitTimer = 0; // Esperar 0.5 segundos antes del siguiente salto
		}
		else {
			posEnemy.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (map->collisionMoveUp(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y) && jumpAngle < 90)
				jumpAngle = 180 - jumpAngle;

			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {
					bJumping = false;
					waitTimer = 0;
					startY = posEnemy.y;
				}
			}
		}

		// Movimiento horizontal automático
		/*
		int dx = facingRight ? 1 : -1;
		posEnemy.x += dx;

		if ((facingRight && map->collisionMoveRight(posEnemy + HITBOX_OFFSET, HITBOX_SIZE)) ||
			(!facingRight && map->collisionMoveLeft(posEnemy + HITBOX_OFFSET, HITBOX_SIZE))) {
			posEnemy.x -= dx;
			facingRight = !facingRight; // Cambia de dirección si choca

		}
		*/
		if (jumpAngle < 150) {
			float dx = (facingRight ? 0.75f : -0.75f);
			posEnemy.x += int(dx);
		}
		

		// Animación de salto
		if (sprite->animation() != (facingRight ? JUMP_RIGHT : JUMP_LEFT))
			sprite->changeAnimation(facingRight ? JUMP_RIGHT : JUMP_LEFT);


	}

	// FASE 2 — Moviment lateral constant (un cop ha caigut al terra)
	else {
		posEnemy.y += FALL_STEP;

		if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {
			// Moviment lateral automàtic
			int dx = facingRight ? 2.5f : -2.5f;
			posEnemy.x += dx;

			// Límits de la càmera
			int camLeft = scene->getCameraLeft() - (2.5*16);
			int camRight = scene->getCameraRight() - (2.5*16);

			if (posEnemy.x <= camLeft) {
				posEnemy.x = camLeft;
				facingRight = true;
			}
			else if (posEnemy.x + HITBOX_SIZE.x >= camRight) {
				posEnemy.x = camRight - HITBOX_SIZE.x;
				facingRight = false;
			}

			// Rebotar si toca una paret
			if ((facingRight && map->collisionMoveRight(posEnemy + HITBOX_OFFSET, HITBOX_SIZE)) ||
				(!facingRight && map->collisionMoveLeft(posEnemy + HITBOX_OFFSET, HITBOX_SIZE))) {
				posEnemy.x -= dx;
				facingRight = !facingRight;
			}

			// Animació de moviment lateral
			if (sprite->animation() != (facingRight ? MOVE_RIGHT : MOVE_LEFT))
				sprite->changeAnimation(facingRight ? MOVE_RIGHT : MOVE_LEFT);

			if (primer) {
				bJumping = true;
				jumpAngle = 0;
				startY = posEnemy.y;
				primer = false;
			}

		}
	}

	// Posició visual
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void SnakeEnemy::render()
{
	sprite->render();
}

void SnakeEnemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void SnakeEnemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void SnakeEnemy::setPlayerPosition(const glm::ivec2& pos) {
	posPlayer = pos;
}

void SnakeEnemy::setSceneReference(Scene* s) {
	scene = s;
}

