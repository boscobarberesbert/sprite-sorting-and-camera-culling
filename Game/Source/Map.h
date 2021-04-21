#ifndef __MAP_H__
#define __MAP_H__

#include <list>
#include <string>
#include "Module.h"
#include "Collision.h"

struct SDL_Texture;

// ----------------------------------------------------

struct ColliderObject {

	std::string name;
	std::string entType;

	ColliderType type;
	uint tileId;
	int	collX;
	int	collY;
	int collWidth = 0;
	int collHeight = 0;	

};

struct LayerProperties
{
	bool draw = true;
};

struct MapProperties
{
	std::string objectsPath;
};

struct MapLayer {
	std::string	name;
	uint		width = 0;
	uint		height = 0;
	uint*		tiles = nullptr;
	bool		visible = true;
	LayerProperties	properties;

	~MapLayer() { delete[] tiles; tiles = nullptr; }

	inline uint Get(int x, int y) const;
};
	
// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tileWidth;
	int					tileHeight;
	SDL_Texture*		texture;
	int					texWidth;
	int					texHeight;
	int					numTilesWidth;
	int					numTilesHeight;
	int					offsetX;
	int					offsetY;

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int							width;
	int							height;
	int							tileWidth;
	int							tileHeight;
	MapProperties				properties;
	SDL_Color					backgroundColor;
	MapTypes					type;
	std::list<TileSet*>			tilesets;
	std::list<MapLayer*>		layers;
	std::list<ColliderObject*>	colliders;
};

// ----------------------------------------------------
class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	
	//void AddCollidersMap();
	

private:

	bool LoadMap();
	bool LoadMapProperties(pugi::xml_node& node);
	bool LoadTilesetDetails(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tilesetNode, TileSet* set);
	bool LoadLayer(pugi::xml_node& layerNode, MapLayer* layer);
	bool LoadObject(pugi::xml_node& tilesetNode, ColliderObject* obj);
	void LoadLayerProperties(pugi::xml_node & propertiesNode, MapLayer* layer = nullptr);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData			data;
	std::string		sceneName;

	bool			drawGrid = false;
	
private:

	pugi::xml_document	mapFile;
	std::string			folder;
	bool				mapLoaded;
	SDL_Texture*		grid = nullptr;
	
};

#endif // __MAP_H__