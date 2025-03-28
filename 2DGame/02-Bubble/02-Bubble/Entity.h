#ifndef ENTITY_H
#define ENTITY_H

#include "Sprite.h"
#include "TileMap.h"

class Entity
{
public:
    virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;

    void setPosition(const glm::vec2& pos) { position = pos; }
    glm::ivec2 getPosition() { return position; }

protected:
    glm::vec2 position;
    Sprite* sprite;
    TileMap* map;
};

#endif // ENTITY_H