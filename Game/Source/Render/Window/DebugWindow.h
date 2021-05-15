#pragma once

namespace Entities {
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
        EntMan* m_EntityManager;
        void m_DrawLightMenu()   const;
        void m_DrawGFXSettings() const;
        void m_DrawBallMenu()    const;
#endif

        bool m_DebugMenuActive = true;
        bool m_ViewportsEnable = false;
        
    };
} // namespace Window
