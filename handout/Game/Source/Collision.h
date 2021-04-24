#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Module.h"
#include <vector>

class Entity;

enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_FLOOR,
	COLLIDER_SCENE,
	COLLIDER_PLAYER,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	ColliderType type;
	Entity* callback = nullptr;
	Collider(SDL_Rect rectangle, ColliderType type, Entity* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	bool CheckCollision(const SDL_Rect& r) const;
};
class Colliders : public Module
{
public:
	Colliders();
	~Colliders();
	bool Awake(pugi::xml_node&);
	bool PreUpdate();
	bool Update(float dt);
	//update_status PostUpdate();
	bool CleanUp();
	Collider* AddCollider(SDL_Rect rect, ColliderType type, Entity * callback = nullptr);
	bool EraseAllCollider(Collider* collider);
	bool EraseMapCollider();
	void DebugDraw();
	bool checkColisionList(Collider* enemCollider);
	bool Check(Collider* c1, ColliderType type);

	bool debug = false;

private:
	std::vector<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};
#endif // __COLLIDERS_H__ 
