#include "Log.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collision.h"
#include "Entity.h"
#include "Window.h"

#include "Brofiler/Brofiler.h"


Colliders::Colliders() : Module()
{
	name.assign("colliders");

	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_SCENE] = false;
	
	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_SCENE] = true;

	matrix[COLLIDER_SCENE][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SCENE][COLLIDER_SCENE] = false;

}
// Destructor
Colliders::~Colliders()
{
}

bool Colliders::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	debug = node.child("debug").attribute("value").as_bool();
	return ret;
}
bool Colliders::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateCollision", Profiler::Color::Yellow);
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
// Called before render is available
bool Colliders::Update(float dt)
{
	BROFILER_CATEGORY("UpdateCollision", Profiler::Color::Red);

	Collider* c1;
	Collider* c2;
	for (uint i = 0; i < colliders.size(); ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;
		c1 = colliders[i];
		// avoid checking collisions already checked
		for (uint k = i + 1; k < colliders.size(); ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;
			c2 = colliders[k];
			if (c1->CheckCollision(c2->rect) == true)
			{
				if (c1->to_delete == false && c2->to_delete == false) {
					if (matrix[c1->type][c2->type] && c1->callback) {
						c1->callback->OnCollision(c1, c2, dt);
					}
					if (c1->to_delete == false && c2->to_delete == false) {
						if (matrix[c2->type][c1->type] && c2 != nullptr && c2->callback != nullptr)
 							c2->callback->OnCollision(c2, c1, dt);
					}
				}
			}
		}
	}
	DebugDraw();
	return true;
}
void Colliders::DebugDraw()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		LOG("You can see the colliders");
		debug = !debug;
	}

	if(debug) {
		Uint8 alpha = 80;
		for (uint i = 0; i < colliders.size(); ++i)
		{
			if (colliders[i] == nullptr)
				continue;
			switch (colliders[i]->type)
			{
			case COLLIDER_NONE: // white
				app->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_FLOOR: // red
				app->render->DrawQuad(colliders[i]->rect, 220, 20, 60, alpha);
				break;
			case COLLIDER_SCENE: //blue
				app->render->DrawQuad(colliders[i]->rect, 0, 0, 220, alpha);
				break;
			case COLLIDER_PLAYER: // green
				app->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			default: //white
				app->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			}
		}
		app->render->DrawCircle(app->win->GetWindowWidth()*0.5F, app->win->GetWindowHeight()*0.5F, 3, 255, 0, 0);//Draw a circle on the middle of screen to know where is the center
	}
}
bool Colliders::checkColisionList(Collider * enemCollider)
{
	for (uint i = 0; i < colliders.size(); ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (enemCollider->CheckCollision(colliders[i]->rect))
			return true;
	}
	return false;
}
// Called before quitting
bool Colliders::CleanUp()
{
	LOG("Freeing all colliders");
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
Collider* Colliders::AddCollider(SDL_Rect rect, ColliderType type, Entity * callback)
{
	Collider* ret = nullptr;
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}
bool Colliders::EraseAllCollider(Collider* collider)
{
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}
	return false;
}

bool Colliders::EraseMapCollider()
{
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] != nullptr && (colliders[i]->type <= COLLIDER_SCENE))
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return false;
}
// -----------------------------------------------------
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.x > rect.x + rect.w || r.x + r.w<rect.x || r.y>rect.y + rect.h || r.y + r.h < rect.y)
		return false;
	return true;
}

bool Colliders::Check(Collider* c1, ColliderType type) {

	Collider* c2 = nullptr;
	// avoid checking collisions already checked
	for (uint k = 0; k < colliders.size(); ++k)
	{
		// skip empty colliders
		if (colliders[k] == nullptr || colliders[k]->type != type)
			continue;
		c2 = colliders[k];
		if (c1->CheckCollision(c2->rect) == true)
		{
			return true;
		}
	}

	return false;
	
}