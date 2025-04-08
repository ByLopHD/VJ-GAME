#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Lanza.h"
#include <vector>




class Player {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    void setOnMovingLog(bool onLog);

    void moveWithPlatform(const glm::ivec2& movement);

    void animacioDamage();

    int getCurrentAnimation() const;

    glm::ivec2 getPosition() { return posPlayer; }
    glm::ivec2& getPositionRef();


private:
    bool bJumping, bAttacking, bCrouching;
    glm::ivec2 tileMapDispl, posPlayer;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    ShaderProgram* shaderProgram;
	int state_teletransport = 0;
    Lanza* lanza;
    int currentLanzaAnim;
    glm::vec2 LanzaOffset = glm::vec2(0.0f);

    bool bOnMovingLog;
	int attackTimer = 0;

    bool bDamaged = false;
    int damageTimer = 0;
    bool facingRight = true; // Si vols controlar la direcció visual
};

#endif // _PLAYER_INCLUDE