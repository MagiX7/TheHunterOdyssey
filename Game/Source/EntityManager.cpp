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
	bool ret = true;

	//ListItem<Entity*>* item = entities.start;
	eastl::list<Entity*>::iterator item = entities.begin();
	
	// Update Logic
	while (item.mpNode)
	{
		item.mpNode->mValue->Update(dt);
		item = item.mpNode->mpNext;
	}

	return ret;
}

void EntityManager::Draw()
{
	//ListItem<Entity*>* item = entities.start;
	eastl::list<Entity*>::iterator item = entities.begin();

	while (item.mpNode)
	{
		item.mpNode->mValue->Draw();
		item = item.mpNode->mpNext;
	}
}

bool EntityManager::UnLoad()
{
	LOG("Unloading Entities");
	bool ret = true;

	//ListItem<Entity*>* item = entities.start;
	eastl::list<Entity*>::iterator item = entities.begin();

	while (item.mpNode)
	{
		item.mpNode->mValue->UnLoad();
		item = item.mpNode->mpNext;
	}
	
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
	//ListItem<Entity*>* item = entities.start;
	eastl::list<Entity*>::iterator item = entities.begin();

	while (item.mpNode)
	{
		if (item.mpNode->mValue == entity)
		{
			entities.remove(item.mpNode->mValue);
			break;
		}
		item = item.mpNode->mpNext;
	}
}