#include "AssetsManager.h"
#include "External/PhysFS/include/physfs.h"

AssetsManager::AssetsManager() :Module() 
{
	name.Create("AssetsManager");
	//TODO 1: initialize the library (1 line)
	PHYSFS_init(nullptr);
}

AssetsManager::~AssetsManager() 
{
	//TODO 2: Deinitialize the library (1 line)
	PHYSFS_deinit();
}

bool AssetsManager::Start() 
{
	//TODO 3: add the search path (1 line)
	PHYSFS_mount("Assets.zip",NULL, 0);
	return true;
}

bool AssetsManager::DeleteBuffer() 
{
	RELEASE_ARRAY(buffer);
	return true;
}

bool AssetsManager::DeleteBufferMus() 
{
 	if (buffer1 != nullptr) 
	{
		RELEASE_ARRAY(buffer1);
	}
	
	return true;
}

char* AssetsManager::GetLastBuffer() 
{
	return buffer;
}

int AssetsManager::MakeLoad(const char* fileName) 
{
	int ret = 0;
	//TODO 5: get the information buffer, and its size with the functions and variables of the library (5 line ~)
	PHYSFS_file* data_file = PHYSFS_openRead(fileName);

	if (data_file != nullptr)
	{
		int file_lenght = PHYSFS_fileLength(data_file);
		buffer = new char[(int)file_lenght];
		int readed = PHYSFS_read(data_file, buffer, 1, (int)file_lenght);
		if (readed != file_lenght)
		{
			RELEASE_ARRAY(buffer);
		}
		ret = readed;
		PHYSFS_close(data_file);
	}
	
	return ret;
}

int AssetsManager::MakeLoadMus(const char* fileName)
{
	int ret = 0;
	//TODO 5: get the information buffer, and its size with the functions and variables of the library (5 line ~)
	PHYSFS_file* data_file = PHYSFS_openRead(fileName);

	if (data_file != nullptr)
	{
		int file_lenght = PHYSFS_fileLength(data_file);
		buffer1 = new char[(int)file_lenght];
		int readed = PHYSFS_read(data_file, buffer1, 1, (int)file_lenght);
		if (readed != file_lenght)
		{
			RELEASE_ARRAY(buffer1);
		}
		ret = readed;
		PHYSFS_close(data_file);
	}

	return ret;
}

SDL_RWops* AssetsManager::Load(const char* fileName) 
{
	//TODO 4: call the MakeLoad functionand get the RWops structure to load the data(3 line)
	
	int size = MakeLoad(fileName);
	return SDL_RWFromConstMem(buffer, size);
}

SDL_RWops* AssetsManager::LoadMusic(const char* fileName)
{
	//TODO 4: call the MakeLoad functionand get the RWops structure to load the data(3 line)

	int size = MakeLoadMus(fileName);
	return SDL_RWFromConstMem(buffer1, size);
}