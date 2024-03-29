#ifndef _PARTICLESMANAGER_H_
#define _PARTICLESMANAGER_H_
#include"List.h"
#include"Generator.h"

class ParticlesManager
{
public:
	ParticlesManager();
	virtual ~ParticlesManager();
	Generator* CreateGenerator(Point<int> position, ParticleType type);
	void StartSimulation(Generator* generator);
	void setGeneratorPosition(int num, iPoint Position);
	void Stop(Generator* generator);
	void Restart(Generator* generator);
	void UpdateGeneratorGoal(Generator* generator,Point<int> Goal);
	void UpdateGeneratorPosition(Generator* generator, Point<int> Goal);
	void DeleteGenerator(Generator* generator);
	void SetAllParticlesDesactivated();
	void setGeneratorState(int num, GeneratorState state);
	void CleanUp();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
private:
	List<Generator*> generatorList;
};



#endif
