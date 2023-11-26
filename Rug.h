#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

class Rug {
private:
	float rotation;
	ImportedModel rugCyl;
	int numRugVertices;
	std::vector<float> rugPvalues;
	std::vector<float> rugNvalues;
	glm::vec3 rugLoc;
	float toRadians(float degrees);
	void init();

public:
	Rug(float x, float y, float z, float rot);

	glm::mat4 calculateRugMMat();

	ImportedModel getRug();
	int getNumRugVertices();
	std::vector<float> getRugPValues();
	std::vector<float> getRugNValues();
	glm::vec3 getRugLoc();
};