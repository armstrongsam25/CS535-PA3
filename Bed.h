#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class Bed {
private:
	float rotation;
	ImportedModel headboardCube;
	ImportedModel leg0Cube;
	ImportedModel leg1Cube;
	ImportedModel frameCube;
	ImportedModel matressCube;
	int numHeadboardVertices;
	int numLeg0Vertices;
	int numLeg1Vertices;
	int numFrameVertices;
	int numMatressVertices;
	std::vector<float> headboardPvalues;
	std::vector<float> headboardNvalues;
	std::vector<float> leg0Pvalues;
	std::vector<float> leg0Nvalues;
	std::vector<float> leg1Pvalues;
	std::vector<float> leg1Nvalues;
	std::vector<float> framePvalues;
	std::vector<float> frameNvalues;
	std::vector<float> matressPvalues;
	std::vector<float> matressNvalues;
	glm::vec3 headboardLoc;
	glm::vec3 leg0Loc;
	glm::vec3 leg1Loc;
	glm::vec3 frameLoc;
	glm::vec3 matressLoc;
	float toRadians(float degrees);
	void init();

public:
	Bed(float x, float y, float z, float rot);

	glm::mat4 calculateHeadboardMMat();
	glm::mat4 calculateLeg0MMat();
	glm::mat4 calculateLeg1MMat();
	glm::mat4 calculateFrameMMat();
	glm::mat4 calculateMatressMMat();

	ImportedModel getHeadboard();
	ImportedModel getLeg0();
	ImportedModel getLeg1();
	ImportedModel getFrame();
	ImportedModel getMatress();
	int getNumHeadboardVertices();
	int getNumLeg0Vertices();
	int getNumLeg1Vertices();
	int getNumFrameVertices();
	int getNumMatressVertices();
	std::vector<float> getHeadboardPValues();
	std::vector<float> getHeadboardNValues();
	std::vector<float> getLeg0PValues();
	std::vector<float> getLeg0NValues();
	std::vector<float> getLeg1PValues();
	std::vector<float> getLeg1NValues();
	std::vector<float> getFramePValues();
	std::vector<float> getFrameNValues();
	std::vector<float> getMatressPValues();
	std::vector<float> getMatressNValues();
	glm::vec3 getHeadboardLoc();
	glm::vec3 getLeg0Loc();
	glm::vec3 getLeg1Loc();
	glm::vec3 getFrameLoc();
	glm::vec3 getMatressLoc();
};