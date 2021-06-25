#pragma once

struct PhongLight
{
    glm::vec3 Position;
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Color;
};


/* Macro definition for direct loading */

/*
#define allLights {        \
PhongLight{                \
 {1.2f, 5.f, -2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 },                        \
PhongLight{                \
 {1.2f, 5.f, -2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 },                        \
PhongLight{                \
 {1.2f, 5.f, -2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 },                        \
PhongLight{                \
 {1.2f, 5.f, -2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 }                         \
}                          \                           
*/


#define allLights {        \
PhongLight{                \
 {1.2f, 5.f, -2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {150.f, 150.f, 150.f},    \
 },                        \
PhongLight{                \
 {5.2f, 5.f,  2.0f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {150.f, 150.f, 150.f},    \
 },                        \
PhongLight{                \
 {-51.2f, 15.f, -20.0f},   \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 },                        \
PhongLight{                \
 {1.2f, 50.f, -2.0f},      \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {150.f, 150.f, 150.f},    \
 }                         \
}                          \
