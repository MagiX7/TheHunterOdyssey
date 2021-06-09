#include "Generator.h"

#include <time.h>
#include <stdlib.h>
#define HALF_CIRCLE 180
Generator::Generator(ParticleType Type) 
{
	srand(time(NULL));
	
	Particle* particle;
	timeCounter = 0;
	type = Type;
	switch (type)
	{
	case ParticleType::DUST:
		maxParticles = 200;
		startFireLoop = true;
		texture = app->tex->Load("Textures/Dust/dust_2.png");
		for (int a = 0; a < maxParticles; ++a)
		{
			particle = new Particle(ParticleState::DESACTIVATED);
			particleList.Add(particle);
		}
		state = GeneratorState::NORMAL;
		break;
	case ParticleType::FIRE:
		maxParticles = 32;
		setGeneratorMaxLive(1);
		texture = app->tex->Load("Textures/Fire/Fire.png");
		for (int a = 0; a < maxParticles; ++a)
		{
			particle = new Particle(ParticleState::DESACTIVATED);
			particleList.Add(particle);
		}
		state = GeneratorState::DISABLE;
		startFireLoop = true;

		break;
	case ParticleType::MAGIC:
		maxParticles = 70;
		texture = app->tex->Load("Textures/Magic/Magic.png");
		for (int a = 0; a < maxParticles; ++a)
		{
			particle = new Particle(ParticleState::DESACTIVATED);
			particleList.Add(particle);
		}
		state = GeneratorState::DISABLE;
		startFireLoop = true;
		break;
	case ParticleType::GRAVITY:
		maxParticles = 30;
		texture = app->tex->Load("Textures/Magic/Magic.png");
		for (int a = 0; a < maxParticles; ++a)
		{
			particle = new Particle(ParticleState::DESACTIVATED);
			particle->setBounds({ 0,0,19,19 });
			particleList.Add(particle);
		}
		state = GeneratorState::DISABLE;
		startFireLoop = true;
		break;
	default:
		break;
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

void Generator::Start() 
{
}

void Generator::CleanUp() 
{
	ListItem<Particle*>* auxiliar1;
	ListItem<Particle*>* auxiliar = particleList.start;
	
	while (auxiliar !=nullptr)
	{
		auxiliar1 = auxiliar->next;
		RELEASE(auxiliar->data);
		particleList.Del(auxiliar);
		auxiliar = auxiliar1;
	}
	
	particleList.Clear();
	app->tex->UnLoad(texture);
}

bool Generator::PreUpdate()
{
	
	switch (type)
	{
	case ParticleType::DUST:
		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
			{

				if (auxiliar->data->GetState() == ParticleState::TOELIM || auxiliar->data->GetAlpha() <= 0 || (IfHasFinished(auxiliar->data->GetPosition(), auxiliar->data->GetGoal())) || auxiliar->data->IsDead())
				{
					auxiliar->data->SetState(ParticleState::DESACTIVATED);
				}
			}
		}
		if (startFireLoop &&state == GeneratorState::NORMAL)
		{
			startFireLoop = false;
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
		break;
	case ParticleType::MAGIC:

		if (state == GeneratorState::STARTING) {
			startFireLoop = true;
			state = GeneratorState::NORMAL;
		}
		
		if (startFireLoop == true) {
			float positionX;
			float positionY;
			int counter = 0;
			
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{
				/*if (counter < 0) {
					auxiliar->data->setBounds({ 0,0,19,19 });
					auxiliar->data->SetAceleration({ 0 ,0.5 });
				}
				else if (counter < 5) {
					auxiliar->data->setBounds({ 2,25,14,14 });
					auxiliar->data->SetAceleration({ 0 ,((rand() % 10)+1.0f) / 10.0f });
					auxiliar->data->SetMaxLive((rand() % 10) / 10);
				}
				else if (counter <20) {*/
					
					/*auxiliar->data->SetAceleration({ 0 ,((rand() % 10)+5.0f)/10.0f });
					auxiliar->data->SetMaxLive((rand() % 10) /10);
				}
				else if (counter < 30) {*/
					//auxiliar->data->setBounds({ 27,29,6,6 });
					auxiliar->data->setBounds({ 25,3,11,11 });
					auxiliar->data->SetAceleration({ 0 ,((rand() % 10) +1.0f)/10.0f});
					float num = (rand() % 10);
					auxiliar->data->SetMaxLive((num+15.0f)/10.0f);
				/*}*/
				auxiliar->data->SetCurrentLive(0);
				auxiliar->data->SetState(ParticleState::STARTING);
				auxiliar->data->SetAlpha(255);
				auxiliar->data->SetVelocity({ 0,0 });
				positionX=(rand() %40)+ temporalPosition.x;
				positionY = (rand() % 10) + temporalPosition.y;
				auxiliar->data->SetPosition({ positionX ,positionY });
				counter++;
				
			}
			startFireLoop = false;
		}
		
		break;
	case ParticleType::GRAVITY:
		if (state == GeneratorState::STARTING) {
			startFireLoop = true;
			state = GeneratorState::NORMAL;
		}

		if (startFireLoop == true) {
			float positionX;
			float positionY;
			int counter = 0;

			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{

				auxiliar->data->SetAceleration({ 0 ,((rand() % 10) + 1.0f) / -10.0f });
				float num = (rand() % 10);
				auxiliar->data->SetMaxLive((num + 15.0f) / 10.0f);

				auxiliar->data->SetCurrentLive(0);
				auxiliar->data->SetState(ParticleState::STARTING);
				auxiliar->data->SetAlpha(255);
				auxiliar->data->SetVelocity({ 0,1 });
				positionX = (rand() % 40) + temporalPosition.x;
				positionY = (rand() % 10) + temporalPosition.y;
				auxiliar->data->SetPosition({ positionX ,positionY });
				counter++;

			}
			startFireLoop = false;
		}
		break;
	case ParticleType::FIRE:
		if (state == GeneratorState::STARTING) {
			startFireLoop = true;
			state = GeneratorState::NORMAL;
		}
		float angle = 0;
		float temp = 0;

		if (startFireLoop == true) {
			float positionX;
			float positionY;

			setGeneratorLive(0);
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{

				auxiliar->data->SetState(ParticleState::STARTING);
				temp = ((angle * M_PI) / HALF_CIRCLE);
				float auxiliarX = cos(temp);
				float auxiliarY = sin(temp);
				float auxVelX = 0;
				float auxVelY = 0;
				auxiliar->data->SetAceleration({ auxiliarX,auxiliarY });
				auxiliar->data->SetRotation((rand() % 360));
				auxVelX = auxiliarX * 3;
				auxVelY = auxiliarY * 3;
				angle += 11.25f;
				auxiliar->data->SetAlpha(255);
				auxiliar->data->SetVelocity({ auxVelX,auxVelY });
				positionX = temporalPosition.x;
				positionY = temporalPosition.y;
				auxiliar->data->SetPosition({ positionX ,positionY });


			}
			startFireLoop = false;
		}
		if (getCurrentLive() > getMaxLive()) {
			state = GeneratorState::DISABLE;
		}
		break;
	
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
	bool isDesact = true;

	switch (type)
	{
	case ParticleType::DUST:
		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
			{
				Point<float> aceleration = auxiliar->data->GetAceleration();
				Point<float>* vel = auxiliar->data->GetVelocityPointer();
				auxiliar->data->SumPosition(Integrator(vel, dt, aceleration));
				if (auxiliar->data->GetAceleration().x > 0)
				{
					sumX = 1;
					sumY = 0;
				}
				else if (auxiliar->data->GetAceleration().x < 0)
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
				auxiliar->data->SetAceleration({ auxiliar->data->GetAceleration().x + ((dt)*sumX), auxiliar->data->GetAceleration().y + ((dt)*sumY) });
			}
		}
		timeCounter += dt;
		if (timeCounter >= dt * 7) 
		{
			timeCounter = 0;
			startFireLoop = true;
		}
		break;
	case ParticleType::FIRE:
		sumGeneratorLive(dt);
		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
			{
				Point<float> aceleration = auxiliar->data->GetAceleration();
				Point<float>* vel = auxiliar->data->GetVelocityPointer();
				auxiliar->data->SumPosition(Integrator(vel, dt, aceleration));
				
			}
		}
		break;
	case ParticleType::GRAVITY:
		
		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
			{
				Point<float> aceleration = auxiliar->data->GetAceleration();
				Point<float>* vel = auxiliar->data->GetVelocityPointer();
				auxiliar->data->SumPosition(Integrator(vel, dt, aceleration));
				auxiliar->data->SumLive(dt);
				isDesact = false;
				if (auxiliar->data->GetCurrentLive() > auxiliar->data->GetMaxLive()) {
					auxiliar->data->SetState(ParticleState::DESACTIVATED);
				}
			}
		}
		if (isDesact && startFireLoop == false) {
			state = GeneratorState::DISABLE;
			startFireLoop = true;
		}
		break;
	case ParticleType::MAGIC:
		
		for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
		{
			if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
			{
				Point<float> aceleration = auxiliar->data->GetAceleration();
				Point<float>* vel = auxiliar->data->GetVelocityPointer();
				auxiliar->data->SumPosition(Integrator(vel, dt, aceleration));
				auxiliar->data->SumLive(dt);
				isDesact = false;
				if (auxiliar->data->GetCurrentLive() > auxiliar->data->GetMaxLive()) {
					auxiliar->data->SetState(ParticleState::DESACTIVATED);
				}
			}
		}
		if (isDesact&& startFireLoop==false) {
			state = GeneratorState::DISABLE; 
			startFireLoop = true;
		}
		break;
	
		
	}

	return true;
}

