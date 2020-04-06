#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

#include <iostream>

#include "Plane.h"

Scene::Scene() {

}

Scene::~Scene()
{
    for (std::vector<TriangleMesh *>::iterator it = shapes.begin(); it != shapes.end(); ++it)
    {
        delete (*it);
    }

    shapes.clear();
}

void Scene::initScene()
{
    compileAndLinkShader();

    shader.setUniform("Fog.maxDist", 4800.0f);
    shader.setUniform("Fog.minDist", 300.0f);
    shader.setUniform("Fog.color", vec3(0.71f, 0.95f, 1.0f));
}

void Scene::addShape(TriangleMesh *mesh)
{
    shapes.push_back(mesh);
}


void Scene::compileAndLinkShader()
{
    try {
		shader.compileShader("./shaders/tex.vert",GLSLShader::VERTEX);
		shader.compileShader("./shaders/tex.frag",GLSLShader::FRAGMENT);
		// shader.compileShader("./shader/shadewire.gs",GLSLShader::GEOMETRY);
		shader.link();
    	shader.use();
    } catch(GLSLProgramException &e ) {
    	std::cerr << e.what() << std::endl;
 		exit( EXIT_FAILURE );
    }
}

void Scene::render(glm::mat4 view, glm::mat4 proj)
{
    shader.use();
    shader.setUniform("Light.position", view * glm::vec4(0.0f, 10000.0f, 10000.0f, 0.0f));
    shader.setUniform("Light.intensity", vec3(0.8f, 0.8f, 0.8f));

    GLuint meshVertIndex = glGetSubroutineIndex(
        shader.getHandle(),
        GL_VERTEX_SHADER,
        "mesh");
    GLuint meshFragIndex = glGetSubroutineIndex(
        shader.getHandle(),
        GL_FRAGMENT_SHADER,
        "mesh");

    GLuint charVertIndex = glGetSubroutineIndex(
        shader.getHandle(),
        GL_VERTEX_SHADER,
        "character");
    GLuint charFragIndex = glGetSubroutineIndex(
        shader.getHandle(),
        GL_FRAGMENT_SHADER,
        "character");

    for (std::vector<TriangleMesh *>::iterator it = shapes.begin(); it != shapes.end(); ++it)
    {
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &meshVertIndex);
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &meshFragIndex);
        (*it)->render(&shader, view, proj);
    }

    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &charVertIndex);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &charFragIndex);
}
