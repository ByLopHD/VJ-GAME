#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50.0f
#define FALL_STEP 4
#define ATTACK_DURATION 250

const glm::ivec2 HITBOX_SIZE = glm::ivec2(23, 32);
const glm::ivec2 HITBOX_OFFSET = glm::ivec2(6, 0);

static bool spinningActive = false;
static bool enterPressedPrev = false;

enum PlayerAnims
{
    STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, BEND_RIGHT, BEND_LEFT,
    JUMP_LEFT, JUMP_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, CROUCH_ATTACK_LEFT,
    CROUCH_ATTACK_RIGHT, UP_BLOCK_LEFT, UP_BLOCK_RIGHT, UP_ATTACK_LEFT,
    UP_ATTACK_RIGHT, DOWN_ATTACK_LEFT, DOWN_ATTACK_RIGHT, STAND_ATTACK_RIGHT,
    STAND_ATTACK_LEFT, DAMAGED_LEFT, DAMAGED_RIGHT

};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    bAttacking = false;
    bCrouching = false;
    this->shaderProgram = &shaderProgram;
    lanza = nullptr;
    currentLanzaAnim = -1;
    spritesheet.loadFromFile("images/PersonajeVJ3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(22);

    sprite->setAnimationSpeed(STAND_LEFT, 12);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.4f, 0.f));

    sprite->setAnimationSpeed(STAND_RIGHT, 12);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(MOVE_LEFT, 12);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.7f, 0.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 12);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3, 0.f));

    sprite->setAnimationSpeed(BEND_LEFT, 12);
    sprite->addKeyframe(BEND_LEFT, glm::vec2(0.4f, 0.4f));

    sprite->setAnimationSpeed(BEND_RIGHT, 12);
    sprite->addKeyframe(BEND_RIGHT, glm::vec2(0.2f, 0.4f));

    sprite->setAnimationSpeed(UP_BLOCK_LEFT, 12);
    sprite->addKeyframe(UP_BLOCK_LEFT, glm::vec2(0.1f, 0.4f));
    sprite->setAnimationSpeed(UP_BLOCK_RIGHT, 12);
    sprite->addKeyframe(UP_BLOCK_RIGHT, glm::vec2(0.f, 0.4f));

    sprite->setAnimationSpeed(UP_ATTACK_LEFT, 12);
    sprite->addKeyframe(UP_ATTACK_LEFT, glm::vec2(0.1f, 0.8f));
    sprite->setAnimationSpeed(UP_ATTACK_RIGHT, 12);
    sprite->addKeyframe(UP_ATTACK_RIGHT, glm::vec2(0.f, 0.8f));

    sprite->setAnimationSpeed(DOWN_ATTACK_LEFT, 12);
    sprite->addKeyframe(DOWN_ATTACK_LEFT, glm::vec2(0.3f, 0.6f));
    sprite->setAnimationSpeed(DOWN_ATTACK_RIGHT, 12);
    sprite->addKeyframe(DOWN_ATTACK_RIGHT, glm::vec2(0.2f, 0.6f));

    sprite->setAnimationSpeed(CROUCH_ATTACK_LEFT, 12);
    sprite->addKeyframe(CROUCH_ATTACK_LEFT, glm::vec2(0.5f, 0.4f));
    sprite->setAnimationSpeed(CROUCH_ATTACK_RIGHT, 12);
    sprite->addKeyframe(CROUCH_ATTACK_RIGHT, glm::vec2(0.3f, 0.4f));

    sprite->setAnimationSpeed(STAND_ATTACK_LEFT, 12);
    sprite->addKeyframe(STAND_ATTACK_LEFT, glm::vec2(0.4f, 0.2f));
    sprite->setAnimationSpeed(STAND_ATTACK_RIGHT, 12);
    sprite->addKeyframe(STAND_ATTACK_RIGHT, glm::vec2(0.f, 0.2f));


    sprite->setAnimationSpeed(ATTACK_LEFT, 12);
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.5f, 0.2f));
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.6f, 0.2f));
    sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.7f, 0.2f));

    sprite->setAnimationSpeed(ATTACK_RIGHT, 12);
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.2f));
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.2f));
    sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.2f));

    sprite->setAnimationSpeed(DAMAGED_LEFT, 12);
    sprite->addKeyframe(DAMAGED_LEFT, glm::vec2(0.6f, 0.4f));

    sprite->setAnimationSpeed(DAMAGED_RIGHT, 12);
    sprite->addKeyframe(DAMAGED_RIGHT, glm::vec2(0.7f, 0.4f));

    sprite->changeAnimation(STAND_RIGHT);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2& Player::getPositionRef() {
    return posPlayer;
}

