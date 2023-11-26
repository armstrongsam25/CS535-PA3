#pragma once

#include "ImportedModel.h"
#include <glm/gtc/matrix_transform.hpp>

#define FAN_ROTATION 120.0f

class Fan {
private:
	float rotation;
	ImportedModel baseCyl;
	ImportedModel stemCyl;
	ImportedModel motorSphere;
	ImportedModel blade0Sphere;
	ImportedModel blade1Sphere;
	ImportedModel blade2Sphere;
	int numBaseVertices;
	int numStemVertices;
	int numMotorVertices;
	int numBlade0Vertices;
	int numBlade1Vertices;
	int numBlade2Vertices;
	std::vector<float> basePvalues;
	std::vector<float> baseNvalues;
	std::vector<float> stemPvalues;
	std::vector<float> stemNvalues;
	std::vector<float> motorPvalues;
	std::vector<float> motorNvalues;
	std::vector<float> blade0Pvalues;
	std::vector<float> blade0Nvalues;
	std::vector<float> blade1Pvalues;
	std::vector<float> blade1Nvalues;
	std::vector<float> blade2Pvalues;
	std::vector<float> blade2Nvalues;
	glm::vec3 baseLoc;
	glm::vec3 stemLoc;
	glm::vec3 motorLoc;
	glm::vec3 blade0Loc;
	glm::vec3 blade1Loc;
	glm::vec3 blade2Loc;
	float toRadians(float degrees);
	void init();

public:
	Fan(float x, float y, float z, float rot);

	glm::mat4 calculateBaseMMat();
	glm::mat4 calculateStemMMat();
	glm::mat4 calculateMotorMMat();
	glm::mat4 calculateBlade0MMat(float step);
	glm::mat4 calculateBlade1MMat(float step);
	glm::mat4 calculateBlade2MMat(float step);

	ImportedModel getBase();
	ImportedModel getStem();
	ImportedModel getMotor();
	ImportedModel getBlade0();
	ImportedModel getBlade1();
	ImportedModel getBlade2();
	int getNumBaseVertices();
	int getNumStemVertices();
	int getNumMotorVertices();
	int getNumBlade0Vertices();
	int getNumBlade1Vertices();
	int getNumBlade2Vertices();
	std::vector<float> getBasePValues();
	std::vector<float> getBaseNValues();
	std::vector<float> getStemPValues();
	std::vector<float> getStemNValues();
	std::vector<float> getMotorPValues();
	std::vector<float> getMotorNValues();
	std::vector<float> getBlade0PValues();
	std::vector<float> getBlade0NValues();
	std::vector<float> getBlade1PValues();
	std::vector<float> getBlade1NValues();
	std::vector<float> getBlade2PValues();
	std::vector<float> getBlade2NValues();
	glm::vec3 getBaseLoc();
	glm::vec3 getStemLoc();
	glm::vec3 getMotorLoc();
	glm::vec3 getBlade0Loc();
	glm::vec3 getBlade1Loc();
	glm::vec3 getBlade2Loc();
};