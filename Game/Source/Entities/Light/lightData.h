#pragma once
struct PhongLight;

#define FLASHLIGHT  	   \
PhongLight{                \
 {14.f, 0.f, 0.f},         \
 {0.1f, 0.1f, 0.1f},       \
 {0.7f, 0.7f, 0.7f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.65f, 0.65f, 0.65f},    \
 LightType::SPOT,	       \
 }						   \

//-----------------------------

#define allLights {        \
PhongLight{                \
 {4.f, -7.f, -8.f},  	   \
 {0.1f, 0.1f, 0.1f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.1f, 0.1f, 0.1f},       \
 {0.65f, 0.65f, 0.65f},    \
 LightType::DIRECTIONAL    \
 },                        \
PhongLight{                \
 {3.f, -7.f, -8.f},        \
 {0.1f, 0.1f, 0.1f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.65f, 0.65f, 0.65f},    \
 LightType::DISABLED	   \
 },                        \
PhongLight{                \
 {5.2f, 50.f,  2.0f},      \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.65f, 0.65f, 0.65f},    \
 LightType::DISABLED       \
 },                        \
PhongLight{                \
 {-51.2f, 15.f, -20.0f},   \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 LightType::DISABLED       \
 },                        \
 PhongLight{               \
 {-51.2f, 15.f, -20.0f},   \
 {0.3f, 0.3f, 0.3f},       \
 {0.5f, 0.5f, 0.5f},       \
 {0.3f, 0.3f, 0.3f},       \
 {0.3f, 0.3f, 0.3f},       \
 LightType::DISABLED,      \
 {51.2f, -15.f, 20.0f},    \
 15.f					   \
 }                         \
}                          \
