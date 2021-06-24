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
        inline static glm::vec3 position{14.f, -5.5f, 3.5f};
        inline static glm::vec3 speed{0.f};
        inline static float scale = 1.f;
        inline static float frictionFactor = -0.2f;
        inline static float shootSpeed     = 10.f;
        inline static float timeout        = 0.2f;
        inline static float angle          = 5.f;

        // Material
        inline static glm::vec3 material_ambient{1.f, 1.f, 1.f};
        inline static glm::vec3 material_diffuse{0.8f, 0.8f, 0.8f};
        inline static glm::vec3 material_specular{0.5f, 0.5f, 0.5f};
        inline static float material_shininess = 255.f;
    };
} // namespace Entities
