#include "EntityManager.h"

#include "NpcWizard.h"
#include "Tabern.h"
#include "Town.h"
#include "Ray.h"

#include "Hunter.h"
#include "Wizard.h"
#include "Thief.h"
#include "Warrior.h"

#include "Bat.h"
#include "Golem.h"
#include "Skull.h"

#include "IceBlock.h"
#include "Door.h"
#include "Statue.h"

#include "QuestManager.h"

#include "Log.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Load()
{
	LOG("Loading Entities");
	bool ret = true;

	// Update Logic
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
		(*item)->Load();

	return ret;
}

bool EntityManager::Update(float dt, Player* currentPlayer, bool& triggerDialogue, int& dialogueId, SceneGameplay* scene)
{
	//LOG("Updating Entities");
	bool ret = true;
	
	// Update Logic
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();
	
	for (item = entities.begin(); item != itEnd; ++item)
	{
		(*item)->Update(dt);
		CheckEntityCollision(*item, scene);
		if ((*item != nullptr) && ((*item)->type == EntityType::NPC_WIZARD || (*item)->type == EntityType::RAY || (*item)->type == EntityType::TOWN) && (*item)->CheckCollision(currentPlayer))
		{
			dialogueId = TriggerDialogue(triggerDialogue, (*item));
		}
	}

	return ret;
}

void EntityManager::Draw(bool showColliders)
{
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();
	
	for (item = entities.begin(); item != itEnd; ++item)
		(*item)->Draw(showColliders);
}

bool EntityManager::UnLoad()
{
	LOG("Unloading Entities");
	bool ret = true;

	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		(*item)->UnLoad();
		RELEASE((*item));
		entities.erase(item);
	}
	
	return ret;
}

bool EntityManager::CheckEntityCollision(Entity* entity, SceneGameplay* scene)
{
	if (scene->CollisionMapEntity(entity->bounds, entity->type))
	{
		entity->OnCollision();
	}

	return true;
}

int EntityManager::TriggerDialogue(bool& triggerDialogue, Entity* item)
{
	triggerDialogue = true;
	item->SetDrawPtext(false);
	item->SetTalkStart(true);
	QuestManager::GetInstance()->CheckQuests(item);
	return item->GetDialogeId();
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint pos, pugi::xml_node anim, int id)
{
	/*LOG("Creating %s", type);*/
	Entity* entity = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:

		//entity = new Hunter(pos);

		break;
	case EntityType::WIZARD:

		//entity = new Wizard(pos);
		break;
	case EntityType::NPC_WIZARD:
		if (id > -1)
			entity = new NpcWizard(pos, anim, id);
		

		//entity->scene = Scene;
		break;
	case EntityType::TABERN:
		if (id > -1)
			entity = new Tabern(pos, anim, id);
		
		//entity->scene = Scene;
		break;
	case EntityType::BAT:

		//entity = new Bat(pos);

		break;
	case EntityType::GOLEM:

		//entity = new Golem(pos);

		break;
	case EntityType::SKULL:

		//entity = new Skull(pos);

		break;
	case EntityType::TOWN:
		if(id > -1)
			entity = new Town(pos, anim, id);

		//entity->scene = Scene;
		break;
	case EntityType::RAY:
		if (id > -1)
			entity = new Ray(pos, anim, id);

		
		//entity->scene = Scene;
		break;
	case EntityType::WARRIOR:

		//entity = new Warrior(pos);

		break;
	case EntityType::THIEF:

		//entity = new Thief(pos);

		break;
	case EntityType::ITEM: break;
	case EntityType::ENEMY: break;
	}

	// Created entities are added to the list
	if (entity != nullptr) entities.push_back(entity); // Should player be on the list?

	return entity;
}

Entity* EntityManager::CreateEntity2(EntityType type, iPoint pos, Player* player, int id)
{
	/*LOG("Creating %s", type);*/
	Entity* entity = nullptr;

	switch (type)
	{
	case EntityType::ICE_BLOCK:
		entity = new IceBlock(type, pos, player);
		entity->Load();
		break;
	case EntityType::DOOR:
		entity = new Door(type, pos, player, id);
		entity->Load();
		break;
	case EntityType::STATUE:
		entity = new Statue(type, pos, player, id);
		entity->Load();
		break;
	}

	// Created entities are added to the list
	if (entity != nullptr) entities.push_back(entity); // Should player be on the list?

	return entity;
}

void EntityManager::DeleteAllNpcActive()
{
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* entity = (*item);
		if (entity->GetState() != EntityState::INACTIVE) 
		{
			if (entity->type == EntityType::NPC_WIZARD || entity->type == EntityType::RAY ||
				entity->type == EntityType::TOWN || entity->type == EntityType::TABERN)
			{
				entity->UnLoad();
				RELEASE((*item));
				entities.remove(*item);
			}
		}
	}
}

void EntityManager::SetAllNpcInactive()
{
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item) 
	{
		Entity* entity = (*item);
		if (entity->type == EntityType::NPC_WIZARD || entity->type == EntityType::RAY || 
			entity->type == EntityType::TOWN || entity->type == EntityType::TABERN)
		{
			entity->SetInactive();
		}
	}
}

