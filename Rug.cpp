#include "Rug.h"

Rug::Rug(float x, float y, float z, float rot) {
	rotation = rot;
	rugCyl = ImportedModel("cylinder.obj");
	rugLoc = glm::vec3(x, y, z);
	Rug::init();
}

float Rug::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Rug::init() {
	// setup rug
	numRugVertices = rugCyl.getNumVertices();
	std::vector<glm::vec3> vert = rugCyl.getVertices();
	std::vector<glm::vec3> norm = rugCyl.getNormals();

	for (int i = 0; i < numRugVertices; i++) {
		rugPvalues.push_back(vert[i].x);
		rugPvalues.push_back(vert[i].y);
		rugPvalues.push_back(vert[i].z);
		rugNvalues.push_back(norm[i].x);
		rugNvalues.push_back(norm[i].y);
		rugNvalues.push_back(norm[i].z);
	}
}

glm::mat4 Rug::calculateRugMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), rugLoc);
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(2.0f, 0.05f, 2.0f));
	return mMat;
}

ImportedModel Rug::getRug() { return rugCyl; }
int Rug::getNumRugVertices() { return numRugVertices; }
std::vector<float> Rug::getRugPValues() { return rugPvalues; }
std::vector<float> Rug::getRugNValues() { return rugNvalues; }
glm::vec3 Rug::getRugLoc() { return rugLoc; }
