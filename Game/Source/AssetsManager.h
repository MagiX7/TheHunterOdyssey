#ifndef _ASSETSMANAGER_H_
#define _ASSETSMANAGER_H_
#include"Module.h"
#include"External/SDL/include/SDL.h"
class AssetsManager :public Module {
public:
	AssetsManager();
	~AssetsManager();
	bool Start()override;
	SDL_RWops* Load(const char* fileName);
	int MakeLoad( const char* fileName);
	bool DeleteBuffer();
	char* GetLastBuffer();
	char* buffer = nullptr;
};
#endif // !_ASSETSMANAGER_H_
