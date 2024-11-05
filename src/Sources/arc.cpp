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

static void faceToArc(
    std::vector<Face> &faces,
    glm::vec2 start, glm::vec2 end, glm::vec2 prevPos,
    Axis axis, float interval, float offset
) {
    glm::vec2 position2D = getArcPosition(start, end, interval, offset);
    if (end.y > 0) {
        position2D.y += end.y;
    }
    else {
        position2D.y -= end.y;
        position2D.y *= -1;
    }
    Face face;
    switch (axis) {
    case X:
        face = {
            NULL,
             0.8f, prevPos.x, prevPos.y,
            -0.8f, prevPos.x, prevPos.y,
            -0.8f, position2D.x, position2D.y,
             0.8f, position2D.x, position2D.y
        };
        break;
    case Y:
        face = {
            NULL,
            prevPos.x,  0.8f, prevPos.y,
            prevPos.x, -0.8f, prevPos.y,
            position2D.x, -0.8f, position2D.y,
            position2D.x,  0.8f, position2D.y
        };
        break;
    case Z:
        face = {
            NULL,
            prevPos.x,  prevPos.y,  0.8f,
            prevPos.x,  prevPos.y,  -0.8f,
            position2D.x,  position2D.y, -0.8f,
            position2D.x,  position2D.y,  0.8f
        };
        break;
    }

    faces.push_back(face);

    if (interval > offset) {
        faceToArc(faces, start, end, position2D, axis, interval, offset + 1);
    }
}

std::vector<Face> faceToArc(
    glm::vec2 start, glm::vec2 end,
    Axis axis, float interval
) {
    std::vector<Face> faces;
    // We use end as prevPos, but it could also be {start.y, start.x}
    faceToArc(faces, start, end, end, axis, interval, 1);
    return faces;
}