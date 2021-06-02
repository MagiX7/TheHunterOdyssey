#include "App.h"
#include "Audio.h"
#include"AssetsManager.h"
#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

Audio::Audio() : Module()
{
	music = NULL;
	name.Create("audio");

	musicVolume = 0;
	fxVolume = 0;
	nextMusic = "";
	auxMusic = 0;
	transition = false;
	fadeOut = false;
	maxChannels = 10;
	numChannels = 0;
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	musicVolume = config.child("musicVolume").attribute("value").as_float();
	fxVolume = config.child("fxVolume").attribute("value").as_int();

	Mix_AllocateChannels(maxChannels);

	return ret;
}

bool Audio::Update(float dt)
{
	/*LOG("MUSIC VOLUME E %f", musicVolume);
	LOG("AUX VOLUME E %f", auxMusic);*/
	if (transition)
	{
		if (fadeOut)
		{
			musicVolume -= (auxMusic * 1.5) * dt;
			if (musicVolume <= 0.0f)
			{
				musicVolume = 0;
				Mix_FreeMusic(music);
				app->assetsManager->DeleteBufferMus();
				music = Mix_LoadMUS_RW(app->assetsManager->LoadMusic(nextMusic), 1);
				Mix_PlayMusic(music, - 1);
				nextMusic = "";
				fadeOut = false;
			}
			Mix_VolumeMusic(musicVolume);
		}
		else
		{
			musicVolume += (auxMusic * 1.5) * dt;
			if (musicVolume >= auxMusic)
			{
				musicVolume = auxMusic;
				transition = false;
				fadeOut = false;
			}
			Mix_VolumeMusic(musicVolume);
		}
	}

	return true;
}

// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	UnLoadFxs();
	Mix_AllocateChannels(0);
	//channels.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, bool needsTransition, float fadeTime)
{
	bool ret = true;
	
	if(!active)
		return false;
	
	if (needsTransition)
	{
		auxMusic = musicVolume;
		transition = true;

		fadeOut = true;
		nextMusic = path;
	}
	else
	{
		Mix_FreeMusic(music);
		app->assetsManager->DeleteBufferMus();
		music = Mix_LoadMUS_RW(app->assetsManager->LoadMusic(path), 1);
		Mix_PlayMusic(music, -1);
	}

	return ret;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(app->assetsManager->Load(path), 1);
	app->assetsManager->DeleteBuffer();
	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(int channel, unsigned int id, int repeat, int volume)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		if (Mix_Playing(channel) == 0)
		{
			Mix_VolumeChunk(fx.at(id - 1), fxVolume);
			Mix_PlayChannel(-1, fx.at(id - 1), repeat);
		}
	}

	return ret;
}

bool Audio::UnLoadFx(int index)
{
	if (index > 0 && index <= fx.size())
	{
		Mix_FreeChunk(fx.at(index - 1));
		eastl::erase(fx, fx.at(index - 1));
	}

	return true;
}

void Audio::SetDistanceFx(int channel, int angle, uint distance, uint maxDistance)
{
	distance = distance * 255 / maxDistance;
	if (distance > 255) distance = 255;
	Mix_SetPosition(channel, angle, distance);
}

void Audio::Reset()
{
	Mix_FreeMusic(music);
	fx.clear();
}

void Audio::SetMusicVolume(int index)
{
	musicVolume = index;
	Mix_VolumeMusic(musicVolume);
}

void Audio::SetFxVolume(int index)
{
	fxVolume = index;
}

int Audio::GetMusicVolume()
{
	return musicVolume;
}

int Audio::GetFxVolume()
{
	return fxVolume;
}

void Audio::UnLoadFxs()
{
	eastl::deque<Mix_Chunk*>::iterator item;
	eastl::deque<Mix_Chunk*>::iterator itEnd = fx.end();
	for (item = fx.begin(); item != itEnd; ++item)
		Mix_FreeChunk(*item);

	fx.clear();
}

int Audio::SetChannel()
{
	int ret = -1;
	++numChannels;
	if (numChannels < maxChannels)
	{
		return numChannels;
	}
	else
	{
		maxChannels += 10;
		Mix_AllocateChannels(maxChannels);
		return numChannels;
	}

	return ret;
}