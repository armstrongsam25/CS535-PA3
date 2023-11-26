///////////////////////////////////////////////////////////////////////////////
//
// Programming Assignment 2
// Samuel Armstrong
//
///////////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Torus.h"
#include "Utils.h"
#include "WallsFloor.h"
#include "Fan.h"
#include "Lamp.h"
#include "Bed.h"
#include "Dresser.h"
#include "Rug.h"
#include "SideTable.h"
#include "ImportedModel.h"
using namespace std;

void passOne(void);
void passTwo(void);

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 54

GLuint renderingProgram1, renderingProgram2;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// set positions of objects
Lamp lamp(3.0f, -1.8f, -3.0f);
WallsFloor wallFloor(0.0f, 0.0f, 0.0f);
Fan fan(-3.0f, -2.9f, 3.5f, 120.0f);// x, y, z, rotation
Bed bed(-0.75f, -3.5f, -1.75f, 90.0f); // x, y, z, rotation
Dresser dresser(6.0f, -2.5f, -3.4f, 0.0f); // x, y, z, rotation
Rug rug(4.5f, -3.9f, 1.0f, 0.0f); // x, y, z, rotation
SideTable sideTable(-3.0f, -3.5f, 1.5f, 0.0f); // x, y, z, rotation

float fanBladeRotation = 0.0f;

//set camera and 2 light locations
glm::vec3 cameraLoc(2.0f, 0.0f, 15.0f);
float currRotationX = 0.0f;
float currRotationY = 0.0f;
glm::vec3 lightLoc(3.0f, 1.0f, -3.0f);
glm::vec3 light1Loc(2.5f, 30.0f, 0.0f);

// white light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* gMatAmb = Utils::goldAmbient();
float* gMatDif = Utils::goldDiffuse();
float* gMatSpe = Utils::goldSpecular();
float gMatShi = Utils::goldShininess();

// bronze material
float* bMatAmb = Utils::bronzeAmbient();
float* bMatDif = Utils::bronzeDiffuse();
float* bMatSpe = Utils::bronzeSpecular();
float bMatShi = Utils::bronzeShininess();

// brown material
float* brownMatAmb = Utils::brownAmbient();
float* brownMatDif = Utils::brownDiffuse();
float* brownMatSpe = Utils::brownSpecular();
float brownMatShi = Utils::brownShininess();

// silver material
float* sMatAmb = Utils::silverAmbient();
float* sMatDif = Utils::silverDiffuse();
float* sMatSpe = Utils::silverSpecular();
float sMatShi = Utils::silverShininess();

// blue material
float* blueMatAmb = Utils::blueAmbient();
float* blueMatDif = Utils::blueDiffuse();
float* blueMatSpe = Utils::blueSpecular();
float blueMatShi = Utils::blueShininess();

// black material
float* blackMatAmb = Utils::blackAmbient();
float* blackMatDif = Utils::blackDiffuse();
float* blackMatSpe = Utils::blackSpecular();
float blackMatShi = Utils::blackShininess();

// yellow material
float* yellowMatAmb = Utils::yellowAmbient();
float* yellowMatDif = Utils::yellowDiffuse();
float* yellowMatSpe = Utils::yellowSpecular();
float yellowMatShi = Utils::yellowShininess();

// white material
float* whiteMatAmb = Utils::whiteAmbient();
float* whiteMatDif = Utils::whiteDiffuse();
float* whiteMatSpe = Utils::whiteSpecular();
float whiteMatShi = Utils::whiteShininess();

// red material
float* redMatAmb = Utils::redAmbient();
float* redMatDif = Utils::redDiffuse();
float* redMatSpe = Utils::redSpecular();
float redMatShi = Utils::redShininess();

// gray material
float* grayMatAmb = Utils::grayAmbient();
float* grayMatDif = Utils::grayDiffuse();
float* grayMatSpe = Utils::graySpecular();
float grayMatShi = Utils::grayShininess();

