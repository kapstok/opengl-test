#pragma once

#include "glitter.hpp"
#include "cube.hpp"
#include <vector>

enum Axis {X, Y, Z};

Face faceToArc(
    glm::vec2 start, glm::vec2 end,
    Axis axis,
    float interval, float offset
);

glm::vec2 getArcPosition(
    glm::vec2 startArc, glm::vec2 endArc,
    float interval, float offset
);