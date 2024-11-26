#include <precomp.h>

#include "EntityManager.h"

#include "Ball/Ball.h"
#include "Skybox/Skybox.h"
#include "Terrain/Terrain.h"

Entities::EntityManager::~EntityManager() { m_Table.clear(); }

bool Entities::EntityManager::Init()
{
    return LoadEntity("skybox", std::make_unique<Skybox>())
        && LoadEntity("terrain", std::make_unique<Terrain>())
        && LoadEntity("ball", std::make_unique<Ball>())
    ;
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
    // std::cout << name << "loaded\n";
    return true;
}

Entities::Entity *Entities::EntityManager::GetEntity(const std::string &name) { return m_Table[name].get(); }

void Entities::EntityManager::m_UpdateTransformations(float delta)
{
    auto *terrain = static_cast<Terrain *>(m_Table["terrain"].get());
    auto *ball    = static_cast<Ball *>(m_Table["ball"].get());


    glm::vec3 friction{ball->m_frictionFactor * ball->m_speed};
    auto gradient{ terrain->GetGradient(ball->position.x, ball->position.z) };
    
    ball->m_acceleration = gradient + friction;
    ball->m_speed += 0.5f * delta * ball->m_acceleration;

    if (glm::length(ball->m_speed) < 0.2f)
    {
        ball->m_speed = glm::vec3{0.f};        
    }

    ball->position += delta * ball->m_speed;
    
    ball->position.y = ball->m_scale * 4.f + terrain->GetHeight(ball->position.x, ball->position.z);    
    terrain->CorrectPosition(ball->position.x, ball->position.z);
}

void Entities::EntityManager::Update(float delta)
{
    m_UpdateTransformations(delta);

    Entity::view = CAMERA.LookAt();

    m_Table["ball"]->Update(delta);
    m_Table["terrain"]->Update(delta);
    m_Table["skybox"]->Update(delta);
}

// the function to get the golf ball position

glm::vec3 Entities::EntityManager::GetGolfBallPosition() const
{
    // get the iterator to the golf ball entity
    auto it = m_Table.find("ball");
    if (it != m_Table.end())
    {
        const Ball* ball = dynamic_cast<const Ball*>(it->second.get());
        if (ball)
        {
            return ball->position; // return the golf ball position
        }
    }
    // if the golf ball entity is not found, return the zero vector
    return glm::vec3(0.0f);
}