void Player::setOnMovingLog(bool onLog)
{
    bOnMovingLog = onLog;
}

void Player::moveWithPlatform(const glm::ivec2& movement) {
    posPlayer += movement;
    sprite->setPosition(glm::vec2(posPlayer.x, posPlayer.y));
}

void Player::update(int deltaTime)
{
    bool isAttacking = Game::instance().getKey(GLFW_KEY_X);
    bool isOnFloor = !map->collisionMoveDown(posPlayer + HITBOX_OFFSET, HITBOX_SIZE, &posPlayer.y);
    bool spawnLanza = false;
    int TypeLanza = NORMAL_ATTACK_RIGHT;
    bool wasKeyPressed = false; // Variable per detectar quan es prem la tecla per primera vegada

    if (Game::instance().getKey(GLFW_KEY_ENTER)) {
        if (!enterPressedPrev) {               // Solo al pulsar Enter por primera vez
            spinningActive = !spinningActive;  // Alternar entre modo lanza normal y lanza giratoria
            if (lanza != nullptr) {
                // Si cambiamos de arma mientras hay una lanza activa, eliminarla
                lanza->deactivate();
                delete lanza;
                lanza = nullptr;
                attackTimer = 0;
                bAttacking = false;
            }
        }
        enterPressedPrev = true;
    }
    else {
        enterPressedPrev = false;
    }

    if (bDamaged) {
        sprite->changeAnimation(DAMAGED_LEFT);
        damageTimer -= deltaTime;
        if (damageTimer <= 0) {
            bDamaged = false;
            sprite->changeAnimation(facingRight ? STAND_RIGHT : STAND_LEFT);
        }
        return;  // 🔥 ATENCIÓ: sortim aquí, res més es fa!
    }
    else {
        if (Game::instance().getKey(GLFW_KEY_P)) {
            if (!wasKeyPressed) { // Només canvia l'estat quan la tecla s'acaba de prémer
                if (state_teletransport == 0) {
                    posPlayer.x = 129 * 16;
                    posPlayer.y = 4 * 16;
                    state_teletransport = 1;
                }
                else if (state_teletransport == 1) {
                    posPlayer.x = 136 * 16;
                    posPlayer.y = 100 * 16;
                    state_teletransport = 2;
                }
                else if (state_teletransport == 2) {
                    posPlayer.x = 195 * 16;
                    posPlayer.y = 100 * 16;
                    state_teletransport = 3;
                }
                else if (state_teletransport == 3) {
                    posPlayer.x = 207 * 16;
                    posPlayer.y = 36 * 16;
                    state_teletransport = 4;
                }
                else if (state_teletransport == 4) {
                    posPlayer.x = 5 * 16;
                    posPlayer.y = 10 * 16;
                    state_teletransport = 0;
                }
            }
            wasKeyPressed = true; // Marquem que la tecla està premuda
        }
        else {
            wasKeyPressed = false; // Reiniciem quan deixem de prémer la tecla
        }

        sprite->update(deltaTime);

        if (bAttacking && sprite->isLastKeyframe())
        {
            bAttacking = false;
        }

        bool isCrouching = Game::instance().getKey(GLFW_KEY_DOWN);
        bool isBlocking = Game::instance().getKey(GLFW_KEY_UP);
        bool isMovingLeft = Game::instance().getKey(GLFW_KEY_LEFT);
        bool isMovingRight = Game::instance().getKey(GLFW_KEY_RIGHT);

        if (!bAttacking)
        {
            if (isOnFloor) {
                if (Game::instance().getKey(GLFW_KEY_DOWN))  // Si est� presionando la flecha abajo
                {
                    if (Game::instance().getKey(GLFW_KEY_X)) {
                        if (sprite->animation() != CROUCH_ATTACK_LEFT && sprite->animation() != CROUCH_ATTACK_RIGHT) {
                            if (sprite->animation() == BEND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
                                sprite->changeAnimation(CROUCH_ATTACK_LEFT);
                                facingRight = false;
                                if (!spinningActive) {
                                    LanzaOffset = glm::vec2(-4.0f, 24.0f);
                                    TypeLanza = NORMAL_ATTACK_LEFT;
                                }
                                else if (spinningActive) {
                                    LanzaOffset = glm::vec2(-19.0f, 24.0f);
                                    TypeLanza = SA;
                                }
                            }
                            else if (sprite->animation() == BEND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
                                sprite->changeAnimation(CROUCH_ATTACK_RIGHT);
                                facingRight = true;
                                if (!spinningActive) {
                                    LanzaOffset = glm::vec2(60.0f, 24.0f);
                                    TypeLanza = NORMAL_ATTACK_RIGHT;
                                }
                                else if (spinningActive) {
                                    LanzaOffset = glm::vec2(45.0f, 24.0f);
                                    TypeLanza = SA;
                                }
                            }
                        }
                        spawnLanza = true;
                        //bAttacking = true;
                    }

                    else if (!Game::instance().getKey(GLFW_KEY_X)) {
                        if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                            sprite->changeAnimation(BEND_LEFT);
                            facingRight = false;
                        }
                        else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                            sprite->changeAnimation(BEND_RIGHT);
                            facingRight = true;
                        }
                        else {
                            if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == CROUCH_ATTACK_RIGHT) {
                                sprite->changeAnimation(BEND_RIGHT);
                                facingRight = true;
                            }
                            else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == CROUCH_ATTACK_LEFT) {
                                sprite->changeAnimation(BEND_LEFT);
                                facingRight = false;
                            }
                        }
                    }
                }
                else if (Game::instance().getKey(GLFW_KEY_UP))  // Si está presionando la flecha arriba
                {
                    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                        sprite->changeAnimation(UP_BLOCK_LEFT);
                        facingRight = false;
                    }
                    else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                        sprite->changeAnimation(UP_BLOCK_RIGHT);
                        facingRight = true;
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
                    if (isAttacking) {
                        if (Game::instance().getKey(GLFW_KEY_LEFT)) {
                            if (sprite->animation() != ATTACK_LEFT)
                                sprite->changeAnimation(ATTACK_LEFT);
                            facingRight = false;
                            if (!spinningActive) {
                                LanzaOffset = glm::vec2(-5.0f, 16.0f);
                                TypeLanza = NORMAL_ATTACK_LEFT;
                            }
                            else if (spinningActive) {
                                LanzaOffset = glm::vec2(-19.0f, 16.0f);
                                TypeLanza = SA;
                            }

                            posPlayer.x -= 2;
                            if (map->collisionMoveLeft(posPlayer + HITBOX_OFFSET, HITBOX_SIZE)) {
                                posPlayer.x += 2;
                                sprite->changeAnimation(STAND_LEFT);
                            }
                        }
                        else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
                            if (sprite->animation() != ATTACK_RIGHT)
                                sprite->changeAnimation(ATTACK_RIGHT);
                            facingRight = true;
                            if (!spinningActive) {
                                LanzaOffset = glm::vec2(58.0f, 16.0f);
                                TypeLanza = NORMAL_ATTACK_RIGHT;
                            }
                            else if (spinningActive) {
                                LanzaOffset = glm::vec2(43.0f, 16.0f);
                                TypeLanza = SA;
                            }

                            posPlayer.x += 2;
                            if (map->collisionMoveRight(posPlayer + HITBOX_OFFSET, HITBOX_SIZE)) {
                                posPlayer.x -= 2;
                                sprite->changeAnimation(STAND_RIGHT);
                            }
                        }

                        if (sprite->animation() != STAND_ATTACK_LEFT && sprite->animation() != STAND_ATTACK_RIGHT) {
                            if (sprite->animation() == BEND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
                                sprite->changeAnimation(STAND_ATTACK_LEFT);
                                facingRight = false;
                                if (!spinningActive) {
                                    LanzaOffset = glm::vec2(-3.0f, 16.0f);
                                    TypeLanza = NORMAL_ATTACK_LEFT;
                                }
                                else if (spinningActive) {
                                    LanzaOffset = glm::vec2(-18.0f, 16.0f);
                                    TypeLanza = SA;
                                }
                            }
                            else if (sprite->animation() == BEND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
                                sprite->changeAnimation(STAND_ATTACK_RIGHT);
                                facingRight = true;
                                if (!spinningActive) {
                                    LanzaOffset = glm::vec2(60.0f, 16.0f);
                                    TypeLanza = NORMAL_ATTACK_RIGHT;
                                }
                                else if (spinningActive) {
                                    LanzaOffset = glm::vec2(45.0f, 16.0f);
                                    TypeLanza = SA;
                                }
                            }
                        }
                        spawnLanza = true;
                    }
                    else if (!isAttacking) {
                        if (sprite->animation() == STAND_ATTACK_LEFT) {
                            sprite->changeAnimation(STAND_LEFT);
                            facingRight = false;
                        }
                        else if (sprite->animation() == STAND_ATTACK_RIGHT) {
                            sprite->changeAnimation(STAND_RIGHT);
                            facingRight = true;
                        }
                        else if (sprite->animation() == ATTACK_LEFT) {
                            sprite->changeAnimation(MOVE_LEFT);
                            facingRight = false;
                        }
                        else if (sprite->animation() == ATTACK_RIGHT) {
                            sprite->changeAnimation(MOVE_RIGHT);
                            facingRight = true;
                        }

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
                            facingRight = false;

							if (posPlayer.x > 0)
								posPlayer.x -= 2;
                            if (map->collisionMoveLeft(posPlayer + HITBOX_OFFSET, HITBOX_SIZE))
                            {
								posPlayer.x += 2;
                                sprite->changeAnimation(STAND_LEFT);
                            }
                        }
                        else if (Game::instance().getKey(GLFW_KEY_RIGHT))
                        {
                            if (sprite->animation() != MOVE_RIGHT)
                                sprite->changeAnimation(MOVE_RIGHT);
                            facingRight = true;

                            if (posPlayer.x < 254.5 * 16)
                                posPlayer.x += 2;
                            if (map->collisionMoveRight(posPlayer + HITBOX_OFFSET, HITBOX_SIZE))
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
                            facingRight = false;
                        }
                    }

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
                if (map->collisionMoveUp(posPlayer + HITBOX_OFFSET, HITBOX_SIZE, &posPlayer.y) && jumpAngle < 90)
                {
                    jumpAngle = 180 - jumpAngle;
                }

                if (jumpAngle > 90)
                    bJumping = !map->collisionMoveDown(posPlayer + HITBOX_OFFSET, HITBOX_SIZE, &posPlayer.y);
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
            if (!bOnMovingLog)
            {
                posPlayer.y += FALL_STEP;
                if (map->collisionMoveDown(posPlayer + HITBOX_OFFSET, HITBOX_SIZE, &posPlayer.y))
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
            else {
                if (Game::instance().getKey(GLFW_KEY_Z))
                {
                    bJumping = true;
                    jumpAngle = 0;
                    startY = posPlayer.y;

                    if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
                        sprite->changeAnimation(BEND_LEFT);
                    else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
                        sprite->changeAnimation(BEND_RIGHT);
                }
            }
            if ((sprite->animation() == UP_ATTACK_LEFT || sprite->animation() == UP_ATTACK_RIGHT) && !Game::instance().getKey(GLFW_KEY_UP)) {
                bAttacking = false;
                sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
            }
            else if ((sprite->animation() == DOWN_ATTACK_LEFT || sprite->animation() == DOWN_ATTACK_RIGHT) && !Game::instance().getKey(GLFW_KEY_DOWN)) {
                bAttacking = false;
                sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
            }
        }

        bool isUpPressed = Game::instance().getKey(GLFW_KEY_UP);
        bool isDownPressed = Game::instance().getKey(GLFW_KEY_DOWN);

        // ✅ Si está en el aire y se presiona ↑, reproducir animación UP_ATTACK
        if (bJumping) {
            if ((sprite->animation() == UP_ATTACK_LEFT || sprite->animation() == UP_ATTACK_RIGHT) && !Game::instance().getKey(GLFW_KEY_UP)) {
                bAttacking = false;
                sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
            }
            else if ((sprite->animation() == DOWN_ATTACK_LEFT || sprite->animation() == DOWN_ATTACK_RIGHT) && !Game::instance().getKey(GLFW_KEY_DOWN)) {
                bAttacking = false;
                sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
            }

            if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == BEND_RIGHT)
                facingRight = true;
            else if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == BEND_LEFT)
                facingRight = false;

            if (!isAttacking) {
                if (isUpPressed) {
                    // Sin dirección horizontal, mantener dirección actual
                    if (facingRight) {
                        if (sprite->animation() != UP_ATTACK_RIGHT) {
                            sprite->changeAnimation(UP_ATTACK_RIGHT);
                        }
                        LanzaOffset = glm::vec2(36.0f, -16.0f);
                        TypeLanza = ATTACK_UP_RIGHT;
                    }
                    else {
                        if (sprite->animation() != UP_ATTACK_LEFT) {
                            sprite->changeAnimation(UP_ATTACK_LEFT);
                        }
                        LanzaOffset = glm::vec2(-28.0f, -16.0f);
                        TypeLanza = ATTACK_UP_LEFT;
                    }
                    spawnLanza = true;
                    bAttacking = true;
                }
                else if (isDownPressed) {
                    // Ataque hacia abajo en el aire
                    if (facingRight) {
                        if (sprite->animation() != DOWN_ATTACK_RIGHT) {
                            sprite->changeAnimation(DOWN_ATTACK_RIGHT);
                        }
                        LanzaOffset = glm::vec2(32.0f, 48.0f);
                        TypeLanza = ATTACK_DOWN_RIGHT;
                    }
                    else {
                        if (sprite->animation() != DOWN_ATTACK_LEFT) {
                            sprite->changeAnimation(DOWN_ATTACK_LEFT);
                        }
                        LanzaOffset = glm::vec2(-32.0f, 48.0f);
                        TypeLanza = ATTACK_DOWN_LEFT;
                    }

                    // Preparar lanza para ataque hacia abajo
                    spawnLanza = true;
                    bAttacking = true;
                }
                else {
                    // Al soltar la tecla, cancelar ataque aéreo
                    if (bAttacking && (sprite->animation() == UP_ATTACK_LEFT || sprite->animation() == UP_ATTACK_RIGHT ||
                        sprite->animation() == DOWN_ATTACK_LEFT || sprite->animation() == DOWN_ATTACK_RIGHT)) {
                        bAttacking = false;
                        // Volver a animación de caída normal
                        sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
                    }
                }
            }
            else {
                // Si aterriza mientras ataca, cancelar ataque y estado
                if (bAttacking && (sprite->animation() == UP_ATTACK_LEFT || sprite->animation() == UP_ATTACK_RIGHT ||
                    sprite->animation() == DOWN_ATTACK_LEFT || sprite->animation() == DOWN_ATTACK_RIGHT)) {
                    bAttacking = false;
                    if (Game::instance().getKey(GLFW_KEY_UP))
                        sprite->changeAnimation(facingRight ? UP_BLOCK_RIGHT : UP_BLOCK_LEFT);
                    else if (Game::instance().getKey(GLFW_KEY_DOWN))
                        sprite->changeAnimation(facingRight ? BEND_RIGHT : BEND_LEFT);
                    else
                        sprite->changeAnimation(facingRight ? STAND_RIGHT : STAND_LEFT);
                }
            }
        }
    }

    if (spawnLanza) {
        if (lanza == nullptr) {
            lanza = new Lanza();
            lanza->init(glm::vec2(posPlayer) + LanzaOffset, *shaderProgram, TypeLanza, !facingRight);
            lanza->setTileMap(map);
            if (TypeLanza == NORMAL_ATTACK_LEFT || TypeLanza == NORMAL_ATTACK_RIGHT) {
                attackTimer = ATTACK_DURATION;
            }
            else if (TypeLanza == SA) {
                attackTimer = 1000;
            }
            else {
                attackTimer = 0;
            }
        }
        else {
            lanza->updatePosition(glm::vec2(posPlayer), LanzaOffset, !facingRight);
        }
        lanza->update(deltaTime);
    }
    else if (lanza != nullptr) {
        lanza->deactivate();
        delete lanza;
        lanza = nullptr;
        attackTimer = 0;
    }

    if (attackTimer > 0) {
        attackTimer -= deltaTime;
        if (attackTimer <= 0) {
            attackTimer = 0;
            if (lanza != nullptr && lanza->isActive()) {
                if (spinningActive) {
                    if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                        LanzaOffset = facingRight ? glm::vec2(60.0f, 24.0f) : glm::vec2(-4.0f, 24.0f);
                        lanza->changeAnimation(facingRight ? HOLD_RIGHT : HOLD_LEFT);
                    }
                    else {
                        LanzaOffset = facingRight ? glm::vec2(60.0f, 16.0f) : glm::vec2(-3.0f, 16.0f);
                        lanza->changeAnimation(facingRight ? HOLD_RIGHT : HOLD_LEFT);
                    }
                    lanza->updatePosition(glm::vec2(posPlayer), LanzaOffset, !facingRight);

                }
                else {
                    if (facingRight) {
                        lanza->changeAnimation(HOLD_RIGHT);
                    }
                    else if (!facingRight) {
                        lanza->changeAnimation(HOLD_LEFT);
                    }
                    lanza->updatePosition(glm::vec2(posPlayer), LanzaOffset, !facingRight);
                }
            }
        }
    }


    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


Lanza* Player::getLanza() const {
    return lanza;
}

void Player::animacioDamage() {
    if (bDamaged) return; // Ja està en mode dany, no cal repetir

    bDamaged = true;
    damageTimer = 500; // Durada de l'animació de dany (ms)

    if (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)
        facingRight = false;
    else if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
        facingRight = true;

    sprite->changeAnimation(DAMAGED_RIGHT);
}

int Player::getCurrentAnimation() const {
    return sprite->animation();
}

void Player::render()
{
    sprite->render();
    if (lanza && lanza->isActive())
        lanza->render();
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
