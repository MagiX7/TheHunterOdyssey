#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

#include "Animation.h"

class Player;
struct SDL_Texture;

enum class EntityState
{
	WALLKING_LEFT,
	WALKING_RIGHT,
	WALLKING_UP,
	WALLKING_DOWN,
	STOP_LEFT,
	STOP_RIGHT,
	STOP_UP,
	STOP_DOWN,
	TALKING,
	INACTIVE
};


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
	ARMOR,
	MAP,
	ICE_BLOCK,
	DOOR,
	STATUE,
	UNKNOWN
};

class Entity
{
public:
	Entity() {}
	Entity(EntityType type) : type(type) {}

	virtual ~Entity(){}

	virtual bool Load() { return true; }
	
	virtual bool Update(float dt) { return true; }

	virtual bool CheckCollisions() { return true; }
	
	virtual void Draw(bool showColliders) {}

	virtual bool UnLoad() { return true; }

	virtual bool LoadState(pugi::xml_node& node) { return true; }

	virtual bool SaveState(pugi::xml_node& node) { return true; } 
	
	virtual void OnCollision() {}

	virtual void SetInactive(){}

	virtual void SetActive() {}

	virtual inline void SetState(EntityState myState) { state = myState; }
	virtual void SetBounds(SDL_Rect myBounds) {}

	virtual EntityState GetState() { return EntityState::STOP_DOWN; }

	virtual bool CheckCollision(Player* player) { return true; }

	virtual inline int const GetDialogeId() const { return -1; }

	virtual void SetDrawPtext(bool DrawPtext) {}
	virtual void SetTalkStart(bool TalkStart) {}
	
public:
	SDL_Rect bounds;
	SString name;
	EntityType type;
	EntityState state;
	SDL_Texture* texture;

	Animation* currentAnim;
	int channel;
	int id;
};

#endif //__ENTITY_H__