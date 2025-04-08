#include "Lanza.h"
#include "Game.h"

void Lanza::init(const glm::vec2& playerPos, ShaderProgram& shaderProgram, int attackType, bool facingLeft)
{
    active = true;

    spritesheet.loadFromFile("images/PersonajeVJ3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.2), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(8); // 0: anim derecha, 1: anim izquierda, 2: punta fija

    // Animación derecha (0,0) -> (0.5,0) -> (0,0.25)
    sprite->setAnimationSpeed(NORMAL_ATTACK_RIGHT, 16);
    sprite->addKeyframe(NORMAL_ATTACK_RIGHT, glm::vec2(0.4f, 0.8f));
    sprite->addKeyframe(NORMAL_ATTACK_RIGHT, glm::vec2(0.5f, 0.8f));
    sprite->addKeyframe(NORMAL_ATTACK_RIGHT, glm::vec2(0.6f, 0.8f));

    sprite->setAnimationSpeed(NORMAL_ATTACK_LEFT, 16);
    sprite->addKeyframe(NORMAL_ATTACK_LEFT, glm::vec2(0.8f, 0.0f));
    sprite->addKeyframe(NORMAL_ATTACK_LEFT, glm::vec2(0.9f, 0.0f));
    sprite->addKeyframe(NORMAL_ATTACK_LEFT, glm::vec2(0.8f, 0.2f));

    sprite->setAnimationSpeed(HOLD_RIGHT, 12);
    sprite->addKeyframe(HOLD_RIGHT, glm::vec2(0.4f, 0.8f));
    sprite->setAnimationSpeed(HOLD_LEFT, 12);
    sprite->addKeyframe(HOLD_LEFT, glm::vec2(0.8f, 0.0f));

    sprite->setAnimationSpeed(ATTACK_DOWN_LEFT, 12);
    sprite->addKeyframe(ATTACK_DOWN_LEFT, glm::vec2(0.3f, 0.8f));

    sprite->setAnimationSpeed(ATTACK_DOWN_RIGHT, 12);
    sprite->addKeyframe(ATTACK_DOWN_RIGHT, glm::vec2(0.2f, 0.8f));

    sprite->setAnimationSpeed(ATTACK_UP_LEFT, 12);
    sprite->addKeyframe(ATTACK_UP_LEFT, glm::vec2(0.0f, 0.6f));

    sprite->setAnimationSpeed(ATTACK_UP_RIGHT, 12);
    sprite->addKeyframe(ATTACK_UP_RIGHT, glm::vec2(0.1f, 0.6f));

    sprite->changeAnimation(attackType);
    position = playerPos;
    sprite->setPosition(position);

}



    void Lanza::update(int deltaTime) {
        if (active && sprite) {
            sprite->update(deltaTime);
        }
    }

    void Lanza::render() {
        if (active)
            sprite->render();
    }

    bool Lanza::isActive() const {
        return active;
    }

    void Lanza::deactivate() {
        active = false;
    }

	void Lanza::changeAnimation(int animId) {
		if (sprite) {
			sprite->changeAnimation(animId);
		}
	}

    void Lanza::setTileMap(TileMap * tileMap) {
        map = tileMap;
    }

    // Permite actualizar la posición de la lanza en función de la posición actual del jugador y el offset deseado
    void Lanza::updatePosition(const glm::vec2 & playerPos, const glm::vec2 & offset, bool facingLeft) {
        if (facingLeft) {
            position = playerPos - glm::vec2(offset.x, -offset.y);  // invertir en X solo
        }
        else {
            position = playerPos + offset;
        }


        sprite->setPosition(position);
    }