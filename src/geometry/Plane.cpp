#include "Plane.h"

#include <SOIL/SOIL.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Plane::Plane(std::string tex_path)
{
    position = glm::vec3(0.f, 0.f, -50.f);

    float xsize = 9;
    float zsize = 12;
    int xdivs = 1;
    int zdivs = 1;
    float smax = 1.f;
    float tmax = 1.f;

    std::vector<GLfloat> p(3 * (xdivs + 1) * (zdivs + 1));
    std::vector<GLfloat> n(3 * (xdivs + 1) * (zdivs + 1));
    std::vector<GLfloat> tex(2 * (xdivs + 1) * (zdivs + 1));
    std::vector<GLuint> el(6 * xdivs * zdivs);
    int width, height, nrChannels;

    unsigned char *image = SOIL_load_image(tex_path.c_str(), &width, &height, &nrChannels, SOIL_LOAD_AUTO);
    if (image)
    {
        float x2 = xsize / 2.0f;
        float z2 = zsize / 2.0f;
        float iFactor = (float)zsize / zdivs;
        float jFactor = (float)xsize / xdivs;
        float texi = smax / zdivs;
        float texj = tmax / xdivs;
        float x, z;
        int vidx = 0, tidx = 0;
        for (int i = 0; i <= zdivs; i++)
        {
            z = iFactor * i - z2;
            for (int j = 0; j <= xdivs; j++)
            {
                x = jFactor * j - x2;
                p[vidx] = x;
                p[vidx + 1] = 0.0f;
                p[vidx + 2] = z;
                n[vidx] = 0.0f;
                n[vidx + 1] = 1.0f;
                n[vidx + 2] = 0.0f;

                tex[tidx] = j * texi;
                tex[tidx + 1] = i * texj;

                vidx += 3;
                tidx += 2;
            }
        }

        GLuint rowStart, nextRowStart;
        int idx = 0;
        for (int i = 0; i < zdivs; i++)
        {
            rowStart = (GLuint)(i * (xdivs + 1));
            nextRowStart = (GLuint)((i + 1) * (xdivs + 1));
            for (int j = 0; j < xdivs; j++)
            {
                el[idx] = rowStart + j;
                el[idx + 1] = nextRowStart + j;
                el[idx + 2] = nextRowStart + j + 1;
                el[idx + 3] = rowStart + j;
                el[idx + 4] = nextRowStart + j + 1;
                el[idx + 5] = rowStart + j + 1;
                idx += 6;
            }
        }

        initBuffers(&el, &p, &n, &tex);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // std::cout << "width: " << width << "\nheight" << height << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << tex_path << std::endl;
        SOIL_free_image_data(image);
    }
}

void Plane::render(GLSLProgram *shader, glm::mat4 view, glm::mat4 proj)
{
    if (vao == 0)
        return;

    glBindVertexArray(vao);

    shader->setUniform("Kd", 0.2f, 0.2f, 0.2f);
    shader->setUniform("Ks", 0.9f, 0.9f, 0.9f);
    shader->setUniform("Ka", 0.1f, 0.1f, 0.1f);
    shader->setUniform("Shininess", 180.0f);

    glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mv = view * model;

    shader->setUniform("ModelViewMatrix", mv);
    shader->setUniform("NormalMatrix",
                    glm::mat3( glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]) ));
    shader->setUniform("MVP", proj * mv);

    glDrawElements(GL_TRIANGLES, nVerts, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
