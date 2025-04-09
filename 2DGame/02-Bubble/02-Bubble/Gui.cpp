#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Gui.h"
#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <map>

enum Arma {
	ARM
};

enum Lamp {
	LINTERNA
};

void Gui::init(const glm::ivec2& screenPos, ShaderProgram& shaderProgram) {
    spritesheet.loadFromFile("images/gui.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // Arma
    arma = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
    arma->setNumberAnimations(1);
    arma->setAnimationSpeed(ARM, 8);
    arma->addKeyframe(ARM, glm::vec2(0.f, 0.f));
    arma->changeAnimation(ARM);
    arma->setPosition(screenPos + glm::ivec2(0, 0));  // Posició sobre els cors

    // Llanternes
    for (int i = 0; i < num_lint; ++i) {
        Sprite* linterna = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
        linterna->setNumberAnimations(1);
        linterna->setAnimationSpeed(LINTERNA, 8);
        linterna->addKeyframe(LINTERNA, glm::vec2(0.25f, 0.f));
        linterna->changeAnimation(LINTERNA);
        linterna->setPosition(screenPos + glm::ivec2(20 + i * 18, 0));  // Al costat de l'arma
        linternes.push_back(linterna);
        linternaVisible.push_back(true);
    }
}

void Gui::setLives(int l) {
    lives = l;
    for (int i = 0; i < linternes.size(); ++i) {
        linternaVisible[i] = (i+1 < lives);  // només mostrar les que calgui
    }
}

void Gui::render() {
    if (arma) arma->render();

    for (int i = 0; i < linternes.size(); ++i) {
        if (linternaVisible[i] == true) linternes[i]->render();
    }
}

void Gui::setPosition(const glm::ivec2& pos) {
    if (arma) arma->setPosition(glm::vec2(pos.x, pos.y)); // arma

    for (int i = 0; i < linternes.size(); ++i) {
        linternes[i]->setPosition(glm::vec2(pos.x + 20 + i * 20, pos.y)); // espaiat
    }

}

