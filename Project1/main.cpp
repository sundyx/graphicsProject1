///////////////////////////////////////////
////Type your name and student ID here/////
////Name:Sandy Xie	
////Student ID:1155140960

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
float x_delta = 0.1f;
int x_press_num = 0;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO:
	if (key == 'a')
	{
		x_press_num -= 1;
	}
	if (key == 'd')
	{
		x_press_num += 1;
	}
}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	//const GLfloat square[] =
	//{
	//	-0.5f, -0.5f, +0.0f, //top left
	//	+0.5f, -0.5f, +0.0f, //top right
	//	-0.5f, +0.5f, +0.0f,

	//	+0.5f, -0.5f, +0.0f, //bottom right
	//	+0.5f, +0.5f, +0.0f, //bottom left
	//	-0.5f, +0.5f, +0.0f, //bottom left
	//
	//};

	//use transformation matrix 
	const GLfloat square[] =
	{
		-0.5f, -0.5f, 0.0f, //left

		+0.5f, -0.5f, -0.0f, //right 
		

		+0.5f, +0.5f, -0.0f, //top right

		-0.5, +0.5, -0.0f //top left
	};
	GLushort indices[] = { 0,1,3,
							1,2,3,};

	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);  //first VAO

	GLuint vbo; //vertex buffer object
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//indexing 
	GLuint vbo_idx;
	glGenBuffers(1, &vbo_idx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f); //specify the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 projectionMatrix = glm::perspective(20.0f, 1.0f, 7.0f, 100.0f);
	GLint projectionMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);

	glm::mat4 modeltransformmatrix = glm::mat4(1.0f);
	modeltransformmatrix = glm::translate(glm::mat4(),
		glm::vec3(x_delta * x_press_num, 0.0f, -9.0f));
	glm::vec3 myRotateAxis(0, 0, 1);
	modeltransformmatrix = glm::rotate(modeltransformmatrix, 0.0f, myRotateAxis);
	GLint modeltransformmatrixuniformlocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modeltransformmatrixuniformlocation, 1,
		GL_FALSE, &modeltransformmatrix[0][0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); //render primitives from array data

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);
	
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}