// green material
float* greenMatAmb = Utils::greenAmbient();
float* greenMatDif = Utils::greenDiffuse();
float* greenMatSpe = Utils::greenSpecular();
float greenMatShi = Utils::greenShininess();

float thisAmb[4], thisDif[4], thisSpe[4], matAmb[4], matDif[4], matSpe[4];
float thisShi, matShi;

// shadow stuff
int scSizeX, scSizeY;
GLuint shadowTex, shadowBuffer;
glm::mat4 lightVmatrix;
glm::mat4 lightPmatrix;
glm::mat4 shadowMVP1;
glm::mat4 shadowMVP2;
glm::mat4 b;

// variable allocation for display
GLuint mvLoc, projLoc, nLoc, sLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, currentLight1Pos, transformed, transformed1;
float lightPos[3], light1Pos[3];
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, 
		posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc,
		amb1Loc, diff1Loc, spec1Loc, pos1Loc;
glm::vec3 origin(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

// installs/updates the lights in the scene
void installLights(int renderingProgram, glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	transformed1 = glm::vec3(vMatrix * glm::vec4(currentLight1Pos, 1.0));
	light1Pos[0] = transformed1.x;
	light1Pos[1] = transformed1.y;
	light1Pos[2] = transformed1.z;

	matAmb[0] = thisAmb[0]; matAmb[1] = thisAmb[1]; matAmb[2] = thisAmb[2]; matAmb[3] = thisAmb[3];
	matDif[0] = thisDif[0]; matDif[1] = thisDif[1]; matDif[2] = thisDif[2]; matDif[3] = thisDif[3];
	matSpe[0] = thisSpe[0]; matSpe[1] = thisSpe[1]; matSpe[2] = thisSpe[2]; matSpe[3] = thisSpe[3];
	matShi = thisShi;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	amb1Loc = glGetUniformLocation(renderingProgram, "light1.ambient");
	diff1Loc = glGetUniformLocation(renderingProgram, "light1.diffuse");
	spec1Loc = glGetUniformLocation(renderingProgram, "light1.specular");
	pos1Loc = glGetUniformLocation(renderingProgram, "light1.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, amb1Loc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diff1Loc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, spec1Loc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, pos1Loc, 1, light1Pos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

// ----- setting up all object VBOs -----
void setupLampVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getBasePValues().size() * 4, &lamp.getBasePValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getBaseNValues().size() * 4, &lamp.getBaseNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getStemPValues().size() * 4, &lamp.getStemPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getStemNValues().size() * 4, &lamp.getStemNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getShadePValues().size() * 4, &lamp.getShadePValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, lamp.getShadeNValues().size() * 4, &lamp.getShadeNValues()[0], GL_STATIC_DRAW);
}

void setupWallFloorVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getFloorPValues().size() * 4, &wallFloor.getFloorPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getFloorNValues().size() * 4, &wallFloor.getFloorNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getLWallPValues().size() * 4, &wallFloor.getLWallPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getLWallNValues().size() * 4, &wallFloor.getLWallNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getRWallPValues().size() * 4, &wallFloor.getRWallPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, wallFloor.getRWallNValues().size() * 4, &wallFloor.getRWallNValues()[0], GL_STATIC_DRAW);
}

void setupFanVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBasePValues().size() * 4, &fan.getBasePValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBaseNValues().size() * 4, &fan.getBaseNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, fan.getStemPValues().size() * 4, &fan.getStemPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, fan.getStemNValues().size() * 4, &fan.getStemNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
	glBufferData(GL_ARRAY_BUFFER, fan.getMotorPValues().size() * 4, &fan.getMotorPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, fan.getMotorNValues().size() * 4, &fan.getMotorNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[18]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade0PValues().size() * 4, &fan.getBlade0PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[19]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade0NValues().size() * 4, &fan.getBlade0NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[20]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade1PValues().size() * 4, &fan.getBlade1PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[21]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade1NValues().size() * 4, &fan.getBlade1NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[22]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade2PValues().size() * 4, &fan.getBlade2PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[23]);
	glBufferData(GL_ARRAY_BUFFER, fan.getBlade2NValues().size() * 4, &fan.getBlade2NValues()[0], GL_STATIC_DRAW);
}

void setupBedVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[24]);
	glBufferData(GL_ARRAY_BUFFER, bed.getHeadboardPValues().size() * 4, &bed.getHeadboardPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[25]);
	glBufferData(GL_ARRAY_BUFFER, bed.getHeadboardNValues().size() * 4, &bed.getHeadboardNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[26]);
	glBufferData(GL_ARRAY_BUFFER, bed.getLeg0PValues().size() * 4, &bed.getLeg0PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[27]);
	glBufferData(GL_ARRAY_BUFFER, bed.getLeg0NValues().size() * 4, &bed.getLeg0NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[28]);
	glBufferData(GL_ARRAY_BUFFER, bed.getLeg1PValues().size() * 4, &bed.getLeg1PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[29]);
	glBufferData(GL_ARRAY_BUFFER, bed.getLeg1NValues().size() * 4, &bed.getLeg1NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[30]);
	glBufferData(GL_ARRAY_BUFFER, bed.getFramePValues().size() * 4, &bed.getFramePValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[31]);
	glBufferData(GL_ARRAY_BUFFER, bed.getFrameNValues().size() * 4, &bed.getFrameNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[32]);
	glBufferData(GL_ARRAY_BUFFER, bed.getMatressPValues().size() * 4, &bed.getMatressPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[33]);
	glBufferData(GL_ARRAY_BUFFER, bed.getMatressNValues().size() * 4, &bed.getMatressNValues()[0], GL_STATIC_DRAW);
}

void setupDresserVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[34]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getFramePValues().size() * 4, &dresser.getFramePValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[35]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getFrameNValues().size() * 4, &dresser.getFrameNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[36]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer0PValues().size() * 4, &dresser.getDrawer0PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[37]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer0NValues().size() * 4, &dresser.getDrawer0NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[38]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer1PValues().size() * 4, &dresser.getDrawer1PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[39]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer1NValues().size() * 4, &dresser.getDrawer1NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[40]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer2PValues().size() * 4, &dresser.getDrawer2PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[41]);
	glBufferData(GL_ARRAY_BUFFER, dresser.getDrawer2NValues().size() * 4, &dresser.getDrawer2NValues()[0], GL_STATIC_DRAW);
}

void setupRugVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[42]);
	glBufferData(GL_ARRAY_BUFFER, rug.getRugPValues().size() * 4, &rug.getRugPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[43]);
	glBufferData(GL_ARRAY_BUFFER, rug.getRugNValues().size() * 4, &rug.getRugNValues()[0], GL_STATIC_DRAW);
}

void setupSideTableVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[44]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getTopPValues().size() * 4, &sideTable.getTopPValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[45]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getTopNValues().size() * 4, &sideTable.getTopNValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[46]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg0PValues().size() * 4, &sideTable.getLeg0PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[47]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg0NValues().size() * 4, &sideTable.getLeg0NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[48]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg1PValues().size() * 4, &sideTable.getLeg1PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[49]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg1NValues().size() * 4, &sideTable.getLeg1NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[50]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg2PValues().size() * 4, &sideTable.getLeg2PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[51]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg2NValues().size() * 4, &sideTable.getLeg2NValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[52]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg3PValues().size() * 4, &sideTable.getLeg3PValues()[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[53]);
	glBufferData(GL_ARRAY_BUFFER, sideTable.getLeg3NValues().size() * 4, &sideTable.getLeg3NValues()[0], GL_STATIC_DRAW);
}

// ----- setting vertices and Shadow buffers -----
void setupVertices(void) {
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	// lamp vbos
	setupLampVBOs();

	//setup walls and floor VBOs
	setupWallFloorVBOs();

	//setup fan vbos
	setupFanVBOs();

	//setup bed vbos
	setupBedVBOs();

	//setup dresser vbos
	setupDresserVBOs();

	//setup rug vbos
	setupRugVBOs();

	//setup side table vbos
	setupSideTableVBOs();

}

void setupLampShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(0, &shadowBuffer);

	glGenTextures(0, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(2, &shadowBuffer);

	glGenTextures(2, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(4, &shadowBuffer);

	glGenTextures(4, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupWallFloorShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(6, &shadowBuffer);

	glGenTextures(6, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(8, &shadowBuffer);

	glGenTextures(8, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(10, &shadowBuffer);

	glGenTextures(10, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupFanShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(12, &shadowBuffer);

	glGenTextures(12, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(14, &shadowBuffer);

	glGenTextures(14, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(16, &shadowBuffer);

	glGenTextures(16, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(18, &shadowBuffer);

	glGenTextures(18, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(20, &shadowBuffer);

	glGenTextures(20, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(22, &shadowBuffer);

	glGenTextures(22, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupBedShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(24, &shadowBuffer);

	glGenTextures(24, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(26, &shadowBuffer);

	glGenTextures(26, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(28, &shadowBuffer);

	glGenTextures(28, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(30, &shadowBuffer);

	glGenTextures(30, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(32, &shadowBuffer);

	glGenTextures(32, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupDresserShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(34, &shadowBuffer);

	glGenTextures(34, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(36, &shadowBuffer);

	glGenTextures(36, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(38, &shadowBuffer);

	glGenTextures(38, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(40, &shadowBuffer);

	glGenTextures(40, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupRugShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(42, &shadowBuffer);

	glGenTextures(42, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupSideTableShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(44, &shadowBuffer);

	glGenTextures(44, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(46, &shadowBuffer);

	glGenTextures(46, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(48, &shadowBuffer);

	glGenTextures(48, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(50, &shadowBuffer);

	glGenTextures(50, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(52, &shadowBuffer);

	glGenTextures(52, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setupShadowBuffers(GLFWwindow* window) {
	setupLampShadowBuffers(window);
	setupWallFloorShadowBuffers(window);
	setupFanShadowBuffers(window);
	setupBedShadowBuffers(window);
	setupDresserShadowBuffers(window);
	setupRugShadowBuffers(window);
	setupSideTableShadowBuffers(window);
}

void drawLampPass1() {
	int numVertices;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			mMat = lamp.calculateBaseMMat();
			numVertices = lamp.getNumBaseVertices();
		}
		else if (i == 1) {
			mMat = lamp.calculateStemMMat();
			numVertices = lamp.getNumStemVertices();
		}
		else {
			mMat = lamp.calculateStemMMat();
			numVertices = lamp.getNumStemVertices();
		}
		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i*2]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawLampPass2() {
	int numVertices;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			mMat = lamp.calculateBaseMMat();
			numVertices = lamp.getNumBaseVertices();
			thisAmb[0] = blackMatAmb[0]; thisAmb[1] = blackMatAmb[1]; thisAmb[2] = blackMatAmb[2];  // black
			thisDif[0] = blackMatDif[0]; thisDif[1] = blackMatDif[1]; thisDif[2] = blackMatDif[2];
			thisSpe[0] = blackMatSpe[0]; thisSpe[1] = blackMatSpe[1]; thisSpe[2] = blackMatSpe[2];
			thisShi = blackMatShi;
		}
		else if (i == 1) {
			mMat = lamp.calculateStemMMat();
			numVertices = lamp.getNumStemVertices();
			thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];  // silver
			thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
			thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
			thisShi = sMatShi;
		}
		else {
			mMat = lamp.calculateShadeMMat();
			numVertices = lamp.getNumShadeVertices();
			thisAmb[0] = redMatAmb[0]; thisAmb[1] = redMatAmb[1]; thisAmb[2] = redMatAmb[2];  // red
			thisDif[0] = redMatDif[0]; thisDif[1] = redMatDif[1]; thisDif[2] = redMatDif[2];
			thisSpe[0] = redMatSpe[0]; thisSpe[1] = redMatSpe[1]; thisSpe[2] = redMatSpe[2];
			thisShi = redMatShi;
		}

		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);

		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawWallsFloorPass1() {
	int numVertices;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			mMat = wallFloor.calculateFloorMMat();
			numVertices = wallFloor.getNumFloorVertices();
		}
		else if (i == 1) {
			mMat = wallFloor.calculateLWallMMat();
			numVertices = wallFloor.getNumLWallVertices();
		}
		else {
			mMat = wallFloor.calculateRWallMMat();
			numVertices = wallFloor.getNumRWallVertices();
		}
		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2)+6]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawWallsFloorPass2() {
	int numVertices;
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			mMat = wallFloor.calculateFloorMMat();
			numVertices = wallFloor.getNumFloorVertices();
			thisAmb[0] = sMatAmb[0]; thisAmb[1] = sMatAmb[1]; thisAmb[2] = sMatAmb[2];  // silver
			thisDif[0] = sMatDif[0]; thisDif[1] = sMatDif[1]; thisDif[2] = sMatDif[2];
			thisSpe[0] = sMatSpe[0]; thisSpe[1] = sMatSpe[1]; thisSpe[2] = sMatSpe[2];
			thisShi = 1000.0f;
		}
		else if (i == 1) {
			mMat = wallFloor.calculateLWallMMat();
			numVertices = wallFloor.getNumLWallVertices();
			thisAmb[0] = blueMatAmb[0]; thisAmb[1] = blueMatAmb[1]; thisAmb[2] = blueMatAmb[2];  // blue
			thisDif[0] = blueMatDif[0]; thisDif[1] = blueMatDif[1]; thisDif[2] = blueMatDif[2];
			thisSpe[0] = blueMatSpe[0]; thisSpe[1] = blueMatSpe[1]; thisSpe[2] = blueMatSpe[2];
			thisShi = 1000.0f;
		}
		else {
			mMat = wallFloor.calculateRWallMMat();
			numVertices = wallFloor.getNumRWallVertices();
			thisAmb[0] = blueMatAmb[0]; thisAmb[1] = blueMatAmb[1]; thisAmb[2] = blueMatAmb[2];  // blue
			thisDif[0] = blueMatDif[0]; thisDif[1] = blueMatDif[1]; thisDif[2] = blueMatDif[2];
			thisSpe[0] = blueMatSpe[0]; thisSpe[1] = blueMatSpe[1]; thisSpe[2] = blueMatSpe[2];
			thisShi = 1000.0f;
		}
		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);

		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+6]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 7]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawFanPass1() {
	int numVertices;
	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			mMat = fan.calculateBaseMMat();
			numVertices = fan.getNumBaseVertices();
		}
		else if (i == 1) {
			mMat = fan.calculateStemMMat();
			numVertices = fan.getNumStemVertices();
		}
		else if (i == 2) {
			mMat = fan.calculateMotorMMat();
			numVertices = fan.getNumMotorVertices();
		}
		else if (i == 3) {
			mMat = fan.calculateBlade0MMat(fanBladeRotation);
			numVertices = fan.getNumBlade0Vertices();
		}
		else if (i == 4) {
			mMat = fan.calculateBlade1MMat(fanBladeRotation);
			numVertices = fan.getNumBlade1Vertices();
		}
		else {
			mMat = fan.calculateBlade2MMat(fanBladeRotation);
			numVertices = fan.getNumBlade2Vertices();
		}
		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2)+12]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawFanPass2() {

	int numVertices;
	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			mMat = fan.calculateBaseMMat();
			numVertices = fan.getNumBaseVertices();
			thisAmb[0] = whiteMatAmb[0]; thisAmb[1] = whiteMatAmb[1]; thisAmb[2] = whiteMatAmb[2];  // white
			thisDif[0] = whiteMatDif[0]; thisDif[1] = whiteMatDif[1]; thisDif[2] = whiteMatDif[2];
			thisSpe[0] = whiteMatSpe[0]; thisSpe[1] = whiteMatSpe[1]; thisSpe[2] = whiteMatSpe[2];
			thisShi = whiteMatShi;
		}
		else if (i == 1) {
			mMat = fan.calculateStemMMat();
			numVertices = fan.getNumStemVertices();
		}
		else if (i == 2) {
			mMat = fan.calculateMotorMMat();
			numVertices = fan.getNumMotorVertices();
		}
		else if (i == 3) {
			mMat = fan.calculateBlade0MMat(fanBladeRotation);
			numVertices = fan.getNumBlade0Vertices();
		}
		else if (i == 4) {
			mMat = fan.calculateBlade1MMat(fanBladeRotation);
			numVertices = fan.getNumBlade1Vertices();
		}
		else {
			mMat = fan.calculateBlade2MMat(fanBladeRotation);
			numVertices = fan.getNumBlade2Vertices();
		}

		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);

		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 12]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 13]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawBedPass1() {
	int numVertices;
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			mMat = bed.calculateHeadboardMMat();
			numVertices = bed.getNumHeadboardVertices();
		}
		else if (i == 1) {
			mMat = bed.calculateLeg0MMat();
			numVertices = bed.getNumLeg0Vertices();
		}
		else if (i == 2) {
			mMat = bed.calculateLeg1MMat();
			numVertices = bed.getNumLeg1Vertices();
		}
		else if (i == 3) {
			mMat = bed.calculateFrameMMat();
			numVertices = bed.getNumFrameVertices();
		}
		else {
			mMat = bed.calculateMatressMMat();
			numVertices = bed.getNumMatressVertices();
		}

		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 24]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);

	}
}

void drawBedPass2() {
	int numVertices;
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			mMat = bed.calculateHeadboardMMat();
			numVertices = bed.getNumHeadboardVertices();
			thisAmb[0] = brownMatAmb[0]; thisAmb[1] = brownMatAmb[1]; thisAmb[2] = brownMatAmb[2];  // brown
			thisDif[0] = brownMatDif[0]; thisDif[1] = brownMatDif[1]; thisDif[2] = brownMatDif[2];
			thisSpe[0] = brownMatSpe[0]; thisSpe[1] = brownMatSpe[1]; thisSpe[2] = brownMatSpe[2];
			thisShi = brownMatShi;
		}
		else if (i == 1) {
			mMat = bed.calculateLeg0MMat();
			numVertices = bed.getNumLeg0Vertices();
		}
		else if (i == 2) {
			mMat = bed.calculateLeg1MMat();
			numVertices = bed.getNumLeg1Vertices();
		}
		else if (i == 3) {
			mMat = bed.calculateFrameMMat();
			numVertices = bed.getNumFrameVertices();
		}
		else {
			mMat = bed.calculateMatressMMat();
			numVertices = bed.getNumMatressVertices();
			thisAmb[0] = yellowMatAmb[0]; thisAmb[1] = yellowMatAmb[1]; thisAmb[2] = yellowMatAmb[2];  // yellow
			thisDif[0] = yellowMatDif[0]; thisDif[1] = yellowMatDif[1]; thisDif[2] = yellowMatDif[2];
			thisSpe[0] = yellowMatSpe[0]; thisSpe[1] = yellowMatSpe[1]; thisSpe[2] = yellowMatSpe[2];
			thisShi = yellowMatShi;
		}
		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);
	
		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+24]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+25]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawDresserPass1() {
	int numVertices;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			mMat = dresser.calculateFrameMMat();
			numVertices = dresser.getNumFrameVertices();
		}
		else if (i == 1) {
			mMat = dresser.calculateDrawer0MMat();
			numVertices = dresser.getNumDrawer0Vertices();
		}
		else if (i == 2) {
			mMat = dresser.calculateDrawer1MMat();
			numVertices = dresser.getNumDrawer1Vertices();
		}
		else {
			mMat = dresser.calculateDrawer2MMat();
			numVertices = dresser.getNumDrawer2Vertices();
		}
		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 34]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawDresserPass2() {
	int numVertices;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			mMat = dresser.calculateFrameMMat();
			numVertices = dresser.getNumFrameVertices();
			thisAmb[0] = brownMatAmb[0]; thisAmb[1] = brownMatAmb[1]; thisAmb[2] = brownMatAmb[2];  // brown
			thisDif[0] = brownMatDif[0]; thisDif[1] = brownMatDif[1]; thisDif[2] = brownMatDif[2];
			thisSpe[0] = brownMatSpe[0]; thisSpe[1] = brownMatSpe[1]; thisSpe[2] = brownMatSpe[2];
			thisShi = brownMatShi;
		}
		else if (i == 1) {
			mMat = dresser.calculateDrawer0MMat();
			numVertices = dresser.getNumDrawer0Vertices();
			thisAmb[0] = brownMatAmb[0]; thisAmb[1] = brownMatAmb[1]; thisAmb[2] = brownMatAmb[2];  // lighter brown
			thisDif[0] = brownMatDif[0]; thisDif[1] = brownMatDif[1]; thisDif[2] = brownMatDif[2];
			thisSpe[0] = brownMatSpe[0]; thisSpe[1] = brownMatSpe[1]; thisSpe[2] = brownMatSpe[2];
			thisShi = 1.0f;
		}
		else if (i == 2) {
			mMat = dresser.calculateDrawer1MMat();
			numVertices = dresser.getNumDrawer1Vertices();
		}
		else {
			mMat = dresser.calculateDrawer2MMat();
			numVertices = dresser.getNumDrawer2Vertices();
		}
		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);

		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 34]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 35]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawRugPass1() {
	//draw rug
	mMat = rug.calculateRugMMat();
	shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[42]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, rug.getNumRugVertices());
}

void drawRugPass2() {
	thisAmb[0] = greenMatAmb[0]; thisAmb[1] = greenMatAmb[1]; thisAmb[2] = greenMatAmb[2];  // green
	thisDif[0] = greenMatDif[0]; thisDif[1] = greenMatDif[1]; thisDif[2] = greenMatDif[2];
	thisSpe[0] = greenMatSpe[0]; thisSpe[1] = greenMatSpe[1]; thisSpe[2] = greenMatSpe[2];
	thisShi = greenMatShi;

	currentLightPos = glm::vec3(lightLoc);
	currentLight1Pos = glm::vec3(light1Loc);
	installLights(renderingProgram2, vMat);

	// draw the rug
	mMat = rug.calculateRugMMat();
	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[42]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[43]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, rug.getNumRugVertices());
}

void drawSideTablePass1() {
	int numVertices;
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			mMat = sideTable.calculateTopMMat();
			numVertices = sideTable.getNumTopVertices();
		}
		else if (i == 1) {
			mMat = sideTable.calculateLeg0MMat();
			numVertices = sideTable.getNumLeg0Vertices();
		}
		else if (i == 2) {
			mMat = sideTable.calculateLeg1MMat();
			numVertices = sideTable.getNumLeg1Vertices();
		}
		else if (i == 3) {
			mMat = sideTable.calculateLeg2MMat();
			numVertices = sideTable.getNumLeg2Vertices();
		}
		else {
			mMat = sideTable.calculateLeg3MMat();
			numVertices = sideTable.getNumLeg3Vertices();
		}
		shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
		sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i * 2) + 44]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

