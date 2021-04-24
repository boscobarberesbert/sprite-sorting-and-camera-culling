#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "Animation.h"
#include <string>
#include "Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct Collider;

struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	std::string name;
	uint tileWidth = 0;
	uint tileHeight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tileCount = 0;
	uint columns = 0;
	std::string imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

enum class EntityState {
	IDLE = 0,
	WALKING,

	UNKNOWN
};

struct EntityAnim {
	uint id = 0;
	uint numFrames = 0;
	SDL_Rect* frames = nullptr;
	EntityState animType;

	uint FrameCount(pugi::xml_node&);
};

struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint numAnimations = 0;
};

struct ColliderInfo {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	ColliderType type;
};

class Entity
{
public:

	enum class Types
	{
		PLAYER,
		NPC,
		STATIC,

		UNKNOWN
	};

public:

	Entity(Types type, int x, int y, std::string name);
	virtual ~Entity();

	const Collider* GetCollider() const;

	bool LoadEntityData(const char*);
	virtual void LoadProperties(pugi::xml_node&);
	virtual void LoadCollider(pugi::xml_node&);
	virtual void IdAnimToEnum();
	virtual void PushBack() {};
	virtual void AddColliders(Entity* c = nullptr);
	void SetPivot(const int &x, const int &y);

	virtual bool Start() { return true; }
	virtual bool Update(float dt);
	virtual void Draw();
	virtual void OnCollision(Collider* collider1, Collider * collider2, float dt) {}
	virtual bool CleanUp();
	
public:

	std::string name;

	fPoint position;
	iPoint pivot = { 0, 0 };

	iPoint size = { 0, 0 };

	Types type;

	Animation animIdle;

	ColliderInfo collider;

	EntityInfo data;

	std::string folder = "Assets/Entities/";

	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;

	pugi::xml_document	entityFile;
	
	Animation* currentAnim = nullptr;
	float animationSpeed;
};
#endif // __ENTITY_H__