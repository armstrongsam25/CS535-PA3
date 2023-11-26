#include "WallsFloor.h"

WallsFloor::WallsFloor(float x, float y, float z) {
	floorCube = ImportedModel("cube.obj");
	leftWallCube = ImportedModel("cube.obj");
	rightWallCube = ImportedModel("cube.obj");
	floorLoc = glm::vec3(x+2.0f, y-4.0f, z);
	lWallLoc = glm::vec3(x-4.0f, y, z);
	rWallLoc = glm::vec3(x+2.0f, y, z-4.0f);
	WallsFloor::init();
}

float WallsFloor::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void WallsFloor::init() {
	// setup base
	numFloorVertices = floorCube.getNumVertices();
	std::vector<glm::vec3> vert = floorCube.getVertices();
	std::vector<glm::vec3> norm = floorCube.getNormals();

	for (int i = 0; i < numFloorVertices; i++) {
		floorPvalues.push_back(vert[i].x);
		floorPvalues.push_back(vert[i].y);
		floorPvalues.push_back(vert[i].z);
		floorNvalues.push_back(norm[i].x);
		floorNvalues.push_back(norm[i].y);
		floorNvalues.push_back(norm[i].z);
	}

	//setup stem
	numLWallVertices = leftWallCube.getNumVertices();
	vert = leftWallCube.getVertices();
	norm = leftWallCube.getNormals();

	for (int i = 0; i < numLWallVertices; i++) {
		lWallPvalues.push_back(vert[i].x);
		lWallPvalues.push_back(vert[i].y);
		lWallPvalues.push_back(vert[i].z);
		lWallNvalues.push_back(norm[i].x);
		lWallNvalues.push_back(norm[i].y);
		lWallNvalues.push_back(norm[i].z);
	}

	//setup stem
	numRWallVertices = rightWallCube.getNumVertices();
	vert = rightWallCube.getVertices();
	norm = rightWallCube.getNormals();

	for (int i = 0; i < numRWallVertices; i++) {
		rWallPvalues.push_back(vert[i].x);
		rWallPvalues.push_back(vert[i].y);
		rWallPvalues.push_back(vert[i].z);
		rWallNvalues.push_back(norm[i].x);
		rWallNvalues.push_back(norm[i].y);
		rWallNvalues.push_back(norm[i].z);
	}
}

glm::mat4 WallsFloor::calculateFloorMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), floorLoc);
	mMat = glm::scale(mMat, glm::vec3(6.0f, 0.1f, 4.0f));
	return mMat;
}

glm::mat4 WallsFloor::calculateLWallMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), lWallLoc);
	mMat = glm::scale(mMat, glm::vec3(0.1f, 4.0f, 4.0f));
	return mMat;
}

glm::mat4 WallsFloor::calculateRWallMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), rWallLoc);
	mMat = glm::scale(mMat, glm::vec3(6.0f, 4.0f, 0.1f));
	return mMat;
}



ImportedModel WallsFloor::getFloor() { return floorCube; }
ImportedModel WallsFloor::getLWall() { return leftWallCube; }
ImportedModel WallsFloor::getRWall() { return rightWallCube; }
int WallsFloor::getNumFloorVertices() { return numFloorVertices; }
int WallsFloor::getNumLWallVertices() { return numLWallVertices; }
int WallsFloor::getNumRWallVertices() { return numRWallVertices; }
std::vector<float> WallsFloor::getFloorPValues() { return floorPvalues; }
std::vector<float> WallsFloor::getFloorNValues() { return floorNvalues; }
std::vector<float> WallsFloor::getLWallPValues() { return lWallPvalues; }
std::vector<float> WallsFloor::getLWallNValues() { return lWallNvalues; }
std::vector<float> WallsFloor::getRWallPValues() { return rWallPvalues; }
std::vector<float> WallsFloor::getRWallNValues() { return rWallNvalues; }
glm::vec3 WallsFloor::getFloorLoc() { return floorLoc; }
glm::vec3 WallsFloor::getLWallLoc() { return lWallLoc; }
glm::vec3 WallsFloor::getRWallLoc() { return rWallLoc; }