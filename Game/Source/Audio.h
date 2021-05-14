#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include "EASTL/deque.h"


#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt) override;

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int channel, unsigned int fx, int repeat = 0, int volume = -1);

	bool UnLoadFx(int index);

	void SetDistanceFx(int channel, int angle, uint distance, uint maxDistance);

	void Reset();

	void SetMusicVolume(int index);

	void SetFxVolume(int index);

	int GetMusicVolume();

	int GetFxVolume();

	inline bool FadeOutCompleted() { return fadeOut; }

	int SetChannel();

	void UnLoadFxs();
private:
	_Mix_Music* music;
	eastl::deque<Mix_Chunk*> fx;
	eastl::list<int> channels;

	float musicVolume;
	int fxVolume;

	const char* nextMusic;
	float auxMusic;
	bool transition;
	bool fadeOut;

	int maxChannels;
	int numChannels;
};

#endif // __AUDIO_H__