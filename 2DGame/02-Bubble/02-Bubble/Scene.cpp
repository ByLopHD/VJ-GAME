#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Mapa_orignial_Terra.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	tileSize = float(map->getTileSize());

	camPosX = (7+6) * tileSize; //Posicio inicial de la camera
	camPosY = (7+1.5) * tileSize;

	projX = 15 * tileSize; //Tamany de la projeccio
	projY = 15 * tileSize;

	configCam();

	/*
	fondo.loadFromFile("images/nose2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	liveFondo = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(1.0 / 6.0f, 1.0 / 3.0f), &fondo, &texProgram);
	liveFondo->setNumberAnimations(1);
	liveFondo->setAnimationSpeed(0, 1);
	liveFondo->addKeyframe(0, glm::vec2(1.0f / 3.0f, 2.0 / 3.0f));
	liveFondo->changeAnimation(0);
	*/

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	configCam();
}

/*

void Scene::configCam() {
	glm::vec2 pos = player->getPosition();
	float aspectR = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	float viewW = projY * aspectR;
	float viewH = projY;
	//float viewH = projY * aspectR;

	if (pos.x + tileSize <= 11 * tileSize) {
		int posAct = camPosX;
		camPosX = (7 + 6) * tileSize;
		projection = glm::ortho(camPosX - viewW / 2.0f, camPosX + viewW / 2.0f, camPosY + projY / 2.0f, camPosY - projY / 2.0f);
		camPosX = posAct;
	}

	else if ((pos.x + tileSize > 129 * tileSize && pos.x + tileSize < 145 * tileSize) || (pos.x + tileSize > 193 * tileSize && pos.x + tileSize < 207 * tileSize)) {
		// Centrar el personatge en Y i mantenir X fixa
		float targetY = pos.y - (7 * tileSize);
		float maxY = 105 * tileSize - projY;
		float minY = 0.0f;
		targetY = std::max(minY, std::min(targetY, maxY));
		targetY = targetY + 1 * tileSize;
		float fixedX;

		if ((pos.x + tileSize > 129 * tileSize && pos.x + tileSize < 145 * tileSize)) {
			fixedX = 136 * tileSize; // Ajusta aquest valor si cal
		}
		else if (pos.x + tileSize > 193 * tileSize && pos.x + tileSize < 207 * tileSize) {
			fixedX = 200 * tileSize;
		}

		projection = glm::ortho(fixedX - viewW / 2.0f, fixedX + viewW / 2.0f, (targetY + projY), targetY);

		camPosY = targetY + viewH / 2.0f;
	
	}

	else {
		// Centrar el personatge a la pantalla amb 7 tiles a l'esquerra i 7 a la dreta

		float targetX = pos.x - (7 * tileSize);
		float minX = 0.0f;
		float maxX = 256 * tileSize - projY;
		targetX = std::max(minX, std::min(targetX, maxX));
		projection = glm::ortho(targetX, targetX + viewW, camPosY + projY / 2.0f, camPosY - projY / 2.0f);

		camPosX = targetX + viewH / 2.0f;

		//projection = glm::ortho(pos.x + tileSize - viewW / 2.0f, pos.x + tileSize + viewW / 2.0f, camPosY + projY / 2.0f, camPosY - projY / 2.0f);
	}
	//projection = glm::ortho(pos.y + tileSize - viewH / 2.0f, pos.y + tileSize + viewH / 2.0f, camPosX + projX / 2.0f, camPosX - projX / 2.0f);
	currentTime = 0.0f;
}
*/


void Scene::configCam() {
	glm::vec2 pos = player->getPosition();
	float aspectR = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	float viewW = projY * aspectR;
	float viewH = projY;
	//float viewH = projY * aspectR;

	if (pos.x +tileSize < 8 * tileSize) {
		//float camX = 8 * tileSize;
		projection = glm::ortho(float(2 * tileSize), float(18 * tileSize), float(16 * tileSize), float(1 * tileSize));
	}

	else if (pos.x + tileSize > 248 * tileSize) {
		projection = glm::ortho(float(242 * tileSize), float(258 * tileSize), float(46 * tileSize), float(31 * tileSize));
	}

	else if ((pos.x + tileSize > 129 * tileSize && pos.x + tileSize < 145 * tileSize)) {
		if (pos.y + tileSize < (7.5 +2) * tileSize) {
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), float(16 * tileSize), float(1 * tileSize));
		}
		else if (pos.y + tileSize > (97.5 + 2) * tileSize) {
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), float(106 * tileSize), float(91 * tileSize));
		}
		else {
			float targetY = pos.y;
			projection = glm::ortho(float(130 * tileSize), float(146 * tileSize), targetY + (float(7.5 * tileSize)), targetY - (float(7.5 * tileSize)));
		}
	}

	else if (pos.x + tileSize > 193 * tileSize && pos.x + tileSize < 208 * tileSize) {
		
		if (pos.y + tileSize < (37.5 + 2) * tileSize) {
		projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), float(46 * tileSize), float(31 * tileSize));
		}
		
		else if (pos.y + tileSize > (97.5 + 2) * tileSize) {
			projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), float(106 * tileSize), float(91 * tileSize));
		}
		else {
			float targetY = pos.y;
			projection = glm::ortho(float(194 * tileSize), float(209 * tileSize), targetY + (float(7.5 * tileSize)), targetY - (float(7.5 * tileSize)));
		}
	}

	else {
		// Centrar el personatge a la pantalla amb 7 tiles a l'esquerra i 7 a la dreta
		if (pos.x + tileSize < 145 * tileSize) camPosY = (7 + 1.5) * tileSize;
		else if (pos.x + tileSize < 208 * tileSize) camPosY = (97 + 1.5) * tileSize;
		else camPosY = (37 + 1.5) * tileSize;

		float targetX = pos.x + (3*tileSize);
		
		projection = glm::ortho(targetX - (float(8 * tileSize)), targetX + (float(8 * tileSize)), camPosY + projY / 2.0f, camPosY - projY / 2.0f);

		camPosX = targetX + viewH / 2.0f;

	}
	//projection = glm::ortho(pos.y + tileSize - viewH / 2.0f, pos.y + tileSize + viewH / 2.0f, camPosX + projX / 2.0f, camPosX - projX / 2.0f);
	currentTime = 0.0f;
}




float clamp(float value, float minVal, float maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}


void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



