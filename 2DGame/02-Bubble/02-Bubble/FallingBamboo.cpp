#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "FallingBamboo.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

void FallingBamboo::init(const glm::ivec2& position, ShaderProgram& shaderProgram) {
    spritesheet.loadFromFile("images/bambo.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);

    tileMapDispl = position;
    posEnemy = glm::ivec2(0);
    active = true;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void FallingBamboo::update(int deltaTime) {
    //posEnemy.y += int(3.f * (deltaTime / 16.f));
    
    if (!wasBlocked) {
        posEnemy.y += velocity.y;
    }
    else {
        // Després de rebotar una mica, segueix caient
        velocity.y += 0.1f;  // gravetat gradual
        posEnemy.y += velocity.y;
    }
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x),
        float(tileMapDispl.y + posEnemy.y)));

}

void FallingBamboo::onBlocked() {
    if (wasBlocked) return;

    wasBlocked = true;
    velocity.y = -1.5f; // petit rebot cap amunt
}

bool FallingBamboo::isBlocked() {
    return wasBlocked;
}

void FallingBamboo::render()
{
	sprite->render();
}

void FallingBamboo::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void FallingBamboo::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void FallingBamboo::setPlayerPosition(const glm::ivec2& pos) {
	posPlayer = pos;
}

bool FallingBamboo::isActive() const {
    return active;
}

void FallingBamboo::setActive(bool b) {
    active = b;
}