void drawSideTablePass2() {
	int numVertices;
	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			mMat = sideTable.calculateTopMMat();
			numVertices = sideTable.getNumTopVertices();
			thisAmb[0] = brownMatAmb[0]; thisAmb[1] = brownMatAmb[1]; thisAmb[2] = brownMatAmb[2];  // brown
			thisDif[0] = brownMatDif[0]; thisDif[1] = brownMatDif[1]; thisDif[2] = brownMatDif[2];
			thisSpe[0] = brownMatSpe[0]; thisSpe[1] = brownMatSpe[1]; thisSpe[2] = brownMatSpe[2];
			thisShi = brownMatShi;
		}
		else if (i == 1) {
			mMat = sideTable.calculateLeg0MMat();
			numVertices = sideTable.getNumLeg0Vertices();
		}
		else if (i == 2) {
			mMat = sideTable.calculateLeg1MMat();
			numVertices = sideTable.getNumLeg1Vertices();
		}
		else if (i == 3) {
			mMat = sideTable.calculateLeg2MMat();
			numVertices = sideTable.getNumLeg2Vertices();
		}
		else {
			mMat = sideTable.calculateLeg3MMat();
			numVertices = sideTable.getNumLeg3Vertices();
		}
		currentLightPos = glm::vec3(lightLoc);
		currentLight1Pos = glm::vec3(light1Loc);
		installLights(renderingProgram2, vMat);

		mvMat = vMat * mMat;
		invTrMat = glm::transpose(glm::inverse(mvMat));
		shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
		glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+44]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[(i*2)+45]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void init(GLFWwindow* window) {
	renderingProgram1 = Utils::createShaderProgram("./vert1Shader.glsl", "./frag1Shader.glsl");
	renderingProgram2 = Utils::createShaderProgram("./vert2Shader.glsl", "./frag2Shader.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();
	setupShadowBuffers(window);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc.x, -cameraLoc.y, -cameraLoc.z));
	vMat = glm::rotate(vMat, toRadians(currRotationX), glm::vec3(0.0f, 1.0f, 0.0f));

	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	currentLightPos = glm::vec3(lightLoc);
	currentLight1Pos = glm::vec3(light1Loc);

	lightVmatrix = glm::lookAt(currentLightPos, origin, up);
	lightPmatrix = glm::perspective(toRadians(60.0f), aspect, 0.1f, 1000.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);

	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);	// for reducing
	glPolygonOffset(2.0f, 4.0f);		//  shadow artifacts

	passOne();
	
	glClear(GL_DEPTH_BUFFER_BIT);

	glDisable(GL_POLYGON_OFFSET_FILL);	// artifact reduction, continued
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDrawBuffer(GL_FRONT);

	passTwo();

	fanBladeRotation -= 5.0f;

	if (fanBladeRotation <= -360.0f) {
		fanBladeRotation = 0.0f;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void passOne(void) {
	glUseProgram(renderingProgram1);

	drawLampPass1();
	drawWallsFloorPass1();
	drawFanPass1();
	drawBedPass1();
	drawDresserPass1();
	drawRugPass1();
	drawSideTablePass1();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void passTwo(void) {
	glUseProgram(renderingProgram2);

	mvLoc = glGetUniformLocation(renderingProgram2, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram2, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram2, "norm_matrix");
	sLoc = glGetUniformLocation(renderingProgram2, "shadowMVP");

	drawLampPass2();
	drawWallsFloorPass2();
	drawFanPass2();
	drawBedPass2();
	drawDresserPass2();
	drawRugPass2();
	drawSideTablePass2();
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_UP:
			currRotationY -= 1.0f;
			break;
		case GLFW_KEY_DOWN:
			currRotationY += 1.0f;
			break;
		case GLFW_KEY_LEFT:
			currRotationX -= 1.0f;
			break;
		case GLFW_KEY_RIGHT:
			currRotationX += 1.0f;
			break;
		}
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc.x, -cameraLoc.y, -cameraLoc.z));
		vMat = glm::rotate(vMat, toRadians(currRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
		vMat = glm::rotate(vMat, toRadians(currRotationY), glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			cameraLoc.z -= 0.5f;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			cameraLoc.z += 0.5f;
		}
	}
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraLoc.x, -cameraLoc.y, -cameraLoc.z));
	vMat = glm::rotate(vMat, toRadians(currRotationX), glm::vec3(0.0f, 1.0f, 0.0f));
	vMat = glm::rotate(vMat, toRadians(currRotationY), glm::vec3(1.0f, 0.0f, 0.0f));
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Programming Assignment 2", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
