#include "arc.hpp"

#include <iostream>

static float getRadius(glm::vec2 start, glm::vec2 end) {
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;

    deltaX *= deltaX;
    deltaY *= deltaY;

    float diameter = glm::sqrt(deltaX + deltaY);
    float radius = diameter / glm::sqrt(2);
    return radius;
}

glm::vec2 getArcPosition(
    glm::vec2 startArc, glm::vec2 endArc,
    float interval, float offset
) {
    float radius = getRadius(startArc, endArc);
    float theta = glm::radians((90 / interval) * offset);

    glm::vec2 result = {
        startArc.x + radius * glm::cos(theta),
        radius * glm::sin(theta)
    };

    return result;
}

Face faceToArc(
    glm::vec2 start, glm::vec2 end,
    Axis axis,
    float interval, float offset
) {
    glm::vec2 position2D = getArcPosition(start, end, interval, offset);
    Face face;
    switch (axis) {
        case X:
            face = {
                NULL,
                 0.8, start.x, start.y,
                 0.8, position2D.x, position2D.y,
                -0.8, position2D.x, position2D.y,
                -0.8, start.x, start.y
            };
            break;
        case Y:
            break;
        case Z:
            face = {
                NULL,
                start.y,  start.x,  0.8f,	// Front C
                start.y,  start.x,  -0.8f,	// Front B
                position2D.x,  1-position2D.y, -0.8f,	// Top B
                position2D.x,  1-position2D.y,  0.8f	// Top C
            };
            if (offset == 2) {
                face = {
                    NULL,
                    0.973205f, 0.9f,  0.8f,	// Front C
                    0.973205f, 0.9f, -0.8f,	// Front B
                    position2D.x,  1 - position2D.y, -0.8f,	// Top B
                    position2D.x,  1 - position2D.y,  0.8f	// Top C
                };
            }
            break;
    }

    return face;
}