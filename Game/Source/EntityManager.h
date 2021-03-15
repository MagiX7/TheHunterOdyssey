#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"

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

	Entity* CreateEntity(EntityType type, iPoint position);

	void DeleteEntity(Entity* entity);

	bool LoadState();

	bool SaveState() const;

public:
	//List<Entity*> entities;
	eastl::list<Entity*> entities;
};

#endif //__ENTITYMANAGER_H__