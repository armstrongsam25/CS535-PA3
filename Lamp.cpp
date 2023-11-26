#include "Lamp.h"

Lamp::Lamp(float x, float y, float z) {
	baseCyl = ImportedModel("cylinder.obj");
	stemCyl = ImportedModel("cylinder.obj");
	shadeCone = ImportedModel("cone.obj");
	baseLoc = glm::vec3(x, y-2.0f, z);
	stemLoc = glm::vec3(x, y, z);
	shadeLoc = glm::vec3(x, y+2.0f, z);
	Lamp::init();
}

float Lamp::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Lamp::init() {
	// setup base
	numBaseVertices = baseCyl.getNumVertices();
	std::vector<glm::vec3> vert = baseCyl.getVertices();
	std::vector<glm::vec3> norm = baseCyl.getNormals();

	for (int i = 0; i < numBaseVertices; i++) {
		basePvalues.push_back(vert[i].x);
		basePvalues.push_back(vert[i].y);
		basePvalues.push_back(vert[i].z);
		baseNvalues.push_back(norm[i].x);
		baseNvalues.push_back(norm[i].y);
		baseNvalues.push_back(norm[i].z);
	}

	//setup stem
	numStemVertices = stemCyl.getNumVertices();
	vert = stemCyl.getVertices();
	norm = stemCyl.getNormals();

	for (int i = 0; i < numStemVertices; i++) {
		stemPvalues.push_back(vert[i].x);
		stemPvalues.push_back(vert[i].y);
		stemPvalues.push_back(vert[i].z);
		stemNvalues.push_back(norm[i].x);
		stemNvalues.push_back(norm[i].y);
		stemNvalues.push_back(norm[i].z);
	}

	//setup stem
	numShadeVertices = shadeCone.getNumVertices();
	vert = shadeCone.getVertices();
	norm = shadeCone.getNormals();

	for (int i = 0; i < numShadeVertices; i++) {
		shadePvalues.push_back(vert[i].x);
		shadePvalues.push_back(vert[i].y);
		shadePvalues.push_back(vert[i].z);
		shadeNvalues.push_back(norm[i].x);
		shadeNvalues.push_back(norm[i].y);
		shadeNvalues.push_back(norm[i].z);
	}
}

glm::mat4 Lamp::calculateBaseMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), baseLoc);
	mMat = glm::scale(mMat, glm::vec3(0.5f, 0.08f, 0.5f));
	return mMat;
}

glm::mat4 Lamp::calculateStemMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), stemLoc);
	mMat = glm::scale(mMat, glm::vec3(0.05f, 2.0f, 0.05f));
	return mMat;
}

glm::mat4 Lamp::calculateShadeMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), shadeLoc);
	mMat = glm::scale(mMat, glm::vec3(0.5f, 0.5f, 0.5f));
	return mMat;
}



ImportedModel Lamp::getBase() { return baseCyl; }
ImportedModel Lamp::getStem() { return stemCyl; }
ImportedModel Lamp::getShade() { return shadeCone; }
int Lamp::getNumBaseVertices() { return numBaseVertices; }
int Lamp::getNumStemVertices() { return numStemVertices; }
int Lamp::getNumShadeVertices() { return numShadeVertices; }
std::vector<float> Lamp::getBasePValues() { return basePvalues; }
std::vector<float> Lamp::getBaseNValues() { return baseNvalues; }
std::vector<float> Lamp::getStemPValues() { return stemPvalues; }
std::vector<float> Lamp::getStemNValues() { return stemNvalues; }
std::vector<float> Lamp::getShadePValues() { return shadePvalues; }
std::vector<float> Lamp::getShadeNValues() { return shadeNvalues; }
glm::vec3 Lamp::getBaseLoc() { return baseLoc; }
glm::vec3 Lamp::getStemLoc() { return stemLoc; }
glm::vec3 Lamp::getShadeLoc() { return shadeLoc; }