#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Animation.h"


struct SDL_Texture;

class NPC : public Entity
{

public:

	NPC(int x, int y, std::string name);

	~NPC();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);

private:

	EntityState state = EntityState::IDLE;

};

#endif 
