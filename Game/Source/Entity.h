#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

#include "Animation.h"
#include"SceneGameplay.h"
#include"Scene.h"

enum class EntityState
{
	WALLKING_LEFT,
	WALLKING_RIGHT,
	WALLKING_UP,
	WALLKING_DOWN,
	STOP_LEFT,
	STOP_RIGHT,
	STOP_UP,
	STOP_DOWN,
	TALKING,
	INACTIVE
};
struct SDL_Texture;

enum class EntityType
{
	PLAYER,
	HUNTER,
	WIZARD,
	THIEF,
	WARRIOR,
	ENEMY,
	GOLEM,
	SKULL,
	BAT,
	NPC,
	TABERN,
	NPC_WIZARD,
	TOWN,
	RAY,
	ITEM,
	MAP,
	UNKNOWN
};

class Entity
{
public:
	Entity(EntityType type) : type(type) {}

	virtual ~Entity(){}

	virtual bool Load() { return true; }
	
	virtual bool Update(float dt) { return true; }

	virtual bool CheckCollisions() { return true; }
	
	virtual void Draw(bool showColliders) {}

	virtual bool UnLoad() { return true; }

	virtual bool LoadState(pugi::xml_node& node) { return true; }

	virtual bool SaveState(pugi::xml_node& node){return true;} 
	
	virtual void onCollision() {}

	virtual void setInactive(){}

	virtual void setActive() {}

	virtual EntityState getState() { return EntityState::STOP_DOWN; }
public:
	SDL_Rect bounds;
	SString name;
	EntityType type;
	SDL_Texture* texture;
	//SceneGameplay* scene;

	Animation* currentAnim;
};

#endif //__ENTITY_H__