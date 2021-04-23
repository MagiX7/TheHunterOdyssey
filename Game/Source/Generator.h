#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include"Particle.h"
#include"External/SDL_image/include/SDL_image.h"
#include"List.h"
#define pixelsToMeters(x) x/30
#define metersToPixels(x) x*30
enum class ParticleType {
	DUST,
	NONE
};
enum class GeneratorState {
	STARTING,
	NORMAL,
	ENDING,
	STOP,
	DISABLE
};
class Generator {
public:
	Generator(ParticleType Type);
	virtual ~Generator();
	void Start();
	GeneratorState GetState();
	void Stop();
	void Restart();
	void SetGoal(Point<int> Goal);
	void SetPosition(Point<int> Position);
	void SetParameters(Point<int> Rang);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool IfHasFinished(Point<float> positionA, Point<int> positionB);
	Point<float> Integrator(Point<float>*Velocity, float dt, Point<float>Acceleration);
	void CleanUp();
private:
	int maxParticles;
	List<Particle*>particleList;
	ParticleType type;
	GeneratorState state;
	Point<int> rang;
	Point<int> temporalGoal;
	Point<int> temporalPosition;
	int timeCounter;
	SDL_Texture* texture;
};


#endif
