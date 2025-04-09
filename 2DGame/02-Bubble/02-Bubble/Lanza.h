#ifndef _LANZA_INCLUDE
#define _LANZA_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

enum LanzaAnims {
    NORMAL_ATTACK_RIGHT = 0,
    NORMAL_ATTACK_LEFT,// Usado para ataque normal, tanto quieto como en movimiento y para ATTACK_CROUCH
    ATTACK_UP_RIGHT,      // Para ataque hacia arriba
    ATTACK_DOWN_RIGHT,
    ATTACK_UP_LEFT,
    ATTACK_DOWN_LEFT,
    HOLD_RIGHT,
    HOLD_LEFT,
    SA
};
class Lanza
{
public:
    void init(const glm::vec2& playerPos, ShaderProgram& shaderProgram, int attackType, bool facingLeft);
    void update(int deltaTime);
    void render();
    bool isActive() const;
    void deactivate();
    void setTileMap(TileMap* tileMap);

    // Actualiza la posición de la lanza en función de la posición del jugador y un offset calculado
    void updatePosition(const glm::vec2& playerPos, const glm::vec2& offset, bool facingLeft);
    void changeAnimation(int animId);

    glm::ivec2 getPosition() { return position; }

    pair<glm::ivec2, glm::ivec2> Lanza::getHitbox();

private:
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    glm::ivec2 position;
    bool active;
};

#endif // _LANZA_INCLUDE