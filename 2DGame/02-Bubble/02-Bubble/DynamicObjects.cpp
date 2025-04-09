#include "DynamicObjects.h"
#include "MovingLog.h"
#include <iostream>

void DynamicObjects::init(ShaderProgram& shaderProgram)
{
    woods.clear();

    // Ejemplo de tronco: posición X = 100 tiles, rango Y de 200 a 250
    addMovingLog(glm::ivec2(201 * 16, 49 * 16), 49 * 16, 52 * 16, shaderProgram);
    addMovingLog(glm::ivec2(199 * 16, 46.5 * 16), 46.5 * 16, 49 * 16, shaderProgram);
    addMovingLog(glm::ivec2(204 * 16, 45.5 * 16), 45.5 * 16, 48 * 16, shaderProgram);
    addMovingLog(glm::ivec2(200 * 16, 41 * 16), 41 * 16, 45 * 16, shaderProgram);
    addMovingLog(glm::ivec2(198 * 16, 37.5 * 16), 37.5 * 16, 40 * 16, shaderProgram);
    addMovingLog(glm::ivec2(202 * 16, 36 * 16), 36 * 16, 39 * 16, shaderProgram);
}

void DynamicObjects::addMovingLog(const glm::ivec2& pos, int minY, int maxY, ShaderProgram& shaderProgram)
{
    std::cout << "Colocando jugador sobre el tronco\n";
    MovingLog* wood = new MovingLog();
    wood->init(pos, shaderProgram);
    wood->setMovementRange(minY, maxY);
    woods.push_back(wood);
}

void DynamicObjects::update(int deltaTime)
{
    for (auto& wood : woods)
        wood->update(deltaTime);
}

void DynamicObjects::render()
{
    for (auto& wood : woods)
        wood->render();
}

bool DynamicObjects::isOnMovingPlatform(const glm::ivec2& playerPos)
{
    for (auto& wood : woods)
    {
        if (wood->isPlayerOnTop(playerPos))
            return true;
    }
    return false;
}

void DynamicObjects::applyPlatformMovement(glm::ivec2& playerPos)
{
    for (auto& wood : woods)
    {
        if (wood->isPlayerOnTop(playerPos))
        {
            glm::ivec2 woodPos = wood->getPosition();

            if (playerPos.y + 32 > woodPos.y)
            {
                playerPos.y = woodPos.y - 48;
                std::cout << "Colocando jugador sobre el tronco\n";
            }
            playerPos.y = woodPos.y - 48; // colocar justo encima
        }
    }
}