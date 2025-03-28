#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50.0f
#define FALL_STEP 4
#define ATTACK_DURATION 300

enum PlayerAnims
{
    STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, BEND_RIGHT, BEND_LEFT,
    JUMP_LEFT, JUMP_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, CROUCH_ATTACK_LEFT,
    CROUCH_ATTACK_RIGHT, UP_BLOCK_LEFT, UP_BLOCK_RIGHT, UP_ATTACK_LEFT,
    UP_ATTACK_RIGHT, DOWN_ATTACK_LEFT, DOWN_ATTACK_RIGHT, STAND_ATTACK_RIGHT,
    STAND_ATTACK_LEFT

};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    bAttacking = false;
    bCrouching = false;
    spritesheet.loadFromFile("images/PersonajeVJ.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(18);

    sprite->setAnimationSpeed(STAND_LEFT, 12);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(STAND_RIGHT, 12);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(MOVE_LEFT, 12);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.875f, 0.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 12);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.f));

    sprite->setAnimationSpeed(BEND_LEFT, 12);
    sprite->addKeyframe(BEND_LEFT, glm::vec2(0.5f, 0.5f));

    sprite->setAnimationSpeed(BEND_RIGHT, 12);
    sprite->addKeyframe(BEND_RIGHT, glm::vec2(0.25f, 0.5f));

    sprite->setAnimationSpeed(UP_BLOCK_LEFT, 12);
    sprite->addKeyframe(UP_BLOCK_LEFT, glm::vec2(0.125f, 0.5f));
    sprite->setAnimationSpeed(UP_BLOCK_RIGHT, 12);
    sprite->addKeyframe(UP_BLOCK_RIGHT, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(UP_ATTACK_LEFT, 12);
    sprite->addKeyframe(UP_ATTACK_LEFT, glm::vec2(0.125f, 0.75f));
    sprite->setAnimationSpeed(UP_ATTACK_RIGHT, 12);
    sprite->addKeyframe(UP_ATTACK_RIGHT, glm::vec2(0.f, 0.75f));

    sprite->setAnimationSpeed(DOWN_ATTACK_LEFT, 12);
    sprite->addKeyframe(DOWN_ATTACK_LEFT, glm::vec2(0.375f, 0.75f));
    sprite->setAnimationSpeed(DOWN_ATTACK_RIGHT, 12);
    sprite->addKeyframe(DOWN_ATTACK_RIGHT, glm::vec2(0.25f, 0.75f));

    sprite->setAnimationSpeed(CROUCH_ATTACK_LEFT, 12);
    sprite->addKeyframe(CROUCH_ATTACK_LEFT, glm::vec2(0.625f, 0.5f));
    sprite->setAnimationSpeed(CROUCH_ATTACK_RIGHT, 12);
    sprite->addKeyframe(CROUCH_ATTACK_RIGHT, glm::vec2(0.375f, 0.5f));

    sprite->setAnimationSpeed(STAND_ATTACK_LEFT, 12);
    sprite->addKeyframe(STAND_ATTACK_LEFT, glm::vec2(0.5f, 0.25f));
    sprite->setAnimationSpeed(STAND_ATTACK_RIGHT, 12);
    sprite->addKeyframe(STAND_ATTACK_RIGHT, glm::vec2(0.f, 0.25f));


    sprite->setAnimationSpeed(ATTACK_LEFT, 12);
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.625f, 0.25f));
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.75f, 0.25f));
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.875f, 0.25f));

    sprite->setAnimationSpeed(ATTACK_RIGHT, 12);
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.125f, 0.25f));
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.25f, 0.25f));
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.375f, 0.25f));

    sprite->changeAnimation(STAND_RIGHT);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
    if (bAttacking && sprite->isLastKeyframe())
    {
        bAttacking = false;
    }

    bool isCrouching = Game::instance().getKey(GLFW_KEY_DOWN);
    bool isJumping = bJumping;
    bool isAttacking = Game::instance().getKey(GLFW_KEY_X);
    bool isBlocking = Game::instance().getKey(GLFW_KEY_UP);
    bool isMovingLeft = Game::instance().getKey(GLFW_KEY_LEFT);
    bool isMovingRight = Game::instance().getKey(GLFW_KEY_RIGHT);


    /*
    if (isAttacking) {  // Si la tecla X est� presionada, atacar
        if (isMovingLeft) {
            sprite->changeAnimation(ATTACK_LEFT);
        }
        else if (isMovingRight) {
            sprite->changeAnimation(ATTACK_RIGHT);
        }
        else {
            if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
                sprite->changeAnimation(STAND_ATTACK_LEFT);
            }
            else {
                sprite->changeAnimation(STAND_ATTACK_RIGHT);
            }
        }
        bAttacking = true;  // Indicar que est� atacando
    }

    else if (bAttacking) {  // Si ya no se presiona X y estaba atacando
        bAttacking = false;
        if (sprite->animation() == ATTACK_LEFT || sprite->animation() == STAND_ATTACK_LEFT) {
            sprite->changeAnimation(STAND_LEFT);
        }
        else if (sprite->animation() == ATTACK_RIGHT || sprite->animation() == STAND_ATTACK_RIGHT) {
            sprite->changeAnimation(STAND_RIGHT);
        }
    }
    */

    if (!bAttacking)
    {
        if (Game::instance().getKey(GLFW_KEY_DOWN))  // Si est� presionando la flecha abajo
        {
            if (Game::instance().getKey(GLFW_KEY_X)) {
                if (sprite->animation() == BEND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
                    sprite->changeAnimation(CROUCH_ATTACK_LEFT);
                }
                else if (sprite->animation() == BEND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
                    sprite->changeAnimation(CROUCH_ATTACK_RIGHT);
                }
                bAttacking = true;
            }
            else {
                if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                    sprite->changeAnimation(BEND_LEFT);
                }
                else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                    sprite->changeAnimation(BEND_RIGHT);
                }
                else {
                    if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
                        sprite->changeAnimation(BEND_RIGHT);
                    }
                    else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
                        sprite->changeAnimation(BEND_LEFT);
                    }
                }
            }
        }
        else if (Game::instance().getKey(GLFW_KEY_UP))  // Si está presionando la flecha abajo
        {
            if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                sprite->changeAnimation(UP_BLOCK_LEFT);
            }
            else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                sprite->changeAnimation(UP_BLOCK_RIGHT);
            }
            else {
                if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
                    sprite->changeAnimation(UP_BLOCK_RIGHT);
                }
                else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
                    sprite->changeAnimation(UP_BLOCK_LEFT);
                }
            }
        }
        else  // Si NO est� presionando abajo, puede moverse
        {
            if (sprite->animation() == BEND_LEFT) {
                sprite->changeAnimation(STAND_LEFT);
            }
            else if (sprite->animation() == BEND_RIGHT) {
                sprite->changeAnimation(STAND_RIGHT);
            }


            else if (sprite->animation() == UP_BLOCK_LEFT) {
                sprite->changeAnimation(STAND_LEFT);
            }

            else if (sprite->animation() == UP_BLOCK_RIGHT) {
                sprite->changeAnimation(STAND_RIGHT);
            }

            if (Game::instance().getKey(GLFW_KEY_LEFT))
            {
                if (sprite->animation() != MOVE_LEFT)
                    sprite->changeAnimation(MOVE_LEFT);
                posPlayer.x -= 2;
                if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
                {
                    posPlayer.x += 2;
                    sprite->changeAnimation(STAND_LEFT);
                }
            }
            else if (Game::instance().getKey(GLFW_KEY_RIGHT))
            {
                if (sprite->animation() != MOVE_RIGHT)
                    sprite->changeAnimation(MOVE_RIGHT);
                posPlayer.x += 2;
                if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
                {
                    posPlayer.x -= 2;
                    sprite->changeAnimation(STAND_RIGHT);
                }
            }
            else  // Si no se mueve, cambiar a animaci�n de estar quieto
            {
                if (sprite->animation() == MOVE_LEFT)
                    sprite->changeAnimation(STAND_LEFT);
                else if (sprite->animation() == MOVE_RIGHT)
                    sprite->changeAnimation(STAND_RIGHT);
            }
        }
    }

    if (bJumping)
    {
        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle >= 180)
        {
            bJumping = false;
            posPlayer.y = startY;

            if (sprite->animation() == BEND_LEFT) {
                sprite->changeAnimation(STAND_LEFT);
            }
            else if (sprite->animation() == BEND_RIGHT) {
                sprite->changeAnimation(STAND_RIGHT);
            }
        }
        else
        {
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y) && jumpAngle < 90)
			{
				jumpAngle = 180 - jumpAngle;
			}

            if (jumpAngle > 90)
                bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
        }
        if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
            sprite->changeAnimation(BEND_LEFT);
        }
        else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
            sprite->changeAnimation(BEND_RIGHT);
        }
    }
    else
    {
        posPlayer.y += FALL_STEP;
        if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
        {
            if (Game::instance().getKey(GLFW_KEY_Z))
            {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;

                if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT) {
                    sprite->changeAnimation(BEND_LEFT);
                }
                else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) {
                    sprite->changeAnimation(BEND_RIGHT);
                }
            }
        }
    }

    /*
    if (isAttacking) {
        if (isJumping) {
            sprite->changeAnimation((sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) ? DOWN_ATTACK_LEFT : DOWN_ATTACK_RIGHT);
        }
        else if (isBlocking) {
            sprite->changeAnimation((sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) ? UP_ATTACK_LEFT : UP_ATTACK_RIGHT);
        }
        else {
            sprite->changeAnimation((sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) ? ATTACK_LEFT : ATTACK_RIGHT);
        }
        bAttacking = true;
    }

    if (isBlocking && !isAttacking) {
        sprite->changeAnimation((sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) ? UP_BLOCK_LEFT : UP_BLOCK_RIGHT);
    }
    */
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
    sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
