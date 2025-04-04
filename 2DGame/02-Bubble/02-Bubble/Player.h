#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Player {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    void setOnMovingLog(bool onLog);

    void animacioDamage();


    glm::ivec2 getPosition() { return posPlayer; }
    glm::ivec2& getPositionRef();

private:
    bool bJumping, bAttacking, bCrouching;
    glm::ivec2 tileMapDispl, posPlayer;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

	int state_teletransport = 0;

    bool bOnMovingLog;

    bool bDamaged = false;
    int damageTimer = 0;
    bool facingRight = true; // Si vols controlar la direcció visual
};

#endif // _PLAYER_INCLUDE