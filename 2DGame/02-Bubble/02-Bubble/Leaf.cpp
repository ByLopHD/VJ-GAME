#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Leaf.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

void Leaf::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
    spritesheet.loadFromFile("images/atac_boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.5f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));

    sprite->changeAnimation(0);

    tileMapDispl = position;
    active = true;
    posEnemy = glm::ivec2(0);

    timeElapsed = 0.f;
    maxRadius = 3.f * 16; // 3 tiles
    angle = 0.f; // cada fulla tindrà el seu angle únic

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void Leaf::update(int deltaTime) {
    if (!active) return;

    timeElapsed += deltaTime;

    // Oscil·lació del radi
    float t = float(timeElapsed) / 3000.f * 3.14159f;
    float radius = sin(t) * maxRadius;

    // 🔁 Rotació constant (0.002f rad/ms = ~1.15 rad/s)
    angle += 0.0010f * deltaTime;

    // Manté angle dins rang [0, 2π]
    if (angle > 2.f * 3.14159f) angle -= 2.f * 3.14159f;

    // Nova posició
    posEnemy.x = center.x + cos(angle) * radius;
    posEnemy.y = center.y + sin(angle) * radius;

    sprite->update(deltaTime);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Leaf::render()
{
    sprite->render();
}

void Leaf::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Leaf::setPosition(const glm::vec2& pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Leaf::setPlayerPosition(const glm::ivec2& pos) {
    posPlayer = pos;
}

void Leaf::setAngle(float angleDegrees) {
    angle = angleDegrees * 3.14159f / 180.f;
}

void Leaf::setInitialPosition(const glm::vec2& pos) {
    center = pos;
    timeElapsed = 0.f;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + center.x), float(tileMapDispl.y + center.y)));
}





