#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);

	background.use();
	glBindVertexArray(bVao);
	glEnableVertexAttribArray(backPosLocation);
	glEnableVertexAttribArray(backTexCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnd();


	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin(levelFile);
	if (!fin.is_open())
		return false;

	string line, tilesheetFile, backgroundFile;
	stringstream sstream;

	getline(fin, line); // TILEMAP
	getline(fin, line); // Map size
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	sstream.clear();

	getline(fin, line); // Tile size & block size
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	sstream.clear();

	getline(fin, line); // Tilesheet
	sstream.str(line);
	sstream >> tilesheetFile;
	sstream.clear();

	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);


	getline(fin, line);
	sstream.str(line);
	sstream >> backgroundFile;
	background.loadFromFile(backgroundFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);


	getline(fin, line); // Tilesheet size
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	sstream.clear();

	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	backgroundSize.x = mapSize.x * blockSize;
	backgroundSize.y = mapSize.y * blockSize;

	map = new int[mapSize.x * mapSize.y];

	for (int j = 0; j < mapSize.y; j++) {
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < mapSize.x; i++) {
			int tile;
			sstream >> tile;
			map[j * mapSize.x + i] = (tile == -1) ? 0 : tile;
			//cout << "Tile a [" << j << "," << i << "]: " << tile << endl;

		}
		sstream.clear();
	}

	fin.close();
	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());

	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			tile = map[j * mapSize.x + i];
			if (tile > 0) {
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float(tile % tilesheetSize.x) / tilesheetSize.x,
					float(tile / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;

				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);

				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	vector<float> backgroundVertices;
	float offsetX = 2 * tileSize; // 2 tiles cap a la dreta
	float offsetY = 1 * tileSize; // 1 tile cap avall

	//First triangle
	backgroundVertices.push_back(offsetX); backgroundVertices.push_back(offsetY);
	backgroundVertices.push_back(0.f); backgroundVertices.push_back(0.f);
	backgroundVertices.push_back(backgroundSize.x + offsetX); backgroundVertices.push_back(offsetY);
	backgroundVertices.push_back(1.f); backgroundVertices.push_back(0.f);
	backgroundVertices.push_back(backgroundSize.x + offsetX); backgroundVertices.push_back(backgroundSize.y + offsetY);
	backgroundVertices.push_back(1.f); backgroundVertices.push_back(1.f);
	//Second triangle
	backgroundVertices.push_back(offsetX); backgroundVertices.push_back(offsetY);
	backgroundVertices.push_back(0.f); backgroundVertices.push_back(0.f);
	backgroundVertices.push_back(backgroundSize.x + offsetX); backgroundVertices.push_back(backgroundSize.y + offsetY);
	backgroundVertices.push_back(1.f); backgroundVertices.push_back(1.f);
	backgroundVertices.push_back(offsetX); backgroundVertices.push_back(backgroundSize.y + offsetY);
	backgroundVertices.push_back(0.f); backgroundVertices.push_back(1.f);

	glGenVertexArrays(1, &bVao);
	glBindVertexArray(bVao);
	glGenBuffers(1, &bVbo);
	glBindBuffer(GL_ARRAY_BUFFER, bVbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &backgroundVertices[0], GL_STATIC_DRAW);
	backPosLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	backTexCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}


// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

std::vector<int> colisionsEspecials = { 0, 120, 121, 122, 123, 124, 200, 201, 202, 203, 220, 221, 222, 223, 240, 241, 242, 243, 260, 261, 262, 263, 300, 301, 303, 305, 336, 337 };

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1, n;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	n = colisionsEspecials.size();

	for (int y = y0; y <= y1; y++)
	{
		bool collision = true;
		for (int i = 0; i < n; i++) if (colisionsEspecials[i] == map[y * mapSize.x + x]) collision = false;

		if (collision) return true;

		/*
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 120 && map[y * mapSize.x + x] != 121)
			return true;
		*/
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1, n;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	n = colisionsEspecials.size();

	for (int y = y0; y <= y1; y++)
	{
		bool collision = true;
		for (int i = 0; i < n; i++) if (colisionsEspecials[i] == map[y * mapSize.x + x]) collision = false;
		
		if (collision) return true;

		/*
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 120 && map[y * mapSize.x + x] != 121)
			return true;
		*/
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 10)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 361 || map[y * mapSize.x + x] == 348)
		{
			/*
			if (*posY - tileSize * y + size.y <= 10)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
			*/
			return true;
		}
	}

	return false;
}
