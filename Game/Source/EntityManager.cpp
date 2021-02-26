#include "EntityManager.h"

#include "Log.h"

EntityManager::EntityManager()
{
	
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Update(float dt)
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;
	//eastl::list<Entity*>::value_type item = entities.begin().mpNode;
	
	// Update Logic
	while (item)
	{
		item->data->Update(dt);
		item = item->next;
	}

	return ret;
}

void EntityManager::Draw()
{
	ListItem<Entity*>* item = entities.start;
	//eastl::list<Entity*>::value_type item = entities.begin().mpNode;

	while (item)
	{
		item->data->Draw();
		item = item->next;
	}
}

bool EntityManager::UnLoad()
{
	LOG("Unloading Entities");
	bool ret = true;

	ListItem<Entity*>* item = entities.start;
	//eastl::list<Entity*>::value_type item = entities.begin().mpNode;

	while (item)
	{
		item->data->UnLoad();
		item = item->next;
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
	if (entity != nullptr) entities.Add(entity); // Should player be on the list?

	return entity;
}

void EntityManager::DeleteEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.start;
	//eastl::list<Entity*>::value_type item = entities.begin().mpNode;

	while (item)
	{
		if (item->data == entity)
		{
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}