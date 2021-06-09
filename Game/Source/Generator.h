#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include"Particle.h"
#include"External/SDL_image/include/SDL_image.h"
#include"List.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#define pixelsToMeters(x) x/30
#define metersToPixels(x) x*30
enum class ParticleType {
	DUST,
	FIRE,
	MAGIC,
	GRAVITY,
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
	void SetGeneratorState(GeneratorState State);
	void SetParameters(Point<int> Rang);
	void SetParticlesDesactivated();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool IfHasFinished(Point<float> positionA, Point<int> positionB);
	Point<float> Integrator(Point<float>*Velocity, float dt, Point<float>Acceleration);
	void CleanUp();
	void setGeneratorLive(float num);
	float getCurrentLive();
	void sumGeneratorLive(float num);
	void setGeneratorMaxLive(float num);
	float getMaxLive();
private:
	int maxParticles;
	List<Particle*>particleList;
	ParticleType type;
	GeneratorState state;
	Point<int> rang;
	Point<int> temporalGoal;
	Point<int> temporalPosition;
	float generatorLive;
	float maxGeneratorLive;
	float timeCounter;
	SDL_Texture* texture;
	bool startFireLoop;
};


#endif
