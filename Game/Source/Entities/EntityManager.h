#pragma once

#include <precomp.h>

#include <Entities/Entity.h>

namespace Entities {
	
	class EntityManager
	{
	public:
		~EntityManager();

		auto Init() -> bool;
		
		void Update(float delta);
		auto LoadEntity(const std::string& name, std::unique_ptr<Entity>&& entity) -> bool;

	private:
		std::unordered_map<std::string, std::unique_ptr<Entity>> m_Table;
		
	};

}