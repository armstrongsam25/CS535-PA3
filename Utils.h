#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Utils
{
private:
	static std::string readShaderFile(const char *filePath);
	static void printShaderLog(GLuint shader);
	static void printProgramLog(int prog);
	static GLuint prepareShader(int shaderTYPE, const char *shaderPath);
	static int finalizeShaderProgram(GLuint sprogram);

public:
	Utils();
	static void displayComputeShaderLimits();
	static bool checkOpenGLError();
	static GLuint createShaderProgram(const char *cp);
	static GLuint createShaderProgram(const char *vp, const char *fp);
	static GLuint createShaderProgram(const char *vp, const char *gp, const char *fp);
	static GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, const char *fp);
	static GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, char *gp, const char *fp);
	static GLuint loadTexture(const char *texImagePath);
	static GLuint loadCubeMap(const char *mapDir);

	static float* goldAmbient();
	static float* goldDiffuse();
	static float* goldSpecular();
	static float goldShininess();

	static float* silverAmbient();
	static float* silverDiffuse();
	static float* silverSpecular();
	static float silverShininess();

	static float* bronzeAmbient();
	static float* bronzeDiffuse();
	static float* bronzeSpecular();
	static float bronzeShininess();

	static float* brownAmbient();
	static float* brownDiffuse();
	static float* brownSpecular();
	static float brownShininess();

	static float* blueAmbient();
	static float* blueDiffuse();
	static float* blueSpecular();
	static float blueShininess();

	static float* blackAmbient();
	static float* blackDiffuse();
	static float* blackSpecular();
	static float blackShininess();

	static float* yellowAmbient();
	static float* yellowDiffuse();
	static float* yellowSpecular();
	static float yellowShininess();

	static float* whiteAmbient();
	static float* whiteDiffuse();
	static float* whiteSpecular();
	static float whiteShininess();

	static float* redAmbient();
	static float* redDiffuse();
	static float* redSpecular();
	static float redShininess();

	static float* grayAmbient();
	static float* grayDiffuse();
	static float* graySpecular();
	static float grayShininess();

	static float* greenAmbient();
	static float* greenDiffuse();
	static float* greenSpecular();
	static float greenShininess();
};