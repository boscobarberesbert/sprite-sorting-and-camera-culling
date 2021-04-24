#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"
#include "Collision.h"


struct SDL_Texture;

enum PlayerState {
	IDLE = 0,
	WALKING,

	UNKNOWN
};

class Player : public Entity
{	

public:

	Player(int x, int y, std::string name);

	~Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void AddColliders();
	void ChangeState(PlayerState);
	void SetCollidersPos();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);
	void LoadCollider(pugi::xml_node&);

private:

	PlayerState state = IDLE;

	Animation anim_walking;

	Animation anim_default;
};

#endif 
