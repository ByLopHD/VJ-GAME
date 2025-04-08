#pragma once
#ifndef _MOVING_LOG_INCLUDE
#define _MOVING_LOG_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class MovingLog
{
public:
    void init(const glm::ivec2& pos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setMovementRange(int minY, int maxY);

    bool isPlayerOnTop(const glm::ivec2& playerPos) const;
    glm::ivec2 getPosition() const;
    glm::ivec2 MovingLog::getMovementOffset() const;

private:
    glm::ivec2 position;
    glm::ivec2 size = glm::ivec2(32, 16);
    int minY, maxY;
    int speed = 1;
    bool movingDown = true;
    glm::ivec2 lastPosition;


    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    glm::ivec2* playerPos = nullptr;
};

#endif
