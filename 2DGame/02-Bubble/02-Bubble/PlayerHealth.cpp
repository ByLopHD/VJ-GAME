#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "PlayerHealth.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <map>

enum Anim {
	COR_SENCER, COR_3_4, COR_1_4, COR_BUIT
};



void PlayerHealth::init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/corazones.png", TEXTURE_PIXEL_FORMAT_RGBA);

	for (int i = 0; i < maxHearts; ++i) {
		Sprite* heart = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25f, 1.0f), &spritesheet, &shaderProgram);
		heart->setNumberAnimations(4);
		
		heart->setAnimationSpeed(COR_SENCER, 8);
		heart->addKeyframe(COR_SENCER, glm::vec2(0.f, 0.f));

		heart->setAnimationSpeed(COR_3_4, 8);
		heart->addKeyframe(COR_3_4, glm::vec2(0.25f, 0.f));

		heart->setAnimationSpeed(COR_1_4, 8);
		heart->addKeyframe(COR_1_4, glm::vec2(0.5f, 0.f));

		heart->setAnimationSpeed(COR_BUIT, 8);
		heart->addKeyframe(COR_BUIT, glm::vec2(0.75f, 0.f));

		heart->changeAnimation(COR_SENCER);

		hearts.push_back(heart);
	}
}

void PlayerHealth::setLives(int n) {
	lives = n;
}

void PlayerHealth::restoreFull() {
	health = float(maxHearts);
}

void PlayerHealth::restoreOneHeart() {
	health += 1.0f;
	if (health > maxHearts)
		health = maxHearts;
}

void PlayerHealth::loseLife() {
	if (lives > 0) {
		lives--;
		restoreFull();
	}
}

void PlayerHealth::render() {
	float hp = health;

	for (int i = 0; i < maxHearts; ++i) {
		int anim;

		if (hp >= 1.0f) anim = COR_SENCER;
		else if (hp >= 0.67f) anim = COR_3_4;
		else if (hp >= 0.34f) anim = COR_1_4;
		else anim = COR_BUIT;

		if (hearts[i]->animation() != anim)
			hearts[i]->changeAnimation(anim);

		hearts[i]->render();
		hp -= 1.0f;
	}
}

void PlayerHealth::setHeartPosition(int i, glm::vec2 pos) {
	if (i >= 0 && i < hearts.size()) {
		hearts[i]->setPosition(pos);
	}
}


void PlayerHealth::takeDamage(float amount) {
	health -= amount;
	if (health <= 0.2f && lives > 0) {
		loseLife();
	}
}

void PlayerHealth::addPumpkin(ShaderProgram& shaderProgram) {
	pumpkinCount++;
	tryAddExtraHeart(shaderProgram);
}

void PlayerHealth::tryAddExtraHeart(ShaderProgram& shaderProgram) {
	static const std::vector<int> pumpkinThresholds = { 9, 12, 16, 22, 30, 42, 62, 99 };
	if (maxHearts >= 12) return;  // màxim 12 cors
	if (pumpkinCount < pumpkinThresholds[maxHearts - 4]) return;

	// Creem un nou sprite de cor buit
	Sprite* newHeart = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25f, 1.0f), &spritesheet, &shaderProgram);
	newHeart->setNumberAnimations(4);

	newHeart->setAnimationSpeed(COR_SENCER, 8);
	newHeart->addKeyframe(COR_SENCER, glm::vec2(0.f, 0.f));

	newHeart->setAnimationSpeed(COR_3_4, 8);
	newHeart->addKeyframe(COR_3_4, glm::vec2(0.25f, 0.f));

	newHeart->setAnimationSpeed(COR_1_4, 8);
	newHeart->addKeyframe(COR_1_4, glm::vec2(0.5f, 0.f));

	newHeart->setAnimationSpeed(COR_BUIT, 8);
	newHeart->addKeyframe(COR_BUIT, glm::vec2(0.75f, 0.f));

	newHeart->changeAnimation(COR_BUIT);

	// Posicionem el cor visual (es vertical)
	int spacing = 10;
	newHeart->setPosition(glm::vec2(10.f, 10.f + maxHearts * spacing));

	hearts.push_back(newHeart);
	maxHearts++;
}





