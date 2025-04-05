#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "BeardEnemy.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50.0f
#define FALL_STEP 2
#define ATTACK_DURATION 300

//const glm::ivec2 HITBOX_SIZE = glm::ivec2(15, 32);
//const glm::ivec2 HITBOX_OFFSET = glm::ivec2(8, 0); //offsetX = (32 - 15) / 2 = 8

const glm::ivec2 HITBOX_SIZE = glm::ivec2(32, 32);
const glm::ivec2 HITBOX_OFFSET = glm::ivec2(0, 0); // subimos 4px la hitbox



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

	// Calcular distància al jugador
	int dx = posPlayer.x - posEnemy.x;
	int dy = posPlayer.y - posEnemy.y;
	int absDx = std::abs(dx);
	int absDy = std::abs(dy);

	// Només canviem direcció si el jugador està realment separat
	if (absDx > 10)
		facingRight = (dx > 0);

	// Si està molt a prop en X i Y → atac
	bool closeInX = (absDx < 20);
	bool closeInY = (absDy < 20);

	if (!bJumping && closeInX && closeInY) {
		Anim attackAnim = facingRight ? ATACK_RIGHT : ATACK_LEFT;
		if (sprite->animation() != attackAnim)
			sprite->changeAnimation(attackAnim);
		return;
	}


	// Si està a l’aire → procés de salt
	if (bJumping) {
		jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle >= 180) {
			bJumping = false;
			posEnemy.y = startY;
			//waitTimer = 500;
		}
		else {
			posEnemy.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (map->collisionMoveUp(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y) && jumpAngle < 90)
				jumpAngle = 180 - jumpAngle;

			if (jumpAngle > 90) {
				if (map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y)) {
					bJumping = false;
					//waitTimer = 500;
					startY = posEnemy.y;
				}
			}
		}

		// Desplaçament lateral mentre salta
		int moveX = facingRight ? 2 : -2;
		posEnemy.x += moveX;
		if ((facingRight && map->collisionMoveRight(posEnemy + HITBOX_OFFSET, HITBOX_SIZE)) ||
			(!facingRight && map->collisionMoveLeft(posEnemy + HITBOX_OFFSET, HITBOX_SIZE))) {
			posEnemy.x -= moveX;
			facingRight = !facingRight;
		}

		if (sprite->animation() != (facingRight ? MOVE_RIGHT : MOVE_LEFT))
			sprite->changeAnimation(facingRight ? MOVE_RIGHT : MOVE_LEFT);
	}
	else {
		// Gravetat
		posEnemy.y += FALL_STEP;
		bool onGround = map->collisionMoveDown(posEnemy + HITBOX_OFFSET, HITBOX_SIZE, &posEnemy.y);

		if (onGround) {
			// Només salta si hi ha una diferència vertical real
			bool verticalDistance = (posEnemy.y - posPlayer.y > 20);
			bool horizontalDistance = absDx < 100;
			bool isSameLevel = abs(posEnemy.y - posPlayer.y) < 10;

			bool shouldJump = verticalDistance && horizontalDistance && !isSameLevel;

			if (shouldJump) {
				bJumping = true;
				jumpAngle = 0;
				startY = posEnemy.y;
			}
			else {
				// Caminem cap al jugador
				int moveX = facingRight ? 2: -2;
				posEnemy.x += moveX;

				if ((facingRight && map->collisionMoveRight(posEnemy + HITBOX_OFFSET, HITBOX_SIZE)) ||
					(!facingRight && map->collisionMoveLeft(posEnemy + HITBOX_OFFSET, HITBOX_SIZE))) {
					posEnemy.x -= moveX;
					facingRight = !facingRight;
				}

				if (sprite->animation() != (facingRight ? MOVE_RIGHT : MOVE_LEFT))
					sprite->changeAnimation(facingRight ? MOVE_RIGHT : MOVE_LEFT);
			}
		}
	}

	// Posició final del sprite
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