bool Generator::PostUpdate()
{
	
		switch (type)
		{
		case ParticleType::DUST:
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{
				if (auxiliar->data->GetState() != ParticleState::DESACTIVATED)
				{

					auxiliar->data->SetAlpha(auxiliar->data->GetAlpha() - 1);
					SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
					SDL_SetTextureAlphaMod(texture, auxiliar->data->GetAlpha());

					app->render->DrawTexture(texture, auxiliar->data->GetPosition().x, auxiliar->data->GetPosition().y, NULL, true, 1.0f, auxiliar->data->GetAngle());
					auxiliar->data->SumLive(0.1);
				}
			}
			
			break;
		case ParticleType::FIRE:
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{
				if (state!=GeneratorState::STARTING)
				{
					auxiliar->data->SetAlpha(auxiliar->data->GetAlpha() - 3);
					SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
					SDL_SetTextureAlphaMod(texture, auxiliar->data->GetAlpha());
					app->render->DrawTexture(texture, auxiliar->data->GetPosition().x, auxiliar->data->GetPosition().y, NULL, true, 1.0f, auxiliar->data->GetAngle());
				}
			}
			break;
		case ParticleType::MAGIC:
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{
				if (state != GeneratorState::STARTING&& state != GeneratorState::DISABLE&& auxiliar->data->GetState()!=ParticleState::DESACTIVATED)
				{
					/*auxiliar->data->SetAlpha(auxiliar->data->GetAlpha() - 1);
					SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
					SDL_SetTextureAlphaMod(texture, auxiliar->data->GetAlpha());*/
					SDL_Rect rect{ auxiliar->data->getBounds().x, auxiliar->data->getBounds().y, auxiliar->data->getBounds().w, auxiliar->data->getBounds().h };
					app->render->DrawTexture(texture, auxiliar->data->GetPosition().x, auxiliar->data->GetPosition().y,&rect, true, 1.0f, auxiliar->data->GetAngle());
				}
			}
			break;
		case ParticleType::GRAVITY:
			for (ListItem<Particle*>* auxiliar = particleList.start; auxiliar != nullptr; auxiliar = auxiliar->next)
			{
				if (state != GeneratorState::STARTING && state != GeneratorState::DISABLE && auxiliar->data->GetState() != ParticleState::DESACTIVATED)
				{
					/*auxiliar->data->SetAlpha(auxiliar->data->GetAlpha() - 1);
					SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
					SDL_SetTextureAlphaMod(texture, auxiliar->data->GetAlpha());*/
					SDL_Rect rect{ auxiliar->data->getBounds().x, auxiliar->data->getBounds().y, auxiliar->data->getBounds().w, auxiliar->data->getBounds().h };
					app->render->DrawTexture(texture, auxiliar->data->GetPosition().x, auxiliar->data->GetPosition().y, &rect, true, 1.0f, auxiliar->data->GetAngle());
				}
			}
			if (getCurrentLive() > getMaxLive()) {
				state = GeneratorState::DISABLE;
			}
			break;
		}
		
	

	
	return true;
}
void Generator::SetGeneratorState(GeneratorState State) 
{
	state = State;
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
	
	return { metersToPixels(pos.x), metersToPixels(pos.y) };
}
void Generator::setGeneratorLive(float num){
	generatorLive = num;
}
float Generator::getCurrentLive(){
	return generatorLive;
}
void Generator::sumGeneratorLive(float num){
	generatorLive += num;
}
void Generator::setGeneratorMaxLive(float num){
	maxGeneratorLive = num;
}
float  Generator::getMaxLive() {
	return maxGeneratorLive;
}