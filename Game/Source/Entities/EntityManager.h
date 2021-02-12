#pragma once

#include <precomp.h>

#include <Entities/Entity.h>

namespace Entities {
	
	class EntityManager
	{
	public:
		~EntityManager();

		bool Init();
		
		void Update(float delta);
		bool LoadEntity(const std::string& name, std::unique_ptr<Entity>&& entity);

	private:
		std::unordered_map<std::string, std::unique_ptr<Entity>> m_Table;
		
	};

}