#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"
#include "SceneGameplay.h"
#include "Point.h"
#include "Defs.h"
#include "EASTL/list.h"

class EntityManager
{
public:
	EntityManager(QuestManager* quests);

	virtual ~EntityManager();

	bool Update(float dt, Player* currentPlayer, bool& triggerDialogue, int& dialogueId, SceneGameplay* scene);

	void Draw(bool showColliders);

	bool UnLoad();

	Entity* CreateEntity(EntityType type, iPoint position, pugi::xml_node anim, int id= -1);
	Entity* CreateEntity2(EntityType type, iPoint position, Player* player, int id);

	void DeleteEntity(Entity* entity);

	void DeleteEntity(EntityType entity);

	void DeleteDoor(int exId);

	void DeleteAllEntities();

	bool LoadState(pugi::xml_node* toLoad, pugi::xml_node* anims);

	bool SaveState(pugi::xml_node* toSave);

	bool Load();

	bool CheckEntityCollision(Entity* entity, SceneGameplay* scene);

	int TriggerDialogue(bool& triggerDialogue, Entity* item);

	void SetAllNpcActive();

	void SetAllNpcInactive();

	void DeleteAllNpcActive();
public:
	//List<Entity*> entities;
	eastl::list<Entity*> entities;
	QuestManager* questManager;
};

#endif //__ENTITYMANAGER_H__