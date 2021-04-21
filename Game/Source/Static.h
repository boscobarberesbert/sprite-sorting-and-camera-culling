#ifndef __STATIC_H__
#define __STATIC_H__

#include "Entity.h"


struct SDL_Texture;

class Static : public Entity
{

public:

	enum class Type {
		BUILDING,
		ROCK,
		CACTUS,
		PALM_TREE,
		MOUNTAIN,

		PILAR,
		CORNER,
		WALL_UP_RIGHT,
		WALL_UP_LEFT,
		WALL_DOWN_RIGHT,
		WALL_DOWN_LEFT,
		DOOR,

		UNKNOWN
	};

public:

	Static(int x, int y, std::string name);

	~Static();

	//void OnCollision(Collider*, Collider*, float dt);

private:

	void Draw();
	void SetRect(int x, int y, int w, int h);

private:
	SDL_Rect frame;
	Type type;
};

#endif