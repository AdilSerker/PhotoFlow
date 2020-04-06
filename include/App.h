#ifndef APP_CORE_H
#define APP_CORE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

class App
{
	GLFWwindow *window;
	GLuint vao, vbo, ibo;
	class GLSLProgram *shader;

	class Camera *camera;
	class Scene *scene;

	std::vector<class Plane*> planes;

	void initWindow();
	void update();
	void render();

  public:
	App();
	~App();
	void init();
	void run();
};

#endif //APP_CORE_H
