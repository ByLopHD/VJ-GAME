#include "FallingBamboo.h"
#include <glm/gtc/matrix_transform.hpp>

void FallingBamboo::init(const glm::ivec2& position, ShaderProgram& shaderProgram)
{
    spritesheet.loadFromFile("images/Bambu.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);

    posBamboo = position;
    sprite->setPosition(glm::vec2(posBamboo));
}

void FallingBamboo::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void FallingBamboo::setPlayerPosition(const glm::ivec2& pPos)
{
    playerPos = pPos;
}

void FallingBamboo::update(int deltaTime)
{
    if (!active) return;

    posBamboo.y += speed;
    

    sprite->setPosition(glm::vec2(posBamboo));

    if (posBamboo.y > 480) { // ← ajusta este valor según el alto de tu ventana
        active = false;
    }
}

void FallingBamboo::render()
{
    if (active)
        sprite->render();
}

bool FallingBamboo::checkCollisionWithPlayer() const
{
    if (!active) return false;

    return (
        playerPos.x < posBamboo.x + size.x &&
        playerPos.x + 32 > posBamboo.x &&
        playerPos.y < posBamboo.y + size.y &&
        playerPos.y + 32 > posBamboo.y
        );
}

bool FallingBamboo::isActive() const {
    return active;
}



