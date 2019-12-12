#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	OpenGLWidget(QWidget *parent);
	~OpenGLWidget();

	void changeRGBofTF(const char* uniformName, int value);
	void changeMidColorPosTF(const char* uniformName, int value);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void setupModelTransformation();
	void setupViewTransformation();
	void setupProjectionTransformation();
	void printContextInformation();
	GLuint createProgram(const char *vshader_filename, const char* fshader_filename);
	char* getShaderCode(const char* filename);
	void printLog(GLuint object);
	GLuint createShader(const char* filename, GLenum type);
	glm::vec3 getTrackBallVector(int x, int y);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	int currentX, currentY;
	int oldX, oldY;
	int size;
	void createCubeObject();
	size_t nTriangles;
	glm::vec3 eye;

	// My code
	// -------
	int texW, texH, texD;
	char* readFromFile(const char* filename);
    GLubyte* readFromFileUnsigned(const char* filename);
    void loadVolume(const char* filename, GLuint *tex);
    GLint texLoc;
	// -------

private:
	GLuint program;
	GLint vVertex_attrib, vNormal_attrib, vColor_attrib;
	GLint vModel_uniform, vView_uniform, vProjection_uniform;
	int screen_width, screen_height;


	glm::mat4 model;
	glm::mat4 view;

	GLuint cube_VAO, sphere_VAO, indices_IBO; //Vertex array object for cube

	int tri_points;
	int nTheta, nPhi;
	std::vector<float> x,y,z, arr;
	std::vector<float>  tri_vertices;
	std::vector<short> tri;
	bool load_texture( const char *file_name, GLuint *tex );
	std::vector<float> u_c, v_c;
	void generateSphereCoords();
	GLuint texcoords_vbo;
	GLuint tex;
};

#endif // OPENGLWIDGET_H
