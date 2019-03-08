#ifndef MINOTAUR_H
#define MINOTAUR_H

#include "Transform.h"
#include "Model.h"
#include "Sound.h"
#include "Maze.h"
#include <vector>

class Minotaur
{
public:
	Minotaur(irrklang::ISoundEngine * engine, std::vector<std::vector<int>> mazeGrid, Maze* maze);
	virtual ~Minotaur();
	Transform GetTransform();
	void increaseAgressionLevel();
	void reactToSound(glm::vec3 soundPos);

	void Update(glm::vec3 playerPos);
	void Draw(Shader* shader);
	glm::vec2 ClampToEdges(glm::vec2 mazeCoords);	// takes in maze coords and spits out mazecoords

	// TEST
	void setupColorData();
	void drawPath();

private:
	Model model;
	Sound stepSound;
	Sound growlSound;
	Transform transform;
	float movementSpeed;
	int alerted = 0;
	glm::vec3 lastSoundHeardPos;
	int searchArea;
	glm::vec2 destination;
	Maze* maze;
	std::vector<glm::vec2> generatedPath;
	std::vector<std::vector<int>> mazeGrid;

	struct distance {
		int y;
		int x;
	};

	void GeneratePath(int startY, int startX, int destinationY, int destinationX);
	void Move();
	glm::vec2 toNearbyFloor(glm::vec2 mazePos);

	// TEST
	// color data for image
	std::vector<std::vector<std::vector<unsigned char>>> image;
};

#endif