#pragma once

#include <precomp.h>

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

    private:
        std::unordered_map<std::string, std::unique_ptr<Entity>> m_Table;
        void m_UpdateTransformations(float delta);
    };
} // namespace Entities
