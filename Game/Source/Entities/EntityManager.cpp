#include <precomp.h>

#include "EntityManager.h"

#include "Ball/Ball.h"
#include "Floor.h"
#include "Skybox/Skybox.h"

Entities::EntityManager::~EntityManager() { m_Table.clear(); }

bool Entities::EntityManager::Init()
{
    return LoadEntity("skybox", std::make_unique<Skybox>()) && LoadEntity("floor", std::make_unique<Floor>()) &&
        LoadEntity("ball", std::make_unique<Ball>());
}

bool Entities::EntityManager::LoadEntity(const std::string &name, std::unique_ptr<Entity> &&entity)
{
    m_Table[name] = std::move(entity);

    if (m_Table[name] == nullptr)
    {
        LOG_FATAL("Entity manager failed to load {}", name);
        return false;
    }

    LOG_INFO("{} loaded", name);
    return true;
}

Entities::Entity* Entities::EntityManager::GetEntity( const std::string& name )
{
	return m_Table[name].get();
}

void Entities::EntityManager::Update(float delta)
{
    Entity::view = CAMERA.LookAt();
    std::for_each(m_Table.begin(), m_Table.end(),
                  [=](std::pair<const std::string, std::unique_ptr<Entity>> &p) { p.second->Update(delta); });
}
