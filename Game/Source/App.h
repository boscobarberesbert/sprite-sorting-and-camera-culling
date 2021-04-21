#ifndef __APP_H__
#define __APP_H__

#include <list>
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Colliders;
class Map;
class FadeToBlack;
class EntityManager;
class PathFinding;
class Fonts;
class Gui;
class MainMenu;


enum Levels
{
	MENU = 0,
	CREDITS,
	TUTORIAL,
	FIRST_LEVEL,
	SECOND_LEVEL,
	
};



class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;


private:

	
	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	Window*			win;
	Input*			input;
	Render*			render;
	Textures*			tex;
	Audio*			audio;
	Scene*			scene;
	Colliders*		collider;
	Map*				map;
	FadeToBlack*      fade;
	EntityManager*	entities;
	PathFinding*		pathfinding;
	Fonts*			fonts;
	Gui*				gui;
	MainMenu*         menu;

private:

	std::list<Module*>	modules;
	uint					frames;
	float					dt;
	int						argc;
	char**					args;

	std::string			title;
	std::string			organization;

	uint32				framerate_cap = 0;
	bool				cap_framerate = true;
	PerfTimer			ptimer;
	uint64				frame_count = 0;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	public:
		bool				quit_game = false;
};

extern App* app;

#endif