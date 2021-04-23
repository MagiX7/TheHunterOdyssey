#include"Particle.h"
Particle::Particle(ParticleState State) {
	state = State;
}
Particle::~Particle() {

}
ParticleState Particle::GetState() {
	return state;
}
void Particle::Draw() {

}
int Particle::GetAlpha() {
	return alpha;
}
void Particle::SetAlpha(int Alpha) {
	alpha = Alpha;
}
void Particle::SetCurrentLive(float num) {
	currentLive = num;
}
void Particle::SetState(ParticleState State) {
	state = State;
}
void Particle::SetPosition(Point<float> Position) {
	position.x = Position.x;
	position.y = Position.y;
}
void Particle::SetAceleration(Point<float> Aceleration) {
		aceleartion.x = Aceleration.x;
		aceleartion.y = Aceleration.y;	
	
	
}
void Particle::SetVelocity(Point<float> Velocity) {
	velocity.x = Velocity.x;
	velocity.y = Velocity.y;
}
Point<float> Particle::GetPosition() {
	return position;
}
void Particle::SumPosition(Point<float> sum) {
	position.x += sum.x;
	position.y += sum.y;
	
}
void Particle::SetMaxLive(float num) {
	maxLive = num;
}
void Particle::SumLive(float num) {
	currentLive += num;
}
bool Particle::IsDead() {
	if (currentLive >= maxLive) {
		return true;
	}
	return false;
}
Point<float>* Particle::GetVelocityPointer() {
	return &velocity;
}
Point<float> Particle::GetVelocity() {
	return velocity;
}
Point<float>Particle::GetAceleration() {
	return aceleartion;
}
Point<int> Particle::GetGoal()
{
	return Goal;
}
void Particle::SetGoal(Point<int> goal)
{
	Goal.x = goal.x;
	Goal.y = goal.y;
}
float Particle::GetAngle() {
	return rotation;
}
