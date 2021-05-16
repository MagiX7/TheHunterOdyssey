#include"ParticlesManager.h"

ParticlesManager::ParticlesManager() 
{
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
}

ParticlesManager::~ParticlesManager() 
{

}

void ParticlesManager::Stop(Generator* generator)
{

}

void ParticlesManager::Restart(Generator* generator)
{

}

void ParticlesManager::DeleteGenerator(Generator* generator) 
{
	if (generator != nullptr) 
	{
		int gen = generatorList.Find(generator);
		ListItem<Generator*>* auxiliar = generatorList.start;
		for (int a = 0; a < gen; a++) 
		{ 
			auxiliar = auxiliar->next; 
		}
		generatorList.Del(auxiliar);
		generator->CleanUp();
		RELEASE(generator);
	}
}
bool ParticlesManager::PreUpdate(){
	for (ListItem<Generator*>* auxiliar = generatorList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != GeneratorState::DISABLE) 
		{
			if (auxiliar->data->PreUpdate() == false)break;
		}
	}
	return true;
}

void ParticlesManager::SetAllParticlesDesactivated() 
{
	for (ListItem<Generator*>* auxiliar = generatorList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != GeneratorState::DISABLE) 
		{
			auxiliar->data->SetParticlesDesactivated();
		}
	}
}

void ParticlesManager::CleanUp() 
{
	ListItem<Generator*>* auxiliar = generatorList.start;
	ListItem<Generator*>* auxiliar1;
	while (auxiliar != nullptr) 
	{
		auxiliar1 = auxiliar->next;
		auxiliar->data->CleanUp();
		generatorList.Del(auxiliar);
		RELEASE(auxiliar->data);
		auxiliar = auxiliar1;
	}
	generatorList.Clear();
}

bool ParticlesManager::Update(float dt)
{
	for (ListItem<Generator*>* auxiliar = generatorList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != GeneratorState::DISABLE)
		{
			if (auxiliar->data->Update(dt) == false)break;
		}
	}
	return true;
}

bool ParticlesManager::PostUpdate()
{
	for (ListItem<Generator*>* auxiliar = generatorList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != GeneratorState::DISABLE)
		{
			if (auxiliar->data->PostUpdate() == false) break;
		}
	}
	return true;
}

void ParticlesManager::StartSimulation(Generator* generator) 
{
	generator->Start();
}
void ParticlesManager::UpdateGeneratorGoal(Generator* generator, Point<int> Goal) 
{
	generator->SetGoal(Goal);
}
void ParticlesManager::UpdateGeneratorPosition(Generator* generator, Point<int> Position) 
{
	generator->SetPosition(Position);
}

Generator* ParticlesManager::CreateGenerator(Point<int> position, ParticleType type)
{
	Generator* generator = nullptr;
	switch (type)
	{
	case ParticleType::DUST:
		generator = new Generator(type);
		break;
	case ParticleType::NONE:
		break;
	default:
		break;
	}
	generator->SetPosition(position);
	generatorList.Add(generator);
	return generator;
}