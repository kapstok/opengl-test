#include "arc.hpp"

#include <iostream>

#define X 0
#define Y 1
#define Z 2

static float getRadius(glm::vec2 start, glm::vec2 end) {
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;

    deltaX *= deltaX;
    deltaY *= deltaY;

    float diameter = glm::sqrt(deltaX + deltaY);
    float radius = diameter / glm::sqrt(2);
    return radius;
}

static char findConstantAxis(glm::vec3 start, glm::vec3 end) {
    glm::vec3 direction = end - start;
    direction = {
        std::abs(direction.x),
        std::abs(direction.y),
        std::abs(direction.z)
    };
    
    if (direction.x >= direction.y && direction.x >= direction.z) {
        return X;
    } else if (direction.y >= direction.x && direction.y >= direction.z) {
        return Y;
    } else {
        return Z;
    }
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

std::vector<Face> faceToArc(
    glm::vec3 startA, glm::vec3 endA,
    glm::vec3 startB, glm::vec3 endB,
    float interval, float offset
) {
    std::vector<Face> result;
    char axisA = findConstantAxis(startA, startB);
    char axisB = findConstantAxis(startB, endB);

    if (axisA != axisB) {
        std::cout << "Axis A and Axis B not equal!" << std::endl;
        Face face = {
            NULL,
            startA.x, startA.y, startA.z,
            endA.x, endA.y, endA.z,
            startB.x, startB.y, startB.z,
            endB.x, endB.y, endB.z
        };
        result.push_back(face);
        return result;
    }

    glm::vec2 position2D;
    glm::vec3 position3DStart, position3DEnd;
    switch (axisA) {
    case X:
        position2D = getArcPosition(
            {startA.y, startA.z},
            {endA.y, endA.z},
            interval, 1
        );
        position3DStart = {startA.x, position2D.x, position2D.y};
        position3DEnd = {endA.x, position2D.x, position2D.y};
        break;
    case Y:
        position2D = getArcPosition(
            {startA.x, startA.z},
            {endA.x, endA.z},
            interval, 1
        );
        position3DStart = {position2D.x, startA.y, position2D.y};
        position3DEnd = {position2D.x, endA.y, position2D.y};
        break;
    case Z:
        position2D = getArcPosition(
            {startA.x, startA.y},
            {endA.x, endA.y},
            interval, 1
        );
        position3DStart = {position2D.x, position2D.y, startA.z};
        position3DEnd = {position2D.x, position2D.y, endA.z};
        break;
    }

    Face face = {
        NULL,
        startA.x, startA.y, startA.z,
        endA.x, endA.y, endA.z,
        position3DStart.x, position3DStart.y, position3DStart.z,
        position3DEnd.x, position3DEnd.y, position3DEnd.z
    };
    result.push_back(face);

    return result;
}