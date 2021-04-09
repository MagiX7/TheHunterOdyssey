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
	EntityManager();

	virtual ~EntityManager();

	bool Update(float dt);

	void Draw(bool showColliders);

	bool UnLoad();

	Entity* CreateEntity(EntityType type, iPoint position, pugi::xml_node anim);

	void DeleteEntity(Entity* entity);

	void DeleteAllEntities();

	bool LoadState(pugi::xml_node* toLoad);

	bool SaveState(pugi::xml_node* toSave);

	bool Load();

	bool checkEntityColision(SceneGameplay* scene);
public:
	//List<Entity*> entities;
	eastl::list<Entity*> entities;
	eastl::list<Npc*> npcs;
};

#endif //__ENTITYMANAGER_H__