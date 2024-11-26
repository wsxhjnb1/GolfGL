#pragma once

#include <Entities/Entity.h>

namespace Entities
{
    class EntityManager
    {
    public:
        ~EntityManager();

        bool Init();

        void Update(float delta);
        bool LoadEntity(const std::string &name, std::unique_ptr<Entity> &&entity);
        Entity* GetEntity(const std::string &name);

        // additional functions for getting the golf ball position
        glm::vec3 GetGolfBallPosition() const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Entity>> m_Table;
        void m_UpdateTransformations(float delta);
    #ifdef _DEBUG
        friend class Window::DebugWindow;
        inline static bool ms_SkipTransform = false;
    #endif
    };
} // namespace Entities
