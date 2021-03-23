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
	case EntityType::NPC: entity = new Npc(EntityType::NPC,NpcType::TABERN,pos); break;
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
	UnLoad();

	const char* string = "/";

	int amount=toLoad->attribute("amount").as_int();
	int npcAmount = toLoad->child("NPCs").attribute("amount").as_int();
	int playerAmount=toLoad->child("player").attribute("amount").as_int();
	pugi::xml_node NodeNpc = toLoad->child("NPCs");
	pugi::xml_node Node;
	pugi::xml_node NodeNpcAuxiliar= NodeNpc.child("NPC");
	Npc* npcNode = nullptr;

	for (int a=0;a< (int)EntityType::UNKNOWN;a++)
	{
		switch (a)
		{
		case (int)EntityType::NPC:
			for (int a = 0; a < npcAmount;a++) {
				npcNode = nullptr;

				string = NodeNpcAuxiliar.child("NpcType").attribute("type").as_string();
				NpcType npcType;
				if (string == "TABERN") {
					npcType = NpcType::TABERN;
				}
				else if (string == "TOWN") {
					npcType = NpcType::TOWN;
				}
				else if (string == "WIZARD") {
					npcType = NpcType::WIZARD;
				}
				else {
					npcType = NpcType::NONE;
				}


				npcNode = (Npc*)CreateEntity(EntityType::NPC, { NodeNpcAuxiliar.child("bounds").attribute("X").as_int(),NodeNpcAuxiliar.child("bounds").attribute("Y").as_int() });
				npcNode->setNpcType(npcType);
				NodeNpcAuxiliar = NodeNpcAuxiliar.next_sibling();
			}
			break;
		case (int)EntityType::PLAYER:

			for (int a = 0; a < playerAmount; a++) {
				Node = toLoad->child("player");
				string = Node.child("playerType").attribute("type").as_string();
				PlayerType plType;
				if (string == "HUNTER") { plType = PlayerType::HUNTER; }
				else if (string == "WIZARD") { plType = PlayerType::WIZARD; }
				else { plType = PlayerType::NONE; }

				Player* player = (Player*)CreateEntity(EntityType::PLAYER, { Node.child("bounds").attribute("X").as_int(),Node.child("bounds").attribute("Y").as_int() });
				player->setPlayerType(plType);
			}
			break;

		}
		
	}
	return true;
}

bool EntityManager::SaveState(pugi::xml_node* toSave)
{
	eastl::list<Entity*>::iterator item;
	pugi::xml_node NodeNpc = toSave->append_child("NPCs");
	pugi::xml_node Node;

	int npcAmount=0;
	int playerAmount=0;

	for (item = entities.begin(); item != entities.end(); ++item)
	{
		switch (item.mpNode->mValue->type)
		{
		case EntityType::NPC:
			npcAmount++;
			break;
		case EntityType::PLAYER:
			playerAmount++;
			break;
		}

	}
	NodeNpc.append_attribute("amount").set_value(npcAmount);
	for (item = entities.begin(); item != entities.end(); ++item)
	{
		
		switch (item.mpNode->mValue->type)
		{
		case EntityType::NPC:

			item.mpNode->mValue->SaveState(NodeNpc);
			break;
		case EntityType::PLAYER:
			Node = toSave->append_child("player");
			Node.append_attribute("amount").set_value(playerAmount);
			item.mpNode->mValue->SaveState(Node);
			break;

		}
	}
	return true;
}