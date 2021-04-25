#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Window.h"

#include "Brofiler\Brofiler.h"

Map::Map() : Module(), mapLoaded(false)
{
	name.assign("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.assign(config.child("folder").child_value());

	return ret;
}

void Map::Draw()
{
	BROFILER_CATEGORY("Draw", Profiler::Color::Azure);

	if(mapLoaded == false)
		return;

	uint id = 0;
	uint tilesPainted = 0;

	for (std::list<MapLayer*>::iterator layer = data.layers.begin(); layer != data.layers.end(); ++layer) {
		if ((*layer)->visible && (*layer)->properties.draw) {
			for (uint i = 0; i < (*layer)->width; ++i) {
				for (uint j = 0; j < (*layer)->height; ++j) {
					id = (*layer)->Get(i, j);
					if (id != 0) {
						TileSet* tileset = GetTilesetFromTileId(id);
						if (tileset != nullptr) {
							if (app->render->IsOnCamera(MapToWorld(i, j).x, MapToWorld(i, j).y, tileset->tileWidth, tileset->tileHeight)) {
								tilesPainted++;
								app->render->Blit(tileset->texture, MapToWorld(i, j).x, MapToWorld(i, j).y, &tileset->GetTileRect(id));
							}
						}
					}
				}
			}
		}
	}

	static char title[30];
	sprintf_s(title, 30, " | Tiles Rendered: %u", tilesPainted);

	app->win->AddStringToTitle(title);

	if (drawGrid) {
		for (uint i = 0; i < data.width; ++i) {
			for (uint j = 0; j < data.height; ++j) {
					app->render->Blit(grid, MapToWorld(i, j).x, MapToWorld(i, j).y);
			}
		}
	}

}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* ret = *item;
	for (; item != data.tilesets.end(); ++item) {
		if (id >= (*item)->firstgid)
			ret = *item;
		else return ret;
	}
	return ret;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);


	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = int (data.tileWidth * 0.5F) * (x - y);
		ret.y = int (data.tileHeight * 0.5F) * (x + y);
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const {
	iPoint ret(0, 0);

	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = x / data.tileWidth + y / data.tileHeight;
		ret.y = 2 * y / data.tileHeight - ret.x;
		ret.x--; ret.y--;
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}


inline uint MapLayer::Get(int x, int y) const {
	return tiles[y * this->width + x];
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relative_id % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relative_id / numTilesWidth));
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator item = data.tilesets.begin();

	while(item != data.tilesets.end())
	{
		app->tex->UnLoad((*item)->texture);
		RELEASE(*item);
		++item;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::iterator item2 = data.layers.begin();
	
	while (item2 != data.layers.end()) {
		RELEASE(*item2);
		++item2;
	}
	data.layers.clear();

	// Remove all colliders
	std::list<ColliderObject*>::iterator item3 = data.colliders.begin();

	while (item3 != data.colliders.end()) {
		RELEASE(*item3);
		++item3;
	}
	data.colliders.clear();

	app->tex->UnLoad(grid);

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* fileName)
{
	bool ret = true;
	std::string tmp = (folder.data() + std::string(fileName));

	pugi::xml_parse_result result = mapFile.load_file(tmp.data());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", fileName, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer")) {

		MapLayer* lay = new MapLayer();

		if (ret == true)
			ret = LoadLayer(layer, lay);

		data.layers.push_back(lay);
	}

	

	//Load Object data ------------------------------------------------
	pugi::xml_node objectGroup;
	pugi::xml_node object;
	
	for (objectGroup = mapFile.child("map").child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
	{
		for (object = objectGroup.child("object"); object; object = object.next_sibling("object")) {

			ColliderObject* obj = new ColliderObject();
		
			if (ret == true && object != NULL) 
				ret = LoadObject(object, obj);
				
			data.colliders.push_back(obj);
		}
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", fileName);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tileWidth: %d tileHeight: %d", data.tileWidth, data.tileHeight);

		/*std::list<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		std::list<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		std::list<ColliderObject*>* item_object = data.colliders.start;
		while (item_object != NULL) {
			ColliderObject* obj = item_object->data;
			LOG("Object ------");
			LOG("name: %s", obj->name.GetString());
			LOG("Collider Position: (%i , %i)", obj->coll_x, obj->coll_y);
			LOG("width: %i  height: %i", obj->coll_width, obj->coll_height);
			item_object = item_object->next;
		}*/

	}

	if(data.type == MAPTYPE_ISOMETRIC)
		grid = app->tex->Load("Assets/Maps/quad_iso.png");
	else if(data.type == MAPTYPE_ORTHOGONAL)
		grid = app->tex->Load("Assets/Maps/quad_ortho.png");


	mapFile.reset();
	mapLoaded = ret;

	return ret;
}

// Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		LoadLayerProperties(map.child("properties"));
		std::string bgColor(map.attribute("backgroundcolor").as_string());

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;

		if(bgColor.length() > 0)
		{
			std::string red;
			std::string green;
			std::string blue;
			bgColor.substr(1, 2);
			bgColor.substr(3, 4);
			bgColor.substr(5, 6);

			int v = 0;

			sscanf_s(red.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.backgroundColor.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}

		LoadMapProperties(map.child("properties"));
	}

	return ret;
}

bool Map::LoadMapProperties(pugi::xml_node & node)
{
	for (node = node.child("property"); node != NULL; node = node.next_sibling()) {
		std::string prop = node.attribute("name").as_string();

		if (prop == "object_texture") {
			data.properties.objectsPath = node.attribute("value").as_string();
		}
	}
	return true;
}

void Map::LoadLayerProperties(pugi::xml_node& propertiesNode, MapLayer* layer) {

	for (propertiesNode = propertiesNode.child("property"); propertiesNode != NULL; propertiesNode = propertiesNode.next_sibling()) {
		std::string prop = propertiesNode.attribute("name").as_string();
		
		if (prop == "NoDraw") {
			layer->properties.draw = propertiesNode.attribute("value").as_bool(true);
		}
	}
}

bool Map::LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	set->name.assign(tilesetNode.attribute("name").as_string());
	set->firstgid = tilesetNode.attribute("firstgid").as_int();
	set->tileWidth = tilesetNode.attribute("tilewidth").as_int();
	set->tileHeight = tilesetNode.attribute("tileheight").as_int();
	set->margin = tilesetNode.attribute("margin").as_int();
	set->spacing = tilesetNode.attribute("spacing").as_int();


	pugi::xml_node offset = tilesetNode.child("tileoffset");

	if(offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

bool Map::LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tilesetNode.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = app->tex->Load(PATH(folder.data(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->texWidth = image.attribute("width").as_int();

		if(set->texWidth <= 0)
		{
			set->texWidth = w;
		}

		set->texHeight = image.attribute("height").as_int();

		if(set->texHeight <= 0)
		{
			set->texHeight = h;
		}

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	if (node.empty())	ret = false;

	layer->name.assign(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->visible = node.attribute("visible").as_bool(true);

	//Load properties of layer
	pugi::xml_node n_property = node.child("properties");
	LoadLayerProperties(n_property, layer);
	
	layer->tiles = new uint[layer->width*layer->height];

	memset(layer->tiles, 0, layer->width*layer->height * sizeof(uint));

	pugi::xml_node tile = node.child("data").child("tile");

	for (uint i = 0; i < layer->width*layer->height; i++) {
		layer->tiles[i] = tile.attribute("gid").as_uint();
		tile = tile.next_sibling("tile");
	}

	return ret;
}

bool Map::LoadObject(pugi::xml_node& nodeObject, ColliderObject* obj) {

	bool ret = true;
	if (nodeObject.empty())	ret = false;

	//Load Collider / Entity data
	obj->name = nodeObject.attribute("name").as_string();
	obj->entType = nodeObject.attribute("type").as_string();
	obj->tileId = nodeObject.attribute("id").as_uint();
	obj->collX = nodeObject.attribute("x").as_int();
	obj->collY = nodeObject.attribute("y").as_int();
	obj->collHeight = nodeObject.attribute("height").as_uint();
	obj->collWidth = nodeObject.attribute("width").as_uint();

	//Load Collider type from ObjectGroup
	pugi::xml_node objGroup = nodeObject.parent();
	std::string type(objGroup.child("properties").child("property").attribute("value").as_string());

	if (type == "COLLIDER_NONE")
	{
		obj->type = COLLIDER_NONE;
	}
	else if (type == "COLLIDER_FLOOR")
	{
		obj->type = COLLIDER_FLOOR;
	}

	return ret;
}