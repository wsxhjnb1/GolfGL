#pragma once

#ifdef _DEBUG
#include <Window/DebugWindow.h>
#endif

namespace Entities {

    class ballDefault
    {
        #ifdef _DEBUG
        friend Window::DebugWindow;
        #endif
        friend class Ball;
        
        inline static const float speed = 2.f;
        inline static const float scalarFixer = 0.006f;
        inline static const float timeout = 0.2f;
        inline static const float angle = 5.f;
        inline static const glm::vec3 direction{ CAMERA.GetCameraFront() };
        inline static const glm::vec3 position{ 0.f, 0.f, 0.f };

        
    };
}