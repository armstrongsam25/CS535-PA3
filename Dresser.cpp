#include "Dresser.h"

Dresser::Dresser(float x, float y, float z, float rot) {
	rotation = rot;
	frameCube = ImportedModel("cube.obj");
	drawer0Cube = ImportedModel("cube.obj");
	drawer1Cube = ImportedModel("cube.obj");
	drawer2Cube = ImportedModel("cube.obj");
	frameLoc = glm::vec3(x, y, z);
	drawer0Loc = glm::vec3(x, y+0.9f, z+0.4f);
	drawer1Loc = glm::vec3(x, y, z+0.4f);
	drawer2Loc = glm::vec3(x, y-0.9f, z + 0.4f);
	Dresser::init();
}

float Dresser::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Dresser::init() {
	// setup frame
	numFrameVertices = frameCube.getNumVertices();
	std::vector<glm::vec3> vert = frameCube.getVertices();
	std::vector<glm::vec3> norm = frameCube.getNormals();

	for (int i = 0; i < numFrameVertices; i++) {
		framePvalues.push_back(vert[i].x);
		framePvalues.push_back(vert[i].y);
		framePvalues.push_back(vert[i].z);
		frameNvalues.push_back(norm[i].x);
		frameNvalues.push_back(norm[i].y);
		frameNvalues.push_back(norm[i].z);
	}

	//setup drawer 0
	numDrawer0Vertices = drawer0Cube.getNumVertices();
	vert = drawer0Cube.getVertices();
	norm = drawer0Cube.getNormals();

	for (int i = 0; i < numDrawer0Vertices; i++) {
		drawer0Pvalues.push_back(vert[i].x);
		drawer0Pvalues.push_back(vert[i].y);
		drawer0Pvalues.push_back(vert[i].z);
		drawer0Nvalues.push_back(norm[i].x);
		drawer0Nvalues.push_back(norm[i].y);
		drawer0Nvalues.push_back(norm[i].z);
	}

	//setup drawer 1
	numDrawer1Vertices = drawer1Cube.getNumVertices();
	vert = drawer1Cube.getVertices();
	norm = drawer1Cube.getNormals();

	for (int i = 0; i < numDrawer1Vertices; i++) {
		drawer1Pvalues.push_back(vert[i].x);
		drawer1Pvalues.push_back(vert[i].y);
		drawer1Pvalues.push_back(vert[i].z);
		drawer1Nvalues.push_back(norm[i].x);
		drawer1Nvalues.push_back(norm[i].y);
		drawer1Nvalues.push_back(norm[i].z);
	}

	//setup drawer 2
	numDrawer2Vertices = drawer2Cube.getNumVertices();
	vert = drawer2Cube.getVertices();
	norm = drawer2Cube.getNormals();

	for (int i = 0; i < numDrawer2Vertices; i++) {
		drawer2Pvalues.push_back(vert[i].x);
		drawer2Pvalues.push_back(vert[i].y);
		drawer2Pvalues.push_back(vert[i].z);
		drawer2Nvalues.push_back(norm[i].x);
		drawer2Nvalues.push_back(norm[i].y);
		drawer2Nvalues.push_back(norm[i].z);
	}
}

glm::mat4 Dresser::calculateFrameMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(1.5f, 1.5f, 0.5f));
	return mMat;
}

glm::mat4 Dresser::calculateDrawer0MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, drawer0Loc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(1.4f, 0.4f, 0.2f));
	return mMat;
}

glm::mat4 Dresser::calculateDrawer1MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, drawer1Loc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(1.4f, 0.4f, 0.2f));
	return mMat;
}

glm::mat4 Dresser::calculateDrawer2MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, drawer2Loc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(1.4f, 0.4f, 0.2f));
	return mMat;
}

ImportedModel Dresser::getFrame() { return frameCube; }
ImportedModel Dresser::getDrawer0() { return drawer0Cube; }
ImportedModel Dresser::getDrawer1() { return drawer1Cube; }
ImportedModel Dresser::getDrawer2() { return drawer2Cube; }
int Dresser::getNumFrameVertices() { return numFrameVertices; }
int Dresser::getNumDrawer0Vertices() { return numDrawer0Vertices; }
int Dresser::getNumDrawer1Vertices() { return numDrawer1Vertices; }
int Dresser::getNumDrawer2Vertices() { return numDrawer2Vertices; }
std::vector<float> Dresser::getFramePValues() { return framePvalues; }
std::vector<float> Dresser::getFrameNValues() { return frameNvalues; }
std::vector<float> Dresser::getDrawer0PValues() { return drawer0Pvalues; }
std::vector<float> Dresser::getDrawer0NValues() { return drawer0Nvalues; }
std::vector<float> Dresser::getDrawer1PValues() { return drawer1Pvalues; }
std::vector<float> Dresser::getDrawer1NValues() { return drawer1Nvalues; }
std::vector<float> Dresser::getDrawer2PValues() { return drawer2Pvalues; }
std::vector<float> Dresser::getDrawer2NValues() { return drawer2Nvalues; }
glm::vec3 Dresser::getFrameLoc() { return frameLoc; }
glm::vec3 Dresser::getDrawer0Loc() { return drawer0Loc; }
glm::vec3 Dresser::getDrawer1Loc() { return drawer1Loc; }
glm::vec3 Dresser::getDrawer2Loc() { return drawer2Loc; }