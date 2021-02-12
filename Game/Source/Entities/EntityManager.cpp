#include <precomp.h>

#include "EntityManager.h"

#include "Skybox/Skybox.h"
#include "Floor.h";
#include "Ball/Ball.h"

Entities::EntityManager::~EntityManager()
{
	m_Table.clear();
}

bool Entities::EntityManager::Init()
{
	// Loading initial entities	
	if( LoadEntity("skybox", std::make_unique<Skybox>()) 

	 &&	LoadEntity("floor",  std::make_unique<Floor>())  

	 && LoadEntity("ball",	 std::make_unique<Ball>()) 	 
		)
	return true;


	return false;
}

// void Entities::EntityManager::LoadEntity(const std::string& name, std::unique_ptr<Entity> entity)
bool Entities::EntityManager::LoadEntity(const std::string& name, std::unique_ptr<Entity>&& entity)
{	
	m_Table[name] = std::move( entity );

	if (m_Table[name] == nullptr)
	{
		LOG_FATAL("Entity manager failed to load {}", name);
		return false;
	}

	LOG_INFO("{} loaded", name);
	return true;
}

void Entities::EntityManager::Update(float delta)
{
	std::for_each(m_Table.begin(), m_Table.end()
		, [=](std::pair<const std::string, std::unique_ptr<Entity>>& p) {				

			p.second->Update(delta);			

		});
}
