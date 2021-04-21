#include "Log.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collision.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Player.h"
#include "Static.h"
#include "NPC.h"
#include "Window.h"
#include <algorithm>

#include "Brofiler/Brofiler.h"

EntityManager::EntityManager()
{
	name.assign("entities");
}

// Destructor
EntityManager::~EntityManager()
{
}


bool EntityManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;
	
	return ret;
}


bool EntityManager::Start()
{
	LOG("loading enemies");

	return true;
}

// Called before render is available
bool EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntityManager", Profiler::Color::Red);

	return UpdateAll(dt);
}

bool EntityManager::UpdateAll(float dt)
{
	bool ret = true;

	std::vector<Entity*> drawEntities;
	SDL_Rect r = { 0,0,0,0 };
	uint entities_drawn = 0;

	for (std::vector<Entity*>::iterator item = entities.begin();item != entities.end(); ++item) {
		if (*item != nullptr) {
			ret = (*item)->Update(dt);

			if (app->render->IsInCamera( (*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y )) {
				drawEntities.push_back(*item);
			}
		}
	}

	std::sort(drawEntities.begin(), drawEntities.end(), EntityManager::SortByYPos);

	for (std::vector<Entity*>::iterator item = drawEntities.begin(); item != drawEntities.end(); ++item) {
		if (*item != nullptr) {
			(*item)->Draw();
			entities_drawn++;

			if (app->scene->entitiesBox) {
				DrawDebugQuad(*item);
			}
		}
	}

	drawEntities.clear();
	
	static char title[30];
	sprintf_s(title, 30, " | Entities drawn: %u", entities_drawn);
	
	app->win->AddStringToTitle(title);

	return ret;
}



bool EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::Green);

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	for (std::vector<Entity*>::iterator item = entities.begin(); item != entities.end(); ++item)
	{
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			RELEASE(*item);
		}
	}
	entities.clear();

	return true;
}

Entity* EntityManager::CreateEntity(Entity::Types type, int PositionX, int PositionY, std::string name)
{
	static_assert(Entity::Types::UNKNOWN == (Entity::Types)3, "code needs update");
	Entity* ret = nullptr;
	switch (type) {
	case Entity::Types::PLAYER: ret = new Player(PositionX, PositionY, name); break;
	case Entity::Types::STATIC: ret = new Static(PositionX, PositionY, name); break;
	case Entity::Types::NPC: ret = new NPC(PositionX, PositionY, name); break;
	}
	if (ret != nullptr) {
		entities.push_back(ret);
		ret->Start();
	}
	
	return ret;
}

void EntityManager::DestroyEntity(Entity * entity)
{
	
	if (entity != nullptr) {
		entity->CleanUp();
		for (int i = 0; i < entities.size(); ++i) {
			if (entities[i] == entity) {
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}
}

void EntityManager::DrawDebugQuad(Entity *entity)
{
	SDL_Rect section = { entity->position.x, entity->position.y, entity->size.x, entity->size.y };
	Uint8 alpha = 80;

	switch (entity->type) {
	case Entity::Types::PLAYER:
		app->render->DrawQuad(section, 0, 255, 0, alpha);
		break;
	case Entity::Types::NPC:
		app->render->DrawQuad(section, 255, 0, 0, alpha);
		break;

	case Entity::Types::STATIC:
		if (entity->name == "pilar")
			app->render->DrawQuad(section, 0, 255, 0, alpha);
		else if (entity->name == "corner" || entity->name == "door")
			app->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_d_l")
			app->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_d_r")
			app->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_u_l")
			app->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_u_r")
			app->render->DrawQuad(section, 255, 255, 0, alpha);
		else {
			app->render->DrawQuad(section, 255, 255, 255, alpha);
		}
		break;
	default:
		app->render->DrawQuad(section, 255, 255, 255, alpha);
		break;
	}

	app->render->DrawCircle(section.x + entity->pivot.x, section.y + entity->pivot.y, 3, 0, 255, 0);
}
