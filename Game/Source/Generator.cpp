#include"Generator.h"

#include<time.h>
#include<stdlib.h>

Generator::Generator(ParticleType Type) {
	srand(time(NULL));
	maxParticles = 200;
	timeCounter = 0;
	type = Type;
	switch (type)
	{
	case ParticleType::DUST:
		texture = app->tex->Load("Assets/Textures/Dust/dust_2.png");
		break;
	case ParticleType::NONE:
		break;
	default:
		break;
	}
	state = GeneratorState::NORMAL;
	Particle* particle;
	for (int a = 0; a < maxParticles; a++) {
		particle = new Particle(ParticleState::DESACTIVATED);
		particleList.Add(particle);
	}
}

Generator::~Generator() 
{

}

void Generator::SetParticlesDesactivated() 
{
	for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
		{
				auxiliar->data->SetState(ParticleState::DESACTIVATED);
		}
	}
}

void Generator::Stop() 
{
	state = GeneratorState::STOP;
}

void Generator::Restart()
{
	state = GeneratorState::NORMAL;
}

void Generator::SetPosition(Point<int> Position) 
{
	temporalPosition.x = Position.x;
	temporalPosition.y = Position.y;
}

GeneratorState Generator::GetState() 
{
	return state;
}

void Generator::SetParameters(Point<int> Rang) 
{
	rang = Rang;
}

void Generator::Start() {}

void Generator::CleanUp() 
{
	ListItem<Particle*>* auxiliar1;
	ListItem<Particle*>* auxiliar=particleList.start;
	
	while (auxiliar !=nullptr)
	{
		auxiliar1=auxiliar->next;
		delete auxiliar->data;
		particleList.Del(auxiliar);
		auxiliar = auxiliar1;
	}
	
	particleList.Clear();
	app->tex->UnLoad(texture);
}

bool Generator::PreUpdate()
{
	for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar!= nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != ParticleState::DESACTIVATED) 
		{
			
			
			
			if (auxiliar->data->GetState()==ParticleState::TOELIM || auxiliar->data->GetAlpha() <= 0 ||(IfHasFinished(auxiliar->data->GetPosition(), auxiliar->data->GetGoal())) || auxiliar->data->IsDead() ) {
				auxiliar->data->SetState(ParticleState::DESACTIVATED);
			}
		}
	}
	if (timeCounter % 4 == 0 && state == GeneratorState::NORMAL) 
	{
		float positionX;
		float positionY;
		float acelerationX;
		float acelerationY;
		int selection;

		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() == ParticleState::DESACTIVATED)
			{
				auxiliar->data->SetGoal(temporalGoal);

				positionX = (rand() % (rang.x * 2)) - rang.x + temporalPosition.x;
				positionY = (rand() % (rang.y * 2)) - rang.y + temporalPosition.y;
				selection = (rand() % 2) + 1;
				if (temporalGoal.x > temporalPosition.x) 
				{
					switch (selection)
					{
					case 0:
						acelerationX = -0.1;
						break;
					case 1:
						acelerationX = -0.05;
						break;
					default:
						break;
					}
					//acelerationX = (rand() % 2) -1;
					acelerationY = 0;
					auxiliar->data->SetVelocity({ 1,0 });
				}
				else if (temporalGoal.x < temporalPosition.x) 
				{
					switch (selection)
					{
					case 0:
						acelerationX = 0.1;
						break;
					case 1:
						acelerationX = 0.05;
						break;
					default:
						break;
					}
					//acelerationX = (rand() % 2) +0.5;
					acelerationY = 0;
					auxiliar->data->SetVelocity({ -1,0 });
				}
				else if (temporalGoal.y > temporalPosition.y) 
				{
					switch (selection)
					{
					case 0:
						acelerationY = -0.1;
						break;
					case 1:
						acelerationY = -0.05;
						break;
					default:
						break;
					}
					//acelerationY = (rand() % 2) -1;
					acelerationX = 0;
					auxiliar->data->SetVelocity({ 0,1 });
				}
				else if (temporalGoal.y < temporalPosition.y) 
				{
					switch (selection)
					{
					case 0:
						acelerationY = 0.1;
						break;
					case 1:
						acelerationY = 0.05;
						break;
					default:
						break;
					}
					//acelerationY = (rand() % 2) +0.5;
					acelerationX = 0;
					auxiliar->data->SetVelocity({ 0,-1 });
				}

				auxiliar->data->SetState(ParticleState::STARTING);
				auxiliar->data->SetPosition({ positionX ,positionY });
				auxiliar->data->SetAceleration({ acelerationX ,acelerationY });
				auxiliar->data->SetCurrentLive(0);
				auxiliar->data->SetMaxLive((rand() % 10) + 3);
				auxiliar->data->SetRotation((rand() % 38));
				auxiliar->data->SetAlpha(255);
				break;
			}
		}

	}
	return true;
}
bool Generator::IfHasFinished(Point<float> positionA, Point<int> positionB) 
{
	/*if (positionB.x - 20 < positionA.x && positionB.x + 20 > positionA.x&& positionB.y - 20 < positionA.y && positionB.y + 20 > positionA.y) {
		return true;
	}*/
	return false;
}

bool Generator::Update(float dt)
{
	int sumX=0;
	int sumY=0;
	for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
		{
			Point<float> aceleration = auxiliar->data->GetAceleration();
			Point<float>* vel = auxiliar->data->GetVelocityPointer();
			auxiliar->data->SumPosition(Integrator(vel, dt, aceleration));

			switch (type)
			{
			case ParticleType::DUST:
				if (auxiliar->data->GetAceleration().x > 0) 
				{
					sumX = 1;
					sumY = 0;
				}else if (auxiliar->data->GetAceleration().x < 0) 
				{	
					sumX = -1;
					sumY = 0;
				}
				else if (auxiliar->data->GetAceleration().y > 0) 
				{
					sumX = 0;
					sumY = 1;
				}
				else if (auxiliar->data->GetAceleration().y < 0) 
				{
					sumX = 0;
					sumY = -1;
				}
				else 
				{
					auxiliar->data->SetState(ParticleState::DESACTIVATED);
				}
				auxiliar->data->SetAceleration({ auxiliar->data->GetAceleration().x + ((dt)*sumX), auxiliar->data->GetAceleration().y + ((dt) * sumY) });
				break;
			case ParticleType::NONE:
				break;
			default:
				break;
			}
		}
	}
	return true;
}

bool Generator::PostUpdate()
{
	for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
	{
		if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
		{
			
			auxiliar->data->SetAlpha(auxiliar->data->GetAlpha() - 1);
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(texture, auxiliar->data->GetAlpha());

			app->render->DrawTexture(texture, auxiliar->data->GetPosition().x, auxiliar->data->GetPosition().y, NULL, 1.0f, auxiliar->data->GetAngle(), false);
			auxiliar->data->SumLive(0.1);
		}
	}
	if (timeCounter == 10) {
		timeCounter = 0;
	}
	timeCounter++;
	return true;
}

void Generator::SetGoal(Point<int> Goal) 
{
	temporalGoal = Goal;
}

Point<float> Generator::Integrator(Point<float>*Velocity, float dt, Point<float>Acceleration) 
{
	Point<float> pos={0,0};
	Velocity->x += Acceleration.x * dt;
	Velocity->y += Acceleration.y * dt;

	pos.y += Velocity->y * dt;
	pos.x += Velocity->x * dt;
	
	return { metersToPixels(pos.x),metersToPixels(pos.y) };
}