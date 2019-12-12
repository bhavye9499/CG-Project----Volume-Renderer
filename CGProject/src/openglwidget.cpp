#include <stdio.h>
#include <stdlib.h>

#include "openglwidget.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>
#include <iostream>
#include <vector>
#include <QGLContext>

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

#define PI 3.14159265359f

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
	glDeleteProgram(program);
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	printContextInformation();

	texW = 256;
	texH = 256;
    texD = 178;

	//Enable certain OpenGL states
    glEnable(GL_DEPTH_TEST); //Enable Z-buffer
	glEnable(GL_MULTISAMPLE); //Draw smoothed polygons
    printf("here\n");
    glCheckError();
    printf("here2\n");

	//Create program
	program = createProgram("./shaders/vshader2.vs", "./shaders/fshader2.fs");
	glUseProgram(program);

	// Loading the volume
    loadVolume("teapot(256x256x178).raw", &tex);

	// Binding volume to sampler3D in fragment shader
	texLoc = glGetUniformLocation(program, "volumeSampler");
	if(texLoc == -1) {
		fprintf(stderr, "Could not bind location: volumeSampler\n");
		std::cout << glGetError() << std::endl;
		exit(0);
	}
    glUniform1i(texLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, tex);

	// Setting camera location
    eye = glm::vec3(0, 0, 2);
	GLint eyeLoc = glGetUniformLocation(program, "eye");
	if(eyeLoc == -1) {
		fprintf(stderr, "Could not bind location: eye\n");
		exit(0);
	}
	glUniform3f(eyeLoc, eye.x, eye.y, eye.z);

	//Setup Transformations
	setupModelTransformation();
	setupViewTransformation();
	setupProjectionTransformation();

	// Creating proxy cube
	createCubeObject();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	screen_width = w;
	screen_height = h;
	glViewport(0, 0, screen_width, screen_height);
	setupProjectionTransformation();// Redo projection matrix
}

void OpenGLWidget::paintGL()
{
	/* Clear the background as white */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(cube_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 36 * sizeof(GLfloat));
}

void OpenGLWidget::changeRGBofTF(const char* uniformName, int value)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, uniformName);
	if (loc == -1) {
		fprintf(stderr, "Could not bind location: Color Slider\n");
		exit(0);
	}

	glUniform1f(loc, value / 255.0f);
	update();
}

void OpenGLWidget::changeMidColorPosTF(const char* uniformName, int value)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, uniformName);
	if (loc == -1) {
		fprintf(stderr, "Could not bind location: Color Slider\n");
		exit(0);
	}

	glUniform1f(loc, value / 100.0f);
	update();
}

