#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "Module.h"
#include "Entity.h"
#include <vector>

struct EntityData
{
	Entity::Types type = Entity::Types::UNKNOWN;
	std::string tsxFile;
};

class EntityManager : public Module
{
public:

	EntityManager();
	~EntityManager();
	
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool UpdateAll(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateEntity(Entity::Types type, int PositionX, int PositionY, std::string name);
	void DestroyEntity(Entity* entity);

	void DrawDebugQuad(Entity*);

	static bool SortByYPos(const Entity* ent1, const Entity* ent2);

public:

	std::vector<Entity*> entities;

};

//struct Sort{
//	bool SortByYPos(const Entity * ent1, const Entity * ent2);
//}

#endif // __ENEMIES_H__
