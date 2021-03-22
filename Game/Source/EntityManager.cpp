#include "EntityManager.h"

#include "Npc.h"
#include"player.h"
#include "Log.h"
#include "EASTL/fixed_allocator.h"
#include "EASTL/iterator.h"

EntityManager::EntityManager()
{
	int a = 0;
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

bool EntityManager::Load()
{
	LOG("Updating Entities");
	bool ret = true;

	// Update Logic
	eastl::list<Entity*>::iterator item;

	for (item = entities.begin(); item != entities.end(); ++item)
		(*item)->Load();

	return ret;
}

void EntityManager::Draw(bool showColliders)
{
	eastl::list<Entity*>::iterator item;
	
	for (item = entities.begin(); item != entities.end(); ++item)
		(*item)->Draw(showColliders);
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
	/*LOG("Creating %s", type);*/
	Entity* entity = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:entity = new Player(PlayerType::HUNTER); break;
	case EntityType::ITEM: break;
	case EntityType::ENEMY: break;
	case EntityType::NPC: entity = new Npc(EntityType::NPC); break;
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

bool EntityManager::LoadState(pugi::xml_node* toLoad)
{
	/*eastl::list<Entity*>::iterator item;
	for (item = entities.begin(); item != entities.end(); ++item)
	{
		item.mpNode->mValue->LoadState();
	}*/
	return true;
}

bool EntityManager::SaveState(pugi::xml_node* toSave)
{
	eastl::list<Entity*>::iterator item;
	pugi::xml_node Node;
	for (item = entities.begin(); item != entities.end(); ++item)
	{
		
		switch (item.mpNode->mValue->type)
		{
		case EntityType::NPC:
			Node = toSave->append_child("NPC");

			item.mpNode->mValue->SaveState(Node);
			break;
		case EntityType::PLAYER:
			Node = toSave->append_child("player");
			item.mpNode->mValue->SaveState(Node);
			break;

		}
	}
	return true;
}