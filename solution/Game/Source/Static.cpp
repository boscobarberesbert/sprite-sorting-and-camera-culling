#include "Static.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"
#include "Map.h"

Static::Static(int x, int y, std::string name) :Entity(Types::STATIC, x, y, name)
{
	//Assign type of static entity, texture rect and pivot
	//Orthogonal map ------------------------
	if (name == "building_0") {
		type = Static::Type::BUILDING;
		SetRect(0, 144, 64, 84);
		SetPivot(32, 80);
	}
	else if (name == "building_1") {
		type = Static::Type::BUILDING;
		SetRect(64, 176, 32, 34);
		SetPivot(16, 32);
	}
	else if (name == "building_2") {
		type = Static::Type::BUILDING;
		SetRect(96, 176, 32, 35);
		SetPivot(16, 32);
	}
	else if (name == "building_3") {
		type = Static::Type::BUILDING;
		SetRect(128, 169, 48, 41);
		SetPivot(24, 39);
	}
	else if (name == "rock_0") {
		type = Static::Type::ROCK;
		SetRect(183, 67, 35, 29);
		SetPivot(17, 29);
	}
	else if (name == "rock_1") {
		type = Static::Type::ROCK;
		SetRect(113, 52, 14, 12);
		SetPivot(7, 12);
	}
	else if (name == "rock_2") {
		type = Static::Type::ROCK;
		SetRect(128, 48, 16, 16);
		SetPivot(8, 16);
	}
	else if (name == "rock_3") {
		type = Static::Type::ROCK;
		SetRect(144, 54, 16, 10);
		SetPivot(8, 10);
	}
	else if (name == "cactus_1") {
		type = Static::Type::CACTUS;
		SetRect(48, 48, 16, 16);
		SetPivot(8, 16);
	}
	else if (name == "cactus_2") {
		type = Static::Type::CACTUS;
		SetRect(67, 52, 11, 12);
		SetPivot(5, 12);
	}
	else if (name == "cactus_3") {
		type = Static::Type::CACTUS;
		SetRect(80, 48, 16, 16);
		SetPivot(8, 16);
	}
	else if (name == "cactus_4") {
		type = Static::Type::CACTUS;
		SetRect(97, 50, 14, 14);
		SetPivot(7, 14);
	}
	else if (name == "palm_tree_1") {
		type = Static::Type::CACTUS;
		SetRect(164, 55, 9, 8);
		SetPivot(4, 8);
	}
	else if (name == "palm_tree_2") {
		type = Static::Type::CACTUS;
		SetRect(177, 48, 13, 16);
		SetPivot(6, 16);
	}
	else if (name == "palm_tree_3") {
		type = Static::Type::CACTUS;
		SetRect(194, 42, 14, 22);
		SetPivot(7, 22);
	}
	else if (name == "mountain_0") {
		type = Static::Type::MOUNTAIN;
		SetRect(0, 64, 48, 80);
		SetPivot(24, 80);
	}


	//Isometric map--------------------------
	else if (name == "pilar") {
		type = Static::Type::PILAR;
		SetRect(256, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "corner") {
		type = Static::Type::CORNER;
		SetRect(64, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_u_r") {
		type = Static::Type::WALL_UP_RIGHT;
		SetRect(0, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_u_l") {
		type = Static::Type::WALL_UP_LEFT;
		SetRect(384, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_d_r") {
		type = Static::Type::WALL_DOWN_RIGHT;
		SetRect(448, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_d_l") {
		type = Static::Type::WALL_DOWN_LEFT;
		SetRect(192, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "door") {
		type = Static::Type::DOOR;
		SetRect(128, 0, 64, 64);
		SetPivot(32, 48);
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);


	if (app->map->data.type == MAPTYPE_ISOMETRIC) {
		position.x /= app->map->data.tileWidth * 0.5f;
		position.y /= app->map->data.tileHeight;

		iPoint pos = app->map->MapToWorld(position.x + 1, position.y + 1);
		position.Create(pos.x, pos.y);
	}

	data.tileset.texture = app->tex->Load(app->map->data.properties.objectsPath.data());
}

Static::~Static()
{
}

void Static::Draw()
{
	app->render->Blit(data.tileset.texture, position.x, position.y, &frame);
}

void Static::SetRect(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}
