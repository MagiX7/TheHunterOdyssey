#include "EntityManager.h"
#include"NpcWizard.h"
#include"Tabern.h"
#include"Town.h"
#include"Wizard.h"
#include"Hunter.h"
#include"Thief.h"
#include "Npc.h"
#include"player.h"
#include"Warrior.h"
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
	case EntityType::HUNTER:

		entity = new Hunter(pos);

		break;
	case EntityType::WIZARD:

		entity = new Wizard(pos);
		break;
	case EntityType::NPC_WIZARD:

		entity = new NpcWizard(pos);

		break;
	case EntityType::TABERN:

		entity = new Tabern(pos);

		break;
	case EntityType::TOWN:

		entity = new Town(pos);

		break;
	case EntityType::WARRIOR:

		entity = new Warrior(pos);

		break;
	case EntityType::THIEF:

		entity = new Thief(pos);

		break;
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
void EntityManager::DeleteAllEntities() 
{

	LOG("Deleting All Entities");
	eastl::list<Entity*>::iterator item;

	for (item = entities.begin(); item != entities.end(); ++item)
	{
		entities.remove(*item);

		
	}

}
bool EntityManager::LoadState(pugi::xml_node* toLoad)
{
	DeleteAllEntities();



	int amount = toLoad->attribute("amount").as_int();
	int npcAmount = toLoad->child("NPCs").attribute("amount").as_int();
	int playerAmount = toLoad->child("players").attribute("amount").as_int();
	pugi::xml_node NodeNpc = toLoad->child("NPCs");
	pugi::xml_node NodePlayer = toLoad->child("players");
	pugi::xml_node NodePlayerAuxiliar = NodePlayer.child("player");
	pugi::xml_node NodeNpcAuxiliar = NodeNpc.child("NPC");
	Npc* npcNode = nullptr;

	for (int a = 0; a < (int)EntityType::UNKNOWN; a++)
	{
		switch (a)
		{
		case (int)EntityType::NPC:
			for (int a = 0; a < npcAmount; a++) {
				npcNode = nullptr;
				SString string;
				string = NodeNpcAuxiliar.child("NpcType").attribute("type").as_string();
				EntityType npcType;
				if (string == "TABERN") {
					npcType = EntityType::TABERN;
				}
				else if (string == "TOWN") {
					npcType = EntityType::TOWN;
				}
				else if (string == "NPC_WIZARD") {
					npcType = EntityType::NPC_WIZARD;
				}
				else {
					npcType = EntityType::UNKNOWN;
				}


				npcNode = (Npc*)CreateEntity(npcType, { NodeNpcAuxiliar.child("bounds").attribute("X").as_int(),NodeNpcAuxiliar.child("bounds").attribute("Y").as_int() });
				NodeNpcAuxiliar = NodeNpcAuxiliar.next_sibling();
			}
			break;
		case (int)EntityType::PLAYER:

			for (int a = 0; a < playerAmount; a++) {
				SString string;
				string = NodePlayerAuxiliar.child("playerType").attribute("type").as_string();
				EntityType plType;

				if (string == "HUNTER") { plType = EntityType::HUNTER; }
				else if (string == "WIZARD") { plType = EntityType::WIZARD; }
				else if (string == "WARRIOR") { plType = EntityType::WARRIOR; }
				else if (string == "THIEF") { plType = EntityType::THIEF; }
				else { plType = EntityType::UNKNOWN; }

				Player* player = (Player*)CreateEntity(plType, { NodePlayerAuxiliar.child("bounds").attribute("X").as_int(),NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() });
				NodePlayerAuxiliar = NodePlayerAuxiliar.next_sibling();
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
	pugi::xml_node NodeNpcAuxiliar;
	pugi::xml_node NodePlayers = toSave->append_child("players");
	pugi::xml_node NodePlayersAuxiliar;

	int npcAmount = 0;
	int playerAmount = 0;

	for (item = entities.begin(); item != entities.end(); ++item)
	{
		switch (item.mpNode->mValue->type)
		{
		case EntityType::NPC_WIZARD:
			npcAmount++;
			break;
		case EntityType::TABERN:
			npcAmount++;
			break;
		case EntityType::TOWN:
			npcAmount++;
			break;
		case EntityType::HUNTER:
			playerAmount++;
			break;
		case EntityType::WIZARD:
			playerAmount++;
			break;
		case EntityType::THIEF:
			playerAmount++;
			break;
		case EntityType::WARRIOR:
			playerAmount++;
			break;
		}

	}

	NodeNpc.append_attribute("amount").set_value(npcAmount);
	NodePlayers.append_attribute("amount").set_value(playerAmount);

	for (item = entities.begin(); item != entities.end(); ++item)
	{

		switch (item.mpNode->mValue->type)
		{
		case EntityType::NPC_WIZARD:

			NodeNpcAuxiliar = NodeNpc.append_child("NPC");
			item.mpNode->mValue->SaveState(NodeNpcAuxiliar);

			break;
		case EntityType::TABERN:

			NodeNpcAuxiliar = NodeNpc.append_child("NPC");
			item.mpNode->mValue->SaveState(NodeNpcAuxiliar);

			break;
		case EntityType::TOWN:

			NodeNpcAuxiliar = NodeNpc.append_child("NPC");
			item.mpNode->mValue->SaveState(NodeNpcAuxiliar);

			break;
		case EntityType::HUNTER:

			NodePlayersAuxiliar = NodePlayers.append_child("player");
			item.mpNode->mValue->SaveState(NodePlayersAuxiliar);

			break;
		case EntityType::WIZARD:

			NodePlayersAuxiliar = NodePlayers.append_child("player");
			item.mpNode->mValue->SaveState(NodePlayersAuxiliar);

			break;
		case EntityType::WARRIOR:

			NodePlayersAuxiliar = NodePlayers.append_child("player");
			item.mpNode->mValue->SaveState(NodePlayersAuxiliar);

			break;
		case EntityType::THIEF:

			NodePlayersAuxiliar = NodePlayers.append_child("player");
			item.mpNode->mValue->SaveState(NodePlayersAuxiliar);

			break;
		}
	}
	return true;
}