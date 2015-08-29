#include <windows.h>
#include "GL/glew.h"
#include "gl\gl.h"
#include "GL/glut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include "ObjFileParser.h"
#include "ModelObjectDTO.h"
#include <cmath>
#include <iostream>
#include "Camera.h"
#include "OpenGLHelper.h"

using namespace std;
using namespace glm;

float *geomVertices;

ModelObjectDTO model;
Camera camera;

int geomVertexCount;

void displayFrame(void) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float fovy = 50.0f;
	float aspect = 1.0f;
	float zNear = 1.0f;
	float zFar = 1000.0f;
	mat4 M = mat4(1.0f);
	mat4 V = lookAt(camera.observer, camera.center, camera.noseVector);
	mat4 P = perspective(fovy, aspect, zNear, zFar);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(V*M));

	glColor3d(0, 1, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, geomVertices);
	glDrawArrays(GL_LINE_LOOP, 0, geomVertexCount);
 	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
}

void modelToArray(ModelObjectDTO *model, float *array)
{
	int i = 0;
	for each (vector<float> vertex in model->Vertices)
	{
		array[i] = vertex[0];
		array[i + 1] = vertex[1];
		array[i + 2] = vertex[2];
		i += 3;
	}
}

void keyDown(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_RIGHT:
		camera.alpha += 2.0f;
		break;
	case GLUT_KEY_LEFT:
		camera.alpha -= 2.0f;
		break;
	case GLUT_KEY_UP:
		camera.stepForward();
		break;
	case GLUT_KEY_DOWN:
		camera.stepBack();
	}

	camera.determineCenterPoint();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	OpenGLHelper::initializeGLUT(&argc, argv);
	glutDisplayFunc(displayFrame);
	OpenGLHelper::initializeGLEW();
	ObjFileParser ofp("castle.obj");
	model = ofp.Parse();
	geomVertices = new float[model.Vertices.size() * 3];
	modelToArray(&model, geomVertices);
	geomVertexCount = model.Vertices.size();
	//Initializing code here
	glEnable(GL_LIGHTING);			//TODO: przetestować 2 tryby cieniowania i przenieść do osobnej funkcji
	glEnable(GL_LIGHT0);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);

	//TODO: ogarnąć Z-buffor:
	glEnable(GL_DEPTH_TEST);
	//TODO: Tutaj tekstury:
	glEnable(GL_COLOR_MATERIAL);
	glutSpecialFunc(keyDown);
	glutMainLoop();
	//Resource releasing code here
	delete geomVertices;
	return 0;
}
