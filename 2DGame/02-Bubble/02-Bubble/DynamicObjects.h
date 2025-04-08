#pragma once
#ifndef _DYNAMIC_OBJECTS_INCLUDE
#define _DYNAMIC_OBJECTS_INCLUDE

#include <vector>
#include "MovingLog.h"

class DynamicObjects
{
public:
    void init(ShaderProgram& shaderProgram);
    void update(int deltaTime, Player& player);
    void render();

    void addMovingLog(const glm::ivec2& pos, int minY, int maxY, ShaderProgram& shaderProgram);
    bool isOnMovingPlatform(const glm::ivec2& playerPos);
    void applyPlatformMovement(glm::ivec2& playerPos);

private:
    std::vector<MovingLog*> woods;
};

#endif // _DYNAMIC_OBJECTS_INCLUDE

