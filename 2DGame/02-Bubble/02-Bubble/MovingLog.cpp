#include "MovingLog.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void MovingLog::init(const glm::ivec2& pos, ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/log.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);

    position = pos;
    sprite->setPosition(glm::vec2(position));
}

void MovingLog::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void MovingLog::setMovementRange(int minY_, int maxY_)
{
    minY = minY_;
    maxY = maxY_;
}

void MovingLog::update(int deltaTime)
{
    lastPosition = position;
    if (movingDown)
    {
        position.y += speed;
        if (position.y >= maxY)
            movingDown = false;
    }
    else
    {
        position.y -= speed;
        if (position.y <= minY)
            movingDown = true;
    }

    sprite->setPosition(glm::vec2(position));
    sprite->update(deltaTime);
}

void MovingLog::render()
{
    sprite->render();
}

bool MovingLog::isPlayerOnTop(const glm::ivec2& playerPos) const
{
    int playerBottom = playerPos.y + 32;
    int logTop = position.y;


    bool onTop = (
        playerPos.x + 50 > position.x &&
        playerPos.x + 45 < position.x + size.x &&
        playerBottom >= logTop - 20 &&  
        playerBottom <= logTop + 20
        );

    return onTop;
}

glm::ivec2 MovingLog::getPosition() const
{
    return position;
}
glm::ivec2 MovingLog::getMovementOffset() const {
    return position - lastPosition;
}