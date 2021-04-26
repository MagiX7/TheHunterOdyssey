#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include"Point.h"
enum class ParticleState {
	DESACTIVATED,
	STARTING,
	TOELIM
};
class Particle {
public:
	Particle(ParticleState State);
	virtual ~Particle();
	ParticleState GetState();
	void Draw();
	void SetState(ParticleState State);
	void SetPosition(Point<float> Position);
	Point<float> GetPosition();
	Point<int> GetGoal();
	void SetGoal(Point<int> goal);
	void SetCurrentLive(float num);
	Point<float> GetVelocity();
	Point<float>* GetVelocityPointer();
	int GetAngle();
	Point<float> GetAceleration();
	void SetAceleration(Point<float> Aceleration);
	void SetRotation(int Rotation);
	void SetVelocity(Point<float> Velocity);
	void SumPosition(Point<float> sum);
	void SumLive(float num);
	void SetMaxLive(float num);
	bool IsDead();
	int GetAlpha();
	void SetAlpha(int Alpha);
private:
	int alpha;
	Point<float> position;
	Point<float> aceleartion;
	Point<float> velocity;
	int rotation;
	ParticleState state;
	float maxLive;
	float currentLive;
	Point<int> Goal;
};


#endif
