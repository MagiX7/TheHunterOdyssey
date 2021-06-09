#include"Enemy.h"

Enemy::Enemy(EntityType type) : Entity(type) {}

Enemy::~Enemy() {}

void Enemy:: GetDamage(int dmg) {}

void Enemy:: Attack(Player* player) {}
void Enemy:: Travel(iPoint destination, float dt) {}


bool Enemy::AnimationFinished()
{
	return this->deathAnim.HasFinished();
}

bool Enemy::SaveState(pugi::xml_node& node)
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	auxiliar1 = node.append_child("map_name");
	auxiliar1.append_attribute("value").set_value(mapName.c_str());

	auxiliar1 = node.append_child("enemy_type");
	auxiliar1.append_attribute("type").set_value((int)eType);

	return true;
}

// Setters
void Enemy::SetCurrentState(EnemyState state) { currentState = state; }

void Enemy::SetPos(iPoint pos)
{
	bounds.x = pos.x;
	bounds.y = pos.y;
	battlePos = pos;
}
void Enemy::startPaarticles(iPoint pos, int particle) {
	if (particle == -1) {
		particlesBattle->setGeneratorState(particleNum, GeneratorState::STARTING);
		particlesBattle->setGeneratorPosition(particleNum, pos);
	}
	else {
		particlesBattle->setGeneratorState(particle, GeneratorState::STARTING);
		particlesBattle->setGeneratorPosition(particle, pos);
	}
}
// Getters
EnemyState Enemy::GetCurrentState() const { return currentState; }

int Enemy::GetHealth() const { return health; }

EnemyType Enemy::GetEnemyType() const { return eType; }

int Enemy::GetMaxHealthPoints() const { return maxHealth; }