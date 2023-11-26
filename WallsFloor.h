#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class WallsFloor {
private:
	ImportedModel floorCube;
	ImportedModel leftWallCube;
	ImportedModel rightWallCube;
	int numFloorVertices;
	int numLWallVertices;
	int numRWallVertices;
	std::vector<float> floorPvalues;
	std::vector<float> floorNvalues;
	std::vector<float> lWallPvalues;
	std::vector<float> lWallNvalues;
	std::vector<float> rWallPvalues;
	std::vector<float> rWallNvalues;
	glm::vec3 floorLoc;
	glm::vec3 lWallLoc;
	glm::vec3 rWallLoc;
	float toRadians(float degrees);
	void init();

public:
	WallsFloor(float x, float y, float z);

	glm::mat4 calculateFloorMMat();
	glm::mat4 calculateLWallMMat();
	glm::mat4 calculateRWallMMat();

	ImportedModel getFloor();
	ImportedModel getLWall();
	ImportedModel getRWall();
	int getNumFloorVertices();
	int getNumLWallVertices();
	int getNumRWallVertices();
	std::vector<float> getFloorPValues();
	std::vector<float> getFloorNValues();
	std::vector<float> getLWallPValues();
	std::vector<float> getLWallNValues();
	std::vector<float> getRWallPValues();
	std::vector<float> getRWallNValues();
	glm::vec3 getFloorLoc();
	glm::vec3 getLWallLoc();
	glm::vec3 getRWallLoc();
};
