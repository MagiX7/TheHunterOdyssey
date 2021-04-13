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

	Entity* CreateEntity(EntityType type, iPoint position, pugi::xml_node anim, int id= -1);

	void DeleteEntity(Entity* entity);

	void DeleteAllEntities();

	bool LoadState(pugi::xml_node* toLoad);

	bool SaveState(pugi::xml_node* toSave);

	bool Load();

	bool CheckEntityColision(SceneGameplay* scene);

	void SetAllNpcActive();

	void SetAllNpcInactive();

	void DeleteAllNpcActive();
public:
	//List<Entity*> entities;
	eastl::list<Entity*> entities;
};

#endif //__ENTITYMANAGER_H__