void EntityManager::SetAllNpcActive()
{
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* entity = (*item);
		if (entity->type == EntityType::NPC_WIZARD || entity->type == EntityType::RAY
			|| entity->type == EntityType::TOWN || entity->type == EntityType::TABERN)
		{
			entity->SetActive();
		}
	}
}

void EntityManager::DeleteEntity(Entity* entity)
{
	LOG("Deleting Entity");
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* en = (*item);
		if (en == entity)
		{
			en->UnLoad();
			entities.remove(*item);
			RELEASE(*item);
			break;
		}
	}
}

void EntityManager::DeleteEntity(EntityType entity)
{
	LOG("Deleting Entity");
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* en = (*item);
		if (en->type == entity)
		{
			en->UnLoad();
			entities.remove(*item);
			RELEASE(*item);
		}
	}
}

void EntityManager::DeleteEntity(EntityType entity, int exId)
{
	LOG("Deleting Entity");
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* en = (*item);
		if (en->type == entity && en->id == exId)
		{
			en->UnLoad();
			entities.remove(*item);
			RELEASE(*item);
		}
	}
}

void EntityManager::DeleteAllEntities() 
{
	LOG("Deleting All Entities");
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();

	for (item = entities.begin(); item != itEnd; ++item)
	{
		(*item)->UnLoad();
		entities.remove(*item);
		RELEASE((*item));
	}
}

bool EntityManager::LoadState(pugi::xml_node* toLoad, pugi::xml_node* anims)
{
	//DeleteAllEntities();

	UnLoad();


	int amount = toLoad->attribute("amount").as_int();
	int npcAmount = toLoad->child("NPCs").attribute("amount").as_int();

	pugi::xml_node NodeNpc = toLoad->child("NPCs");

	pugi::xml_node NodeNpcAuxiliar = NodeNpc.child("NPC");
	Npc* npcNode = nullptr;


	for (int a = 0; a < npcAmount; a++)
	{
		npcNode = nullptr;
		SString string;
		string = NodeNpcAuxiliar.child("NpcType").attribute("type").as_string();
		EntityType npcType;
		if (string == "TABERN")
		{
			npcType = EntityType::TABERN;
		}
		else if (string == "TOWN")
		{
			npcType = EntityType::TOWN;
		}
		else if (string == "NPC_WIZARD")
		{
			npcType = EntityType::NPC_WIZARD;
		}
		else if (string == "RAY")
		{
			npcType = EntityType::RAY;
		}
		else
		{
			npcType = EntityType::UNKNOWN;
		}

		npcNode = (Npc*)CreateEntity(npcType, { NodeNpcAuxiliar.child("bounds").attribute("X").as_int(),NodeNpcAuxiliar.child("bounds").attribute("Y").as_int() }, (*anims), NodeNpcAuxiliar.child("Dialoge").attribute("idDialoge").as_int());
		NodeNpcAuxiliar = NodeNpcAuxiliar.next_sibling();
	}



	/*Player* player = (Player*)CreateEntity(plType, { NodePlayerAuxiliar.child("bounds").attribute("X").as_int(),NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() }, anims);
	NodePlayerAuxiliar = NodePlayerAuxiliar.next_sibling();*/
	/*	break;
	case (int)EntityType::PLAYER:
		for (int a = 0; a < playerAmount; a++)
		{

		}
		break;
	}
}*/
	return true;
}

bool EntityManager::SaveState(pugi::xml_node* toSave)
{
	eastl::list<Entity*>::iterator item;
	eastl::list<Entity*>::iterator itEnd = entities.end();
	pugi::xml_node nodeNpc = toSave->append_child("NPCs");
	pugi::xml_node nodeNpcAuxiliar;


	int npcAmount = 0;

	for (item = entities.begin(); item != itEnd; ++item)
	{
		switch ((*item)->type)
		{
		case EntityType::NPC_WIZARD:
			npcAmount++;
			break;
		case EntityType::RAY:
			npcAmount++;
			break;
		case EntityType::TABERN:
			npcAmount++;
			break;
		case EntityType::TOWN:
			npcAmount++;
			break;
		}
	}

	nodeNpc.append_attribute("amount").set_value(npcAmount);


	for (item = entities.begin(); item != itEnd; ++item)
	{
		Entity* entity = (*item);
		switch (entity->type)
		{
		case EntityType::NPC_WIZARD:
			nodeNpcAuxiliar = nodeNpc.append_child("NPC");
			entity->SaveState(nodeNpcAuxiliar);
			break;
		case EntityType::TABERN:
			nodeNpcAuxiliar = nodeNpc.append_child("NPC");
			entity->SaveState(nodeNpcAuxiliar);
			break;
		case EntityType::RAY:
			nodeNpcAuxiliar = nodeNpc.append_child("NPC");
			entity->SaveState(nodeNpcAuxiliar);
			break;
		case EntityType::TOWN:
			nodeNpcAuxiliar = nodeNpc.append_child("NPC");
			entity->SaveState(nodeNpcAuxiliar);
			break;
		}
	}

	return true;
}