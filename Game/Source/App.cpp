#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

// EASTL library
void* operator new[](size_t size, const char* pName, int flags, unsigned     debugFlags, const char* file, int line)
{
	return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return malloc(size);
}

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	sceneManager = new SceneManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(Window::Get());
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(sceneManager);

	// Render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	eastl::list<Module*>::iterator item;

	for (item = modules.end().prev(); item != modules.begin(); --item)
	{
		RELEASE(*item);
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		int cap = configApp.attribute("framerate_cap").as_int(-1);
		if (cap > 0) cappedMs = 1000 / cap;
	}

	if (ret == true)
	{
		eastl::list<Module*>::iterator item;

		for (item = modules.begin(); item != modules.end() && ret; ++item)
		{
			ret = (*item)->Awake(config.child((*item)->name.GetString()));
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	eastl::list<Module*>::iterator item;

	for (item = modules.begin(); item != modules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{

	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;

}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
		averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);

	Window::Get()->SetTitle(title);

	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		// L08: DONE 3: Measure accurately the amount of time SDL_Delay actually waits compared to what was expected
		PerfTimer pt;
		SDL_Delay(cappedMs - lastFrameMs);
		LOG("We waited for %d milliseconds and got back in %f", cappedMs - lastFrameMs, pt.ReadMs());
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	eastl::list<Module*>::iterator item;
	Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false)
			continue;

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	eastl::list<Module*>::iterator item;
	Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false)
			continue;

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	eastl::list<Module*>::iterator item;
	Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = *item;

		if(pModule->active == false) 
			continue;

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	eastl::list<Module*>::iterator item;

	for (item = modules.end().prev(); item != modules.begin() && ret == true; --item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	loadGameRequested = false;
	bool ret = true;

	pugi::xml_parse_result resul = saveLoadFile.load_file("save_game.xml");

	if (resul == NULL)
	{
		LOG("Could not load save and load xml file. pugi error: %s", resul.description());
		return false;
	}
	else
	{
		saveState = saveLoadFile.child("save_status");

		eastl::list<Module*>::iterator item = modules.begin();

		while (item != modules.end() && ret)
		{
			ret = (*item)->LoadState(saveState.child((*item)->name.GetString()));
			++item;
		}

		LOG("File loaded successfully!");
	}

	return ret;
}

bool App::SaveGame() const
{
	LOG("Saving Results!!");
	saveGameRequested = false;
	bool ret = true;

	eastl::list<Module*>::iterator item = modules.begin().mpNode;
	
	pugi::xml_document file;

	auto root = file.append_child("save_status");

	while (item != modules.end())
	{
		root.append_child((*item)->name.GetString());
		ret = (*item)->SaveState(root.child((*item)->name.GetString()));
		++item;
	}

	bool saveSucceed = file.save_file("save_game.xml", PUGIXML_TEXT("  "));
	if (saveSucceed == false)
	{
		LOG("Couldn't save the file. pugi error: %s", pugi::status_internal_error);
	}

	return ret;
}