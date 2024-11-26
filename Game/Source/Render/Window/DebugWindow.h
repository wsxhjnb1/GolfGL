#pragma once

namespace Entities {
    class Entity;
    class EntityManager;
}
using EntMan = Entities::EntityManager;

namespace Window
{
    /* Wrapper for ImGui window */
    class DebugWindow
    {
    public:
        bool Init(EntMan* entityManager = nullptr);
        bool Update();

        void Destroy();

        DebugWindow()  = default;
        ~DebugWindow() = default;
        

    private:
#ifdef _DEBUG
        
        void m_DrawGFXSettings() const;
        void m_DrawCameraMenu()  const;
        void m_DrawLightMenu()   const;    
#endif
        EntMan* m_EntityManager;
        bool m_DebugMenuActive = true;
        bool m_ViewportsEnable = false;
        void m_DrawCameraMenu();
        void m_DrawBallMenu();
        
    };
} // namespace Window
