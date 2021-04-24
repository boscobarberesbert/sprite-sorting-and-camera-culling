#include "App.h"
#include "Player.h"
#include "Textures.h"
#include "Collision.h"
#include "Input.h"

#include "Brofiler/Brofiler.h"

Player::Player(int x, int y, std::string name) : Entity(Types::PLAYER, x, y, name) {

	LoadEntityData("Assets/Entities/player.tsx");

}

Player::~Player()
{
}

bool Player::Start()
{

	state = IDLE;

	currentAnim = &animIdle;
	currentAnim->speed = animationSpeed;

	AddColliders();

	data.tileset.texture = app->tex->Load(data.tileset.imagePath.data());

	SetPivot(7, 21);

	return true;
}

bool Player::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);
	int speed = 125;
	fPoint prev_pos = position;

	if (app->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		position.x += speed * dt;
		ChangeState(WALKING);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
		position.x -= speed * dt;
		ChangeState(WALKING);
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT) {
		position.y += speed * dt;
		ChangeState(WALKING);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT) {
		position.y -= speed * dt;
		ChangeState(WALKING);
	}

	if (prev_pos == position) {
		ChangeState(IDLE);
	}

	currentAnim->GetCurrentFrame(dt);

	return true;
}

// Called before quitting
bool Player::CleanUp()
{
	bool ret = false;
	ret = app->tex->UnLoad(data.tileset.texture);
	if (collider.collider != nullptr)
		collider.collider->to_delete = true;

	return ret;
}

void Player::OnCollision(Collider* c1, Collider* c2, float dt) {
}

void Player::PushBack() {

	for (uint i = 0; i < data.numAnimations; ++i) {
		for (uint j = 0; j < data.animations[i].numFrames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				animIdle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}

void Player::AddColliders() {
	SDL_Rect r;
	
	r = { (int)position.x + collider.offset.x,	(int)position.y + collider.offset.y, collider.width, collider.height };
	collider.collider = app->collider->AddCollider(r, collider.type, this);

}

void Player::SetCollidersPos() {
	if (collider.collider != nullptr)
		collider.collider->SetPos((int)position.x + collider.offset.x, (int)position.y + collider.offset.y);
}

void Player::IdAnimToEnum()
{
	for (uint i = 0; i < data.numAnimations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 3:
			data.animations[i].animType = EntityState::WALKING;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void Player::LoadProperties(pugi::xml_node &node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}

void Player::LoadCollider(pugi::xml_node &node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "Collider") {
			collider.offset.x = node.attribute("x").as_int();
			collider.offset.y = node.attribute("y").as_int();
			collider.width = node.attribute("width").as_uint();
			collider.height = node.attribute("height").as_uint();
			collider.type = ColliderType::COLLIDER_PLAYER;
		}

		node = node.next_sibling();
	}
}

void Player::ChangeState(PlayerState change_state) {

	if (change_state != state) {
		switch (change_state)
		{
		case IDLE:
			currentAnim = &animIdle;
			break;
		case WALKING:
			currentAnim = &anim_walking;
			break;
		default:
			break;
		}

		state = change_state;

		currentAnim->reset();
		currentAnim->speed = animationSpeed;
	}
}