void OpenGLWidget::createCubeObject()
{
	const GLfloat cube_vertices[3 * 36] = {
		-0.5 , -0.5 , -0.5 ,
		-0.5 , -0.5 ,  0.5 ,
		-0.5 ,  0.5 ,  0.5 ,
		 0.5 ,  0.5 , -0.5 ,
		-0.5 , -0.5 , -0.5 ,
		-0.5 ,  0.5 , -0.5 ,
		 0.5 , -0.5 ,  0.5 ,
		-0.5 , -0.5 , -0.5 ,
		 0.5 , -0.5 , -0.5 ,
		 0.5 ,  0.5 , -0.5 ,
		 0.5 , -0.5 , -0.5 ,
		-0.5 , -0.5 , -0.5 ,
		-0.5 , -0.5 , -0.5 ,
		-0.5 ,  0.5 ,  0.5 ,
		-0.5 ,  0.5 , -0.5 ,
		 0.5 , -0.5 ,  0.5 ,
		-0.5 , -0.5 ,  0.5 ,
		-0.5 , -0.5 , -0.5 ,
		-0.5 ,  0.5 ,  0.5 ,
		-0.5 , -0.5 ,  0.5 ,
		 0.5 , -0.5 ,  0.5 ,
		 0.5 ,  0.5 ,  0.5 ,
		 0.5 , -0.5 , -0.5 ,
		 0.5 ,  0.5 , -0.5 ,
		 0.5 , -0.5 , -0.5 ,
		 0.5 ,  0.5 ,  0.5 ,
		 0.5 , -0.5 ,  0.5 ,
		 0.5 ,  0.5 ,  0.5 ,
		 0.5 ,  0.5 , -0.5 ,
		-0.5 ,  0.5 , -0.5 ,
		 0.5 ,  0.5 ,  0.5 ,
		-0.5 ,  0.5 , -0.5 ,
		-0.5 ,  0.5 ,  0.5 ,
		 0.5 ,  0.5 ,  0.5 ,
		-0.5 ,  0.5 ,  0.5 ,
		 0.5 , -0.5 , 0.5
	};

	GLuint vbo;
	glGenVertexArrays(1, &cube_VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(cube_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), cube_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void OpenGLWidget::loadVolume(const char *filename, GLuint *tex)
{
   GLubyte* texData = readFromFileUnsigned(filename);
   glGenTextures(1, tex);
   glBindTexture(GL_TEXTURE_3D, *tex);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage3D(GL_TEXTURE_3D,0,GL_R8,texW,texH,texD,0,GL_RED,GL_UNSIGNED_BYTE,texData);

   printf("Vol 3\n");   glCheckError();   printf("Vol 4\n");
   delete [] texData;
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event){
	if (event->button() == Qt::LeftButton){
		oldX = event->x();
		oldY = event->y();
	}
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event){
	if(event->buttons() & Qt::LeftButton)
	{
		currentX = event->x();
		currentY = event->y();

		if(currentX !=oldX || currentY != oldY)
		{
			glUseProgram(program);

			glm::vec3 va = getTrackBallVector(oldX, oldY);
			glm::vec3 vb = getTrackBallVector(currentX, currentY);
			float angle = acos(fmin(1.0f, glm::dot(va, vb)));
			glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
			glm::mat3 camera2object = glm::inverse(glm::mat3(view*model));
			glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
			model = glm::rotate(model, angle, axis_in_object_coord);
			glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
			oldX = currentX;
			oldY = currentY;

			this->update();
		}
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event){
	if(event->button() == Qt::LeftButton){
		oldX = event->x();
		oldY = event->y();
	}
}

void OpenGLWidget::setupModelTransformation()
{
	//Modelling transformations (Model -> World coordinates)
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

	//Pass on the modelling matrix to the vertex shader
	glUseProgram(program);
	vModel_uniform = glGetUniformLocation(program, "vModel");
	if(vModel_uniform == -1){
		fprintf(stderr, "Could not bind location: vModel\n");
		exit(0);
	}
	glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(model));
}

void OpenGLWidget::setupViewTransformation()
{
	//Viewing transformations (World -> Camera coordinates
	view = glm::lookAt(eye, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	//Pass on the viewing matrix to the vertex shader
	glUseProgram(program);
	vView_uniform = glGetUniformLocation(program, "vView");
	if(vView_uniform == -1){
		fprintf(stderr, "Could not bind location: vView\n");
		exit(0);
	}
	glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(view));
}

void OpenGLWidget::setupProjectionTransformation()
{
	//Projection transformation (Perspective projection)
	float aspect = (float)screen_width/(float)screen_height;

	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	//Pass on the projection matrix to the vertex shader
	glUseProgram(program);
	vProjection_uniform = glGetUniformLocation(program, "vProjection");
	if(vProjection_uniform == -1){
		fprintf(stderr, "Could not bind location: vProjection\n");
		exit(0);
	}
	glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}

glm::vec3 OpenGLWidget::getTrackBallVector(int x, int y)
{
	glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if(mag2 <= 1.0)
		p.z = sqrt(1.0 - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}

void OpenGLWidget::printContextInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;

	//Get GL version info
	glType = (context()->isOpenGLES())?"OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	//Get profile information

#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch(format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	qDebug() <<qPrintable(glType)<<qPrintable(glVersion)<<"("<<qPrintable(glProfile) << ")";

}

GLuint OpenGLWidget::createProgram(const char *vshader_filename, const char* fshader_filename)
{
	//Create shader objects
	GLuint vs, fs;
	if ((vs = createShader(vshader_filename, GL_VERTEX_SHADER))   == 0) return 0;
	if ((fs = createShader(fshader_filename, GL_FRAGMENT_SHADER)) == 0) return 0;

	//Creare program object and link shader objects
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	GLint link_ok;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram error:");
		printLog(program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

char* OpenGLWidget::readFromFile(const char* filename)
{
	FILE* input = fopen(filename, "rb");
	if(input == NULL) return NULL;

	if(fseek(input, 0, SEEK_END) == -1) return NULL;
	long size = ftell(input);
	if(size == -1) return NULL;
	if(fseek(input, 0, SEEK_SET) == -1) return NULL;

	/*if using c-compiler: dont cast malloc's return value*/
	char *content = (char*) malloc( (size_t) size +1  );
	if(content == NULL) return NULL;

	fread(content, 1, (size_t)size, input);
	if(ferror(input)) {
		free(content);
		return NULL;
	}

	fclose(input);
	content[size] = '\0';
	return content;
}

GLubyte* OpenGLWidget::readFromFileUnsigned(const char* filename)
{
    FILE* input = fopen(filename, "rb");
    if(input == NULL) return NULL;

    if(fseek(input, 0, SEEK_END) == -1) return NULL;
    long size = ftell(input);
    if(size == -1) return NULL;
    if(fseek(input, 0, SEEK_SET) == -1) return NULL;

    /*if using c-compiler: dont cast malloc's return value*/
    GLubyte *content = (GLubyte*) malloc( (size_t) size +1  );
    if(content == NULL) return NULL;

    fread(content, 1, (size_t)size, input);
    if(ferror(input)) {
        free(content);
        return NULL;
    }

    fclose(input);
    content[size] = '\0';
    return content;
}

char* OpenGLWidget::getShaderCode(const char* filename)
{
	return readFromFile(filename);
}

void OpenGLWidget::printLog(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else if (glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	else {
		fprintf(stderr, "printlog: Not a shader or a program\n");
		return;
	}

	char* log = (char*)malloc(log_length);

	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, NULL, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, NULL, log);

	fprintf(stderr, "%s", log);
	free(log);
}

GLuint OpenGLWidget::createShader(const char* filename, GLenum type)
{
	const GLchar* source = getShaderCode(filename);
	if (source == NULL) {
		fprintf(stderr, "Error opening %s: ", filename); perror("");
		return 0;
	}
	GLuint res = glCreateShader(type);
	glShaderSource(res, 1, &source, NULL);
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s:", filename);
		printLog(res);
		glDeleteShader(res);
		return 0;
	}

	return res;
}
