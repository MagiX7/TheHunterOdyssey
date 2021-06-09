#include"Entity.h"
Entity::Entity() {}
Entity::Entity(EntityType type) : type(type) {}

Entity::~Entity() {}
bool Entity::Load() { return true; }

bool Entity::Update(float dt) { return true; }

bool Entity::CheckCollisions() { return true; }

void Entity::Draw(bool showColliders) {}

bool Entity::UnLoad() { return true; }

bool Entity::LoadState(pugi::xml_node& node) { return true; }

bool Entity::SaveState(pugi::xml_node& node) { return true; }
void Entity::OnCollision() {}

void Entity::SetInactive() {}

void Entity::SetActive() {}

void Entity::SetState(EntityState myState) { state = myState; }
void Entity::SetBounds(SDL_Rect myBounds) {}

EntityState Entity::GetState() { return EntityState::STOP_DOWN; }

bool Entity::CheckCollision(Player* player) { return true; }

int const Entity::GetDialogeId() const { return -1; }
int Entity::getParticlesNum() {
	return particleNum;
}
void Entity::SetDrawPtext(bool DrawPtext) {}
void Entity::SetTalkStart(bool TalkStart) {}
void Entity::generatorList(ParticlesManager* Fire, int num) {
	particlesBattle = Fire;
	particleNum = num;
}