#include "SideTable.h"

SideTable::SideTable(float x, float y, float z, float rot) {
	rotation = rot;
	topCube = ImportedModel("cube.obj");
	leg0Cube = ImportedModel("cube.obj");
	leg1Cube = ImportedModel("cube.obj");
	leg2Cube = ImportedModel("cube.obj");
	leg3Cube = ImportedModel("cube.obj");
	topLoc = glm::vec3(x, y+0.5f, z);
	leg0Loc = glm::vec3(x-0.4f, y, z-0.4f);
	leg1Loc = glm::vec3(x - 0.4f, y, z + 0.4f);
	leg2Loc = glm::vec3(x + 0.4f, y, z - 0.4f);
	leg3Loc = glm::vec3(x + 0.4f, y, z + 0.4f);
	SideTable::init();
}

float SideTable::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void SideTable::init() {
	// setup top
	numTopVertices = topCube.getNumVertices();
	std::vector<glm::vec3> vert = topCube.getVertices();
	std::vector<glm::vec3> norm = topCube.getNormals();

	for (int i = 0; i < numTopVertices; i++) {
		topPvalues.push_back(vert[i].x);
		topPvalues.push_back(vert[i].y);
		topPvalues.push_back(vert[i].z);
		topNvalues.push_back(norm[i].x);
		topNvalues.push_back(norm[i].y);
		topNvalues.push_back(norm[i].z);
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

	//setup leg 2
	numLeg2Vertices = leg2Cube.getNumVertices();
	vert = leg2Cube.getVertices();
	norm = leg2Cube.getNormals();

	for (int i = 0; i < numLeg2Vertices; i++) {
		leg2Pvalues.push_back(vert[i].x);
		leg2Pvalues.push_back(vert[i].y);
		leg2Pvalues.push_back(vert[i].z);
		leg2Nvalues.push_back(norm[i].x);
		leg2Nvalues.push_back(norm[i].y);
		leg2Nvalues.push_back(norm[i].z);
	}

	//setup leg 3
	numLeg3Vertices = leg3Cube.getNumVertices();
	vert = leg3Cube.getVertices();
	norm = leg3Cube.getNormals();

	for (int i = 0; i < numLeg3Vertices; i++) {
		leg3Pvalues.push_back(vert[i].x);
		leg3Pvalues.push_back(vert[i].y);
		leg3Pvalues.push_back(vert[i].z);
		leg3Nvalues.push_back(norm[i].x);
		leg3Nvalues.push_back(norm[i].y);
		leg3Nvalues.push_back(norm[i].z);
	}
}

glm::mat4 SideTable::calculateTopMMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), topLoc);
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(0.5f, 0.1f, 0.5f));
	return mMat;
}

glm::mat4 SideTable::calculateLeg0MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getTopLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg0Loc - getTopLoc());
	mMat = glm::scale(mMat, glm::vec3(0.05f, 0.5f, 0.05f));
	return mMat;
}

glm::mat4 SideTable::calculateLeg1MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getTopLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg1Loc - getTopLoc());
	mMat = glm::scale(mMat, glm::vec3(0.05f, 0.5f, 0.05f));
	return mMat;
}

glm::mat4 SideTable::calculateLeg2MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getTopLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg2Loc - getTopLoc());
	mMat = glm::scale(mMat, glm::vec3(0.05f, 0.5f, 0.05f));
	return mMat;
}

glm::mat4 SideTable::calculateLeg3MMat() {
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), getTopLoc());
	mMat = glm::rotate(mMat, toRadians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::translate(mMat, leg3Loc - getTopLoc());
	mMat = glm::scale(mMat, glm::vec3(0.05f, 0.5f, 0.05f));
	return mMat;
}

ImportedModel SideTable::getTop() { return topCube; }
ImportedModel SideTable::getLeg0() { return leg0Cube; }
ImportedModel SideTable::getLeg1() { return leg1Cube; }
ImportedModel SideTable::getLeg2() { return leg2Cube; }
ImportedModel SideTable::getLeg3() { return leg3Cube; }
int SideTable::getNumTopVertices() { return numTopVertices; }
int SideTable::getNumLeg0Vertices() { return numLeg0Vertices; }
int SideTable::getNumLeg1Vertices() { return numLeg1Vertices; }
int SideTable::getNumLeg2Vertices() { return numLeg2Vertices; }
int SideTable::getNumLeg3Vertices() { return numLeg3Vertices; }
std::vector<float> SideTable::getTopPValues() { return topPvalues; }
std::vector<float> SideTable::getTopNValues() { return topNvalues; }
std::vector<float> SideTable::getLeg0PValues() { return leg0Pvalues; }
std::vector<float> SideTable::getLeg0NValues() { return leg0Nvalues; }
std::vector<float> SideTable::getLeg1PValues() { return leg1Pvalues; }
std::vector<float> SideTable::getLeg1NValues() { return leg1Nvalues; }
std::vector<float> SideTable::getLeg2PValues() { return leg2Pvalues; }
std::vector<float> SideTable::getLeg2NValues() { return leg2Nvalues; }
std::vector<float> SideTable::getLeg3PValues() { return leg3Pvalues; }
std::vector<float> SideTable::getLeg3NValues() { return leg3Nvalues; }
glm::vec3 SideTable::getTopLoc() { return topLoc; }
glm::vec3 SideTable::getLeg0Loc() { return leg0Loc; }
glm::vec3 SideTable::getLeg1Loc() { return leg1Loc; }
glm::vec3 SideTable::getLeg2Loc() { return leg2Loc; }
glm::vec3 SideTable::getLeg3Loc() { return leg3Loc; }