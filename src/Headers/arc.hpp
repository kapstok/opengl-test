#pragma once

#include "glitter.hpp"
#include "cube.hpp"
#include <vector>

std::vector<Face> faceToArc(
    glm::vec3 startA, glm::vec3 endA,
    glm::vec3 startB, glm::vec3 endB,
    float interval, float offset
);

glm::vec2 getArcPosition(
    glm::vec2 startArc, glm::vec2 endArc,
    float interval, float offset
);