#include "App.h"
#include "NPC.h"
#include "Render.h"
#include "Textures.h"

#include "Brofiler/Brofiler.h"

NPC::NPC(int x, int y, std::string name) : Entity(Types::NPC, x, y, name) {

	LoadEntityData("Assets/Entities/mudkip.tsx");

}

NPC::~NPC()
{
}

bool NPC::Start()
{

	currentAnim = &animIdle;
	currentAnim->speed = animationSpeed;

	data.tileset.texture = app->tex->Load(data.tileset.imagePath.data());

	SetPivot(7, 16);

	return true;
}

bool NPC::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	currentAnim->GetCurrentFrame(dt);

	return true;
}

// Called before quitting
bool NPC::CleanUp()
{
	return app->tex->UnLoad(data.tileset.texture);

}

void NPC::OnCollision(Collider* c1, Collider* c2, float dt) {
}

void NPC::PushBack() {

	for (uint i = 0; i < data.numAnimations; ++i) {
		for (uint j = 0; j < data.animations[i].numFrames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				animIdle.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}

void NPC::IdAnimToEnum()
{
	for (uint i = 0; i < data.numAnimations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void NPC::LoadProperties(pugi::xml_node &node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}