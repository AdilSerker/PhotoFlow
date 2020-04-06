#ifndef PLANE_H
#define PLANE_H

#include "TriangleMesh.h"
#include <string.h>

class Plane : public TriangleMesh
{
  public:
    glm::vec3 position;

    Plane(std::string tex_path);
    void render(GLSLProgram *shader, glm::mat4 view, glm::mat4 proj);
};

#endif //PLANE_H
