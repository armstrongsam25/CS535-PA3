#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class Lamp {
private:
	ImportedModel baseCyl;
	ImportedModel stemCyl;
	ImportedModel shadeCone;
	int numBaseVertices;
	int numStemVertices;
	int numShadeVertices;
	std::vector<float> basePvalues;
	std::vector<float> baseNvalues;
	std::vector<float> stemPvalues;
	std::vector<float> stemNvalues;
	std::vector<float> shadePvalues;
	std::vector<float> shadeNvalues;
	glm::vec3 baseLoc;
	glm::vec3 stemLoc;
	glm::vec3 shadeLoc;
	float toRadians(float degrees);
	void init();

public:
	Lamp(float x, float y, float z);

	glm::mat4 calculateBaseMMat();
	glm::mat4 calculateStemMMat();
	glm::mat4 calculateShadeMMat();
	
	ImportedModel getBase();
	ImportedModel getStem();
	ImportedModel getShade();
	int getNumBaseVertices();
	int getNumStemVertices();
	int getNumShadeVertices();
	std::vector<float> getBasePValues();
	std::vector<float> getBaseNValues();
	std::vector<float> getStemPValues();
	std::vector<float> getStemNValues();
	std::vector<float> getShadePValues();
	std::vector<float> getShadeNValues();
	glm::vec3 getBaseLoc();
	glm::vec3 getStemLoc();
	glm::vec3 getShadeLoc();
};