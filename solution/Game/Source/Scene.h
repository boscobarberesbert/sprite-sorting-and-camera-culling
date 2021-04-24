#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include <vector>

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& map);

	// Called before the first frame
	bool Start();

	void CreateEntities();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool entitiesBox = false;

private:
	std::vector<std::string> scenes;
};

#endif // __SCENE_H__