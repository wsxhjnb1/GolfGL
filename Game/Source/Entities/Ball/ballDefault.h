#pragma once

#ifdef _DEBUG
#include <Window/DebugWindow.h>
#endif

namespace Entities
{
    class ballDefault
    {
#ifdef _DEBUG
        friend Window::DebugWindow;
#endif
        friend class Ball;

        inline static float speed               = 10.f;
        inline static float accel               = 2.f;
        inline static float rotationFixer       = 40.f;
        inline static float timeout             = 0.2f;
        inline static float angle               = 5.f;        
        inline static glm::vec3 position{0.f, 0.f, 0.f};
        inline static glm::vec3 material_ambient{1.f, 1.f, 1.f};
        inline static glm::vec3 material_diffuse{0.8f, 0.8f, 0.8f};
        inline static glm::vec3 material_specular{0.5f, 0.5f, 0.5f};
        inline static float material_shininess = 255.f;
    };
} // namespace Entities
