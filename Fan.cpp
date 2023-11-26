#include "Fan.h"

Fan::Fan(float x, float y, float z, float rot) {
	rotation = rot;
	baseCyl = ImportedModel("cylinder.obj");
	stemCyl = ImportedModel("cylinder.obj");
	motorSphere = ImportedModel("sphere.obj");
	blade0Sphere = ImportedModel("sphere.obj");
	blade1Sphere = ImportedModel("sphere.obj");
	blade2Sphere = ImportedModel("sphere.obj");
	baseLoc = glm::vec3(x, y-1.0f, z);
	stemLoc = glm::vec3(x, y, z);
	motorLoc = glm::vec3(x, y+1.0f, z);
	blade0Loc = glm::vec3(x+0.35f, y+1.0f, z + 0.15f);
	blade1Loc = glm::vec3(x+0.35f, y+1.0f, z + 0.15f);
	blade2Loc = glm::vec3(x+0.35f, y+1.0f, z + 0.15f);
	Fan::init();
}

float Fan::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Fan::init() {
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

	//setup motor
	numMotorVertices = motorSphere.getNumVertices();
	vert = motorSphere.getVertices();
	norm = motorSphere.getNormals();

	for (int i = 0; i < numMotorVertices; i++) {
		motorPvalues.push_back(vert[i].x);
		motorPvalues.push_back(vert[i].y);
		motorPvalues.push_back(vert[i].z);
		motorNvalues.push_back(norm[i].x);
		motorNvalues.push_back(norm[i].y);
		motorNvalues.push_back(norm[i].z);
	}

	// setup blade 0
	numBlade0Vertices = blade0Sphere.getNumVertices();
	vert = blade0Sphere.getVertices();
	norm = blade0Sphere.getNormals();

	for (int i = 0; i < numBlade0Vertices; i++) {
		blade0Pvalues.push_back(vert[i].x);
		blade0Pvalues.push_back(vert[i].y);
		blade0Pvalues.push_back(vert[i].z);
		blade0Nvalues.push_back(norm[i].x);
		blade0Nvalues.push_back(norm[i].y);
		blade0Nvalues.push_back(norm[i].z);
	}

	// setup blade 1
	numBlade1Vertices = blade1Sphere.getNumVertices();
	vert = blade1Sphere.getVertices();
	norm = blade1Sphere.getNormals();

	for (int i = 0; i < numBlade1Vertices; i++) {
		blade1Pvalues.push_back(vert[i].x);
		blade1Pvalues.push_back(vert[i].y);
		blade1Pvalues.push_back(vert[i].z);
		blade1Nvalues.push_back(norm[i].x);
		blade1Nvalues.push_back(norm[i].y);
		blade1Nvalues.push_back(norm[i].z);
	}

	// setup blade 2
	numBlade2Vertices = blade2Sphere.getNumVertices();
	vert = blade2Sphere.getVertices();
	norm = blade2Sphere.getNormals();

	for (int i = 0; i < numBlade2Vertices; i++) {
		blade2Pvalues.push_back(vert[i].x);
		blade2Pvalues.push_back(vert[i].y);
		blade2Pvalues.push_back(vert[i].z);
		blade2Nvalues.push_back(norm[i].x);
		blade2Nvalues.push_back(norm[i].y);
		blade2Nvalues.push_back(norm[i].z);
	}
}

glm::mat4 Fan::calculateBaseMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), baseLoc);
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(0.25f, 0.04f, 0.25f));
	return mMat;
}

glm::mat4 Fan::calculateStemMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), stemLoc);
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(0.02f, 1.0f, 0.02f));
	return mMat;
}

glm::mat4 Fan::calculateMotorMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), motorLoc);
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(0.1f, 0.1f, 0.2f));
	return mMat;
}

glm::mat4 Fan::calculateBlade0MMat(float step) {
	float newRot = 0.0f + step;
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getMotorLoc()); // Translate to the fan motor position
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(newRot), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the fan motor's z-axis
	mMat = glm::translate(mMat, blade0Loc - getMotorLoc()); // Translate back to the blade's position relative to the motor
	mMat = glm::scale(mMat, glm::vec3(0.3f, 0.1f, 0.005f));
	return mMat;
}

glm::mat4 Fan::calculateBlade1MMat(float step) {
	float newRot = 120.0f + step;
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getMotorLoc()); // Translate to the fan motor position
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(newRot), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the fan motor's z-axis
	mMat = glm::translate(mMat, blade1Loc - getMotorLoc()); // Translate back to the blade's position relative to the motor
	mMat = glm::scale(mMat, glm::vec3(0.3f, 0.1f, 0.005f));
	return mMat;
}

glm::mat4 Fan::calculateBlade2MMat(float step) {
	float newRot = 240.0f + step;
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getMotorLoc()); // Translate to the fan motor position
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(newRot), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around the fan motor's z-axis
	mMat = glm::translate(mMat, blade2Loc - getMotorLoc()); // Translate back to the blade's position relative to the motor
	mMat = glm::scale(mMat, glm::vec3(0.3f, 0.1f, 0.005f));
	return mMat;
}



ImportedModel Fan::getBase() { return baseCyl; }
ImportedModel Fan::getStem() { return stemCyl; }
ImportedModel Fan::getMotor() { return motorSphere; }
ImportedModel Fan::getBlade0() { return blade0Sphere; }
ImportedModel Fan::getBlade1() { return blade1Sphere; }
ImportedModel Fan::getBlade2() { return blade2Sphere; }
int Fan::getNumBaseVertices() { return numBaseVertices; }
int Fan::getNumStemVertices() { return numStemVertices; }
int Fan::getNumMotorVertices() { return numMotorVertices; }
int Fan::getNumBlade0Vertices() { return numBlade0Vertices; }
int Fan::getNumBlade1Vertices() { return numBlade1Vertices; }
int Fan::getNumBlade2Vertices() { return numBlade2Vertices; }
std::vector<float> Fan::getBasePValues() { return basePvalues; }
std::vector<float> Fan::getBaseNValues() { return baseNvalues; }
std::vector<float> Fan::getStemPValues() { return stemPvalues; }
std::vector<float> Fan::getStemNValues() { return stemNvalues; }
std::vector<float> Fan::getMotorPValues() { return motorPvalues; }
std::vector<float> Fan::getMotorNValues() { return motorNvalues; }
std::vector<float> Fan::getBlade0PValues() { return blade0Pvalues; }
std::vector<float> Fan::getBlade0NValues() { return blade0Nvalues; }
std::vector<float> Fan::getBlade1PValues() { return blade1Pvalues; }
std::vector<float> Fan::getBlade1NValues() { return blade1Nvalues; }
std::vector<float> Fan::getBlade2PValues() { return blade2Pvalues; }
std::vector<float> Fan::getBlade2NValues() { return blade2Nvalues; }
glm::vec3 Fan::getBaseLoc() { return baseLoc; }
glm::vec3 Fan::getStemLoc() { return stemLoc; }
glm::vec3 Fan::getMotorLoc() { return motorLoc; }
glm::vec3 Fan::getBlade0Loc() { return blade0Loc; }
glm::vec3 Fan::getBlade1Loc() { return blade1Loc; }
glm::vec3 Fan::getBlade2Loc() { return blade2Loc; }