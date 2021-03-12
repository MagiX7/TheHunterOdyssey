#include "EntityManager.h"

#include "Log.h"
#include "EASTL/fixed_allocator.h"
#include "EASTL/iterator.h"

EntityManager::EntityManager()
{
	
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Update(float dt)
{
	LOG("Updating Entities");
	bool ret = true;
	
	// Update Logic
	eastl::list<Entity*>::iterator item;
	
	for (item = entities.begin(); item != entities.end(); ++item)
		(*item)->Update(dt);

	return ret;
}

void EntityManager::Draw()
{
	eastl::list<Entity*>::iterator item;
	
	for (item = entities.begin(); item != entities.end(); ++item)
		(*item)->Draw();
}

bool EntityManager::UnLoad()
{
	LOG("Unloading Entities");
	bool ret = true;

	eastl::list<Entity*>::iterator item = entities.begin();

	for (item = entities.begin(); item != entities.end(); ++item)
		(*item)->UnLoad();
	
	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint pos)
{
	LOG("Creating %s", type);
	Entity* entity = nullptr;

	switch (type)
	{
	/*case EntityType::PLAYER: break;*/
	case EntityType::ITEM: break;
	case EntityType::ENEMY: break;
	}

	// Created entities are added to the list
	if (entity != nullptr) entities.push_back(entity); // Should player be on the list?

	return entity;
}

void EntityManager::DeleteEntity(Entity* entity)
{
	LOG("Deleting Entity");
	eastl::list<Entity*>::iterator item;

	for (item = entities.begin(); item != entities.end(); ++item)
	{
		if (*item == entity)
		{
			entities.remove(*item);
			break;
		}
	}
}