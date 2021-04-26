#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Scene.h"

#include "Brofiler/Brofiler.h"

Scene::Scene() : Module()
{
	name.assign("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	bool ret = true;

	for (conf = conf.first_child(); conf != nullptr; conf = conf.next_sibling()) {
		scenes.push_back(conf.attribute("name").as_string());
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	bool ret = false;

	ret = app->map->Load(scenes[0].data()); //Load first map in config.xml

	CreateEntities();

	app->render->camera = app->render->CameraInitPos();

	return ret;
}

void Scene::CreateEntities()
{
	//Iterate all objects of the map made with Tiled to find entities

	for (std::list<ColliderObject*>::iterator position = app->map->data.colliders.begin(); position != app->map->data.colliders.end(); position++) {
		if ((*position)->name == "player") {
			app->entities->CreateEntity(Entity::Types::PLAYER, (*position)->collX, (*position)->collY, (*position)->name);
		}
		else if ((*position)->entType == "static") {
			app->entities->CreateEntity(Entity::Types::STATIC, (*position)->collX, (*position)->collY, (*position)->name);
				
		}
		else if ((*position)->entType == "NPC") {
			app->entities->CreateEntity(Entity::Types::NPC, (*position)->collX, (*position)->collY, (*position)->name);
		}
		else {
			LOG("There isn't any entity with name %s and type %s", (*position)->name.data(), (*position)->entType.data());
		}
	}
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::Red);

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 300 * dt;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 300 * dt;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 300 * dt;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 300 * dt;


	//------Debug Keys---------------------
	//Show/hide grid
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->map->drawGrid = !app->map->drawGrid;

	//Show/hide entities quad size and pivot
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		entitiesBox = !entitiesBox;

	//Change between different map types----------------------------
	bool change_map = false;
	uint id = -1;

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		change_map = true;
		id = 0;
	}
	else if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		change_map = true;
		id = 1;
	}

	if (change_map) {
		app->map->CleanUp();
		app->entities->CleanUp();

		app->map->Load(scenes[id].data());
		CreateEntities();
	}
	//--------------------------------------------------------------
	//Change between zoom levels-----------------------------
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		app->win->SetScale(1);

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		app->win->SetScale(2);

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		app->win->SetScale(3);
	//-------------------------------------------------------

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->quit_game = true;


	app->map->Draw();	
	
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	return app->entities->CleanUp();
}