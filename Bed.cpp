#include "Bed.h"

Bed::Bed(float x, float y, float z, float rot) {
	rotation = rot;
	headboardCube = ImportedModel("cube.obj");
	leg0Cube = ImportedModel("cube.obj");
	leg1Cube = ImportedModel("cube.obj");
	frameCube = ImportedModel("cube.obj");
	matressCube = ImportedModel("cube.obj");
	headboardLoc = glm::vec3(x, y+0.5f, z-3.0f);
	leg0Loc = glm::vec3(x-1.75f, y-0.25, z+2.75f);
	leg1Loc = glm::vec3(x+1.75f, y-0.25, z+2.75f);
	frameLoc = glm::vec3(x, y, z);
	matressLoc = glm::vec3(x, y+0.3f, z);
	Bed::init();
}

float Bed::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Bed::init() {
	// setup headboard
	numHeadboardVertices = headboardCube.getNumVertices();
	std::vector<glm::vec3> vert = headboardCube.getVertices();
	std::vector<glm::vec3> norm = headboardCube.getNormals();

	for (int i = 0; i < numHeadboardVertices; i++) {
		headboardPvalues.push_back(vert[i].x);
		headboardPvalues.push_back(vert[i].y);
		headboardPvalues.push_back(vert[i].z);
		headboardNvalues.push_back(norm[i].x);
		headboardNvalues.push_back(norm[i].y);
		headboardNvalues.push_back(norm[i].z);
	}

	//setup leg 0
	numLeg0Vertices = leg0Cube.getNumVertices();
	vert = leg0Cube.getVertices();
	norm = leg0Cube.getNormals();

	for (int i = 0; i < numLeg0Vertices; i++) {
		leg0Pvalues.push_back(vert[i].x);
		leg0Pvalues.push_back(vert[i].y);
		leg0Pvalues.push_back(vert[i].z);
		leg0Nvalues.push_back(norm[i].x);
		leg0Nvalues.push_back(norm[i].y);
		leg0Nvalues.push_back(norm[i].z);
	}

	//setup leg 1
	numLeg1Vertices = leg1Cube.getNumVertices();
	vert = leg1Cube.getVertices();
	norm = leg1Cube.getNormals();

	for (int i = 0; i < numLeg1Vertices; i++) {
		leg1Pvalues.push_back(vert[i].x);
		leg1Pvalues.push_back(vert[i].y);
		leg1Pvalues.push_back(vert[i].z);
		leg1Nvalues.push_back(norm[i].x);
		leg1Nvalues.push_back(norm[i].y);
		leg1Nvalues.push_back(norm[i].z);
	}

	// setup frame
	numFrameVertices = frameCube.getNumVertices();
	vert = frameCube.getVertices();
	norm = frameCube.getNormals();

	for (int i = 0; i < numFrameVertices; i++) {
		framePvalues.push_back(vert[i].x);
		framePvalues.push_back(vert[i].y);
		framePvalues.push_back(vert[i].z);
		frameNvalues.push_back(norm[i].x);
		frameNvalues.push_back(norm[i].y);
		frameNvalues.push_back(norm[i].z);
	}

	// setup matress
	numMatressVertices = matressCube.getNumVertices();
	vert = matressCube.getVertices();
	norm = matressCube.getNormals();

	for (int i = 0; i < numMatressVertices; i++) {
		matressPvalues.push_back(vert[i].x);
		matressPvalues.push_back(vert[i].y);
		matressPvalues.push_back(vert[i].z);
		matressNvalues.push_back(norm[i].x);
		matressNvalues.push_back(norm[i].y);
		matressNvalues.push_back(norm[i].z);
	}
}

glm::mat4 Bed::calculateHeadboardMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, headboardLoc - getFrameLoc()); 
	mMat = glm::scale(mMat, glm::vec3(2.0f, 1.0f, 0.1f));
	return mMat;
}

glm::mat4 Bed::calculateLeg0MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg0Loc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(0.1f, 0.25f, 0.1f));
	return mMat;
}

glm::mat4 Bed::calculateLeg1MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg1Loc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(0.1f, 0.25f, 0.1f));
	return mMat;
}

glm::mat4 Bed::calculateFrameMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(2.0f, 0.25f, 3.0));
	return mMat;
}

glm::mat4 Bed::calculateMatressMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getFrameLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, matressLoc - getFrameLoc());
	mMat = glm::scale(mMat, glm::vec3(1.9f, 0.15f, 2.9f));
	return mMat;
}

ImportedModel Bed::getHeadboard() { return headboardCube; }
ImportedModel Bed::getLeg0() { return leg0Cube; }
ImportedModel Bed::getLeg1() { return leg1Cube; }
ImportedModel Bed::getFrame() { return frameCube; }
ImportedModel Bed::getMatress() { return matressCube; }
int Bed::getNumHeadboardVertices() { return numHeadboardVertices; }
int Bed::getNumLeg0Vertices() { return numLeg0Vertices; }
int Bed::getNumLeg1Vertices() { return numLeg1Vertices; }
int Bed::getNumFrameVertices() { return numFrameVertices; }
int Bed::getNumMatressVertices() { return numMatressVertices; } 
std::vector<float> Bed::getHeadboardPValues() { return headboardPvalues; }
std::vector<float> Bed::getHeadboardNValues() { return headboardNvalues; }
std::vector<float> Bed::getLeg0PValues() { return leg0Pvalues; }
std::vector<float> Bed::getLeg0NValues() { return leg0Nvalues; }
std::vector<float> Bed::getLeg1PValues() { return leg1Pvalues; }
std::vector<float> Bed::getLeg1NValues() { return leg1Nvalues; }
std::vector<float> Bed::getFramePValues() { return framePvalues; }
std::vector<float> Bed::getFrameNValues() { return frameNvalues; }
std::vector<float> Bed::getMatressPValues() { return matressPvalues; }
std::vector<float> Bed::getMatressNValues() { return matressNvalues; }
glm::vec3 Bed::getHeadboardLoc() { return headboardLoc; }
glm::vec3 Bed::getLeg0Loc() { return leg0Loc; }
glm::vec3 Bed::getLeg1Loc() { return leg1Loc; }
glm::vec3 Bed::getFrameLoc() { return frameLoc; }
glm::vec3 Bed::getMatressLoc() { return matressLoc; }