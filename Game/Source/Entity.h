#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"
#include "Animation.h"
#include"ParticlesManager.h"
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
	MERCHANT,
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
	Entity();
	Entity(EntityType type);

	virtual ~Entity();
	virtual bool Load();
	
	virtual bool Update(float dt);

	virtual bool CheckCollisions();
	
	virtual void Draw(bool showColliders);

	virtual bool UnLoad();

	virtual bool LoadState(pugi::xml_node& node);

	virtual bool SaveState(pugi::xml_node& node);
	virtual void OnCollision();

	virtual void SetInactive();

	virtual void SetActive();
	void generatorList(ParticlesManager* Fire, int num);
	virtual inline void SetState(EntityState myState);
	virtual void SetBounds(SDL_Rect myBounds);

	virtual EntityState GetState();

	virtual bool CheckCollision(Player* player);

	virtual inline int const GetDialogeId() const;

	virtual void SetDrawPtext(bool DrawPtext);
	virtual void SetTalkStart(bool TalkStart);
	int getParticlesNum();
public:
	SDL_Rect bounds;
	SString name;
	EntityType type;
	EntityState state;
	SDL_Texture* texture;
	ParticlesManager* particlesBattle;
	Animation* currentAnim;
	int channel;
	int id;
	int particleNum;
};

#endif //__ENTITY_H__