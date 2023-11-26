#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class SideTable {
private:
	float rotation;
	ImportedModel topCube;
	ImportedModel leg0Cube;
	ImportedModel leg1Cube;
	ImportedModel leg2Cube;
	ImportedModel leg3Cube;
	int numTopVertices;
	int numLeg0Vertices;
	int numLeg1Vertices;
	int numLeg2Vertices;
	int numLeg3Vertices;
	std::vector<float> topPvalues;
	std::vector<float> topNvalues;
	std::vector<float> leg0Pvalues;
	std::vector<float> leg0Nvalues;
	std::vector<float> leg1Pvalues;
	std::vector<float> leg1Nvalues;
	std::vector<float> leg2Pvalues;
	std::vector<float> leg2Nvalues;
	std::vector<float> leg3Pvalues;
	std::vector<float> leg3Nvalues;
	glm::vec3 topLoc;
	glm::vec3 leg0Loc;
	glm::vec3 leg1Loc;
	glm::vec3 leg2Loc;
	glm::vec3 leg3Loc;
	float toRadians(float degrees);
	void init();

public:
	SideTable(float x, float y, float z, float rot);

	glm::mat4 calculateTopMMat();
	glm::mat4 calculateLeg0MMat();
	glm::mat4 calculateLeg1MMat();
	glm::mat4 calculateLeg2MMat();
	glm::mat4 calculateLeg3MMat();
	ImportedModel getTop();
	ImportedModel getLeg0();
	ImportedModel getLeg1();
	ImportedModel getLeg2();
	ImportedModel getLeg3();
	int getNumTopVertices();
	int getNumLeg0Vertices();
	int getNumLeg1Vertices();
	int getNumLeg2Vertices();
	int getNumLeg3Vertices();
	std::vector<float> getTopPValues();
	std::vector<float> getTopNValues();
	std::vector<float> getLeg0PValues();
	std::vector<float> getLeg0NValues();
	std::vector<float> getLeg1PValues();
	std::vector<float> getLeg1NValues();
	std::vector<float> getLeg2PValues();
	std::vector<float> getLeg2NValues();
	std::vector<float> getLeg3PValues();
	std::vector<float> getLeg3NValues();
	glm::vec3 getTopLoc();
	glm::vec3 getLeg0Loc();
	glm::vec3 getLeg1Loc();
	glm::vec3 getLeg2Loc();
	glm::vec3 getLeg3Loc();
};