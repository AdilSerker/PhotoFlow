#include "App.h"

#include <iostream>
using namespace std;

#include "Camera.h"
#include "Scene.h"
#include "Plane.h"

void App::update()
{
	// const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// int WINDOW_WIDTH = mode->width / 1.5;
	// int WINDOW_HEIGHT = mode->height / 1.5;

	camera->computeMatrices(window);
	// glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	for (auto pl : planes) {
		if (pl)
			pl->position += glm::vec3(0.f, 0.f, 0.1f);
	}
}

void App::render()
{
	scene->render(camera->getViewMatrix(), camera->getProjectionMatrix());
}

App::App()
{
	initWindow();

	scene = nullptr;

	camera = nullptr;
}

App::~App()
{
	delete scene;
	delete camera;
}

void App::init()
{
	this->scene = new Scene();
	scene->initScene();

	int column = 3;

	for (int i = 1; i < 37; i++) {
		Plane* pl = new Plane("./pictures/photos/photo" + std::to_string(i) + ".png");

		float x = column == 3 ? 5.f : column == 1 ? -15.f : column == 2 ? -5.f : 15.f;

		pl->position = glm::vec3(x, -10.f, (i+5) * -15.f);

		scene->addShape(pl);
		planes.push_back(pl);

		column = column == 3 ? 2 : column == 2 ? 4 : column == 4 ? 1 : 3;
	}

	this->camera = new Camera();

}

void App::initWindow()
{
	if (!glfwInit())
	{
		fprintf(stderr, "GLFW Initialization error\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int WINDOW_WIDTH = 1280; // mode->width / 1.5;
	int WINDOW_HEIGHT = 720; // mode->height / 1.5;

	window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"Photo flow",
		NULL, 
		// glfwGetPrimaryMonitor(),
		NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Window initialization error\n");
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW Initialization error\n");
		exit(-1);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.19f, 0.23f, 0.16f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
}

double currentTimeInMs()
{
	return glfwGetTime() * 1000;
}

void App::run()
{
	
	double MS_PER_TICK = 1000 / 30;
	double PROCESSED_TIME = currentTimeInMs();
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		double currentTime = glfwGetTime();
		nbFrames++;

		char fps[256];
		// char cameraPos[256];
		
		if (currentTime - lastTime >= 1.0)
		{
			sprintf(fps,"%i fps", nbFrames );
			// sprintf(cameraPos, "%f.2"); 


			nbFrames = 0;
			lastTime += 1.0;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		while ((PROCESSED_TIME + MS_PER_TICK) < currentTimeInMs())
		{
			update();
			PROCESSED_TIME += MS_PER_TICK;
		}

		render();



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
}
