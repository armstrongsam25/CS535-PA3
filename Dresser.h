#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class Dresser {
private:
	float rotation;
	ImportedModel frameCube;
	ImportedModel drawer0Cube;
	ImportedModel drawer1Cube;
	ImportedModel drawer2Cube;
	int numFrameVertices;
	int numDrawer0Vertices;
	int numDrawer1Vertices;
	int numDrawer2Vertices;
	std::vector<float> framePvalues;
	std::vector<float> frameNvalues;
	std::vector<float> drawer0Pvalues;
	std::vector<float> drawer0Nvalues;
	std::vector<float> drawer1Pvalues;
	std::vector<float> drawer1Nvalues;
	std::vector<float> drawer2Pvalues;
	std::vector<float> drawer2Nvalues;
	glm::vec3 frameLoc;
	glm::vec3 drawer0Loc;
	glm::vec3 drawer1Loc;
	glm::vec3 drawer2Loc;
	float toRadians(float degrees);
	void init();

public:
	Dresser(float x, float y, float z, float rot);

	glm::mat4 calculateFrameMMat();
	glm::mat4 calculateDrawer0MMat();
	glm::mat4 calculateDrawer1MMat();
	glm::mat4 calculateDrawer2MMat();

	ImportedModel getFrame();
	ImportedModel getDrawer0();
	ImportedModel getDrawer1();
	ImportedModel getDrawer2();
	int getNumFrameVertices();
	int getNumDrawer0Vertices();
	int getNumDrawer1Vertices();
	int getNumDrawer2Vertices();
	std::vector<float> getFramePValues();
	std::vector<float> getFrameNValues();
	std::vector<float> getDrawer0PValues();
	std::vector<float> getDrawer0NValues();
	std::vector<float> getDrawer1PValues();
	std::vector<float> getDrawer1NValues();
	std::vector<float> getDrawer2PValues();
	std::vector<float> getDrawer2NValues();
	glm::vec3 getFrameLoc();
	glm::vec3 getDrawer0Loc();
	glm::vec3 getDrawer1Loc();
	glm::vec3 getDrawer2Loc();
};