# Who Am I
I am [Bosco Barber Esbert](https://es.linkedin.com/in/bosco-barber-esbert-b13876201), student of the [Bachelor's degree in Video Game Design and Development](https://www.citm.upc.edu/ing/estudis/grau-videojocs-bcn/) by [UPC](https://www.upc.edu/en) at [CITM](https://www.citm.upc.edu/). This content is developed for the second year’s subject Project II, under supervision of the lecturer [Ramón Santamaría](https://es.linkedin.com/in/raysan).

# First things first
In this website you will find information about Sprite Sorting and Camera Culling and how to implement them in your project. You can also visit the [main GitHub repository page](https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling) where you will be able to find two Visual Studio projects, one with the Sprite Sorting and Camera Culling systems fully implemented and another one with some parts missing to help you practice.

# Sprite Sorting

## What is Sprite Sorting?

As we all know, the start of video games was defined by conceptual graphics that barely conveyed the mechanics of them. Although that, the need and desire to represent reality has always been there from the beginning. This is the path that many developers of the time took for their projects, and nowadays the main focus of game development is still based on the representation of not only realistic but also extraordinary worlds that can work in our minds, more than conceptual games.

The first step was to develop 2D perpendicular games, but possibilities were far beyond that. With the evolution of 2D games, the desire to represent a 3D world in 2D was increasing. This is when we went from side and overhead (perpendicular) view games to 2.5D, top-down (3/4) and isometric games. In the last two cases, it appears a problem, sprite overlapping. Since the sprites order on those games change depending on the game state and the depth of the objects, we will need to be able to sort sprites dynamically.

When we are talking about rendering sprites, we know that the compiler will have to render the images in a certain order, as it can’t render everything at the same time. This causes that if two sprites overlap each other maybe the compiler will render it in the opposite order we want it to work, making the sprite that is supposed to be at the back to be in front of the other one. Depending on the game, to solve that we can manually code which of the sprites we want to render first, but in other cases we may have games that require to change the render order in real time, so we need to make use of different automatic Sprite Sorting systems to help us choose which sprite must be on top.

In games like _Super Mario_ or _Grand Theft Auto_ we can identify a sprite ordering without considering the depths, for example in _Super Mario_ we can render the turtle before Mario or vice versa, it just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.
For the same reason, _Grand Theft Auto_ does not have to sort sprites. We can follow the order of sprites like this: background->furniture->enemies->guns->player.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/super_mario.gif?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/grand_theft_auto.gif?raw=true" width="500">

On the other hand, we have games like _The Legend of Zelda_ and _Pokémon_, that are a good example of the beginning of sorting sprites in video games.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/the_legend_of_zelda.gif?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pokemon.gif?raw=true">

## Why is Sprite Sorting so important?

When making a top-down view game, for example, we need to create the feeling that the game has depth when it’s actually a 2D game. To do that we have to take into account the perspective and the enviroment, so if for example the player goes behind a tree in the map, we don't want to print the player on top of the tree, we want to print the player first and the tree afterwards so the tree sprite gets to be on top of the player sprite.

Since we can't be predicting all the time where the player is going to go to, doing it manually is not an option anymore for these kinds of games, and that’s where automatic sprite sorting systems take action and help us to have the sprites properly rendered at each moment of the game.

As we can see in this example, the different entities' sprites are properly sorted taking into account the top-down view. The higher the entity is on screen, the sooner it is rendered in order to convey the illusion of depth. That's why the player located lower on screen overlap the rest. We can see it because each one's heads overlap the feets of the next one.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/sprite_sorting_example.png?raw=true">

Sprite ordering might be like this:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/sprite_sorting.gif?raw=true">

## Different approaches by different games

There are some systems to sort sprites, it depends on the type of game, the resources of the machine and the code structure.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_3.jpg?raw=true">

### Cut Sprites

This is the laziest way to solve the sorting sprites problem, but also the one that can become a slow and cumbersome way in the long run. Although that, it can serve ample in many cases. It consists in separating a sprite in two parts, the lower part and the higher part. So, the core of the system is to render first the lower part, later all the entities, and finally the higher part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources. You can see the separated layers and the result, tinted to see where the cut is.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_5.png?raw=true" width="500">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_2.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_6.gif?raw=true">

### Sorting layers

This is the most common system used, but it could be used with different approaches.

#### By Position

That consists in sorting entities depending on the position of an entity. It is only focused on the vertical position (Y). In order to make sense of depth, all entities and objects will be sorted by Y position, from the lower Y to the higher Y, from top of the window to down. Entities placed higher will be rendered before entities placed lower. We can see this example of _Chrono Trigger_ that uses this system.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/chrono_trigger_example.png?raw=true" width="500">

Although that, since we may have sprites with different sizes (widths and heights), if we only take into account the vertical position we can get a bad sorting. That's why sometimes we may need to modify it a little bit. This can be done taking into account not only the position but also the height of the sprite. In some cases, if we want to be more accurate with the sorting or if we had an special case in which we wanted to sort also by horizontal position, we can set a pivot on each object, and sort all the sprites depending on it.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_1.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_4.png?raw=true">

That could consume more resources than we expected, because we must sort a lot of objects. Remember that we are doing it each and every frame with all the objects and entities we have, including static and dynamic entities. Although that, if this approach is selected there is no other way to do it, because remember that not only the player can change its position, but also the enemies, NPCs or any other element we have around the map; and this has to be taken into account and sorted every frame. In order to optimize, we have implemented a camera culling, in order to sort and render only the entities on camera (on screen). Also, it depends on the entity types and how are saved. The sorting method also influences.

#### By Colliders

Sorting layers by colliders is not the ideal way to sort sprites, and it is not widely used but in some cases it's the only way to get a good result. I would recommend it for more specific situations in which we have some complex structure that requires a different sorting not only depending on depth (having different floors with different heights for example). It is based on putting colliders in different zones that on the one hand change the layer of the entity regarding the elements of the map, and on the other hand they activate or deactivate the colliders that define the path that players can take.

A good example for this, is a video made by [Guinxu](https://www.youtube.com/user/GuinxuVideos) (a spanish indie game developer) that we can find [here](https://youtu.be/eMMnaUmWtnw?t=85) (in spanish). In order to explain it, I will get some screenshots and I will explain how it works.

First, the problem is that the player will have to be able to pass under the bridge and to pass above.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/colliders_example_1.png?raw=true" width="500">

To do that, Guinxu solved the problem putting up two types of colliders. One type made the player be under the bridge, and the other vice versa, so, when player goes over the bridge, the last collider that touches is the red (up arrow) and the player layer moves higher than bridge, when he comes out, the player touches the blue collider (down arrow) and moves the player layer below the bridge. Also, that colliders with arrows activate or deactivate colliders that let the player pass or not. For example, if the player is going below bridge, he cannot pass for the left and right like if he is passing above bridge, and the same case when the player is going above the bridge, he cannot be able to jump across the bridge.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/colliders_example_2.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/colliders_example_3.png?raw=true">

#### By Vector 3D

This approach is common in isometric maps because it looks like a 3D environment. Although that, it's the most difficult way to implement. We must use 3 dimensions in order to get well the sorting of sprites. To render objects, we will have to project the vector3 to 2D. It is the most tricky and complex way to have sprite sorting. It is also the best way to sort sprites in isometric maps.

# Camera Culling

Camera culling is a basic method that allows the program to only work with entities and objects that are on the camera viewport. This is used to save resources to the machine, only rendering the tiles and sprites that are on screen. It will also be an advantage for the sprite sorting system, because this way we will only manage and sort the sprites of the entities and the objects that are in the camera viewport. It has no sense to render or sort elements that are not on screen at the moment, the only thing it would do is to consume resources unnecessarily. It helps especially in games with large worlds and a lot of entities to render. We will see the implementation and effect in code later. The only thing we have to do is to check if what we are going to render or sort is inside the camera or not, and this can be done just by checking if the rectangle of the camera is intersecting with the rectangle of the tile, entity or object in question.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/camera_culling.jpg?raw=true">

# Selected approach

In my case, I will sort layers by position, but with a modification. We won't operate with layers, we will operate directly with a list of entities. We will have an entity type that will be ```STATIC```, which will do nothing. I decided to do it like this because Tiled works only with objects with one tile, and this way we will be able to load bigger objects and link them easily with the sprite sorting system implemented. We will see the integration I have done from Tiled. Also, we will implement a camera culling to prevent rendering tiles that are not on the camera viewport. Moreover, it will only sort and render the sprites of the entities and objects on screen.

The result that we want to achieve with this project is something like this:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/project_result.gif?raw=true">

As we can see, the player moves around objects and the program manages the render order. On the title we can see the information of how many tiles are being rendered and how many entities are being sorted and rendered. In my case, I used a fictional pivot to sort entities; entities with pivot above will be rendered before entities with pivot below.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/project_result_debug.JPG?raw=true">

The pivot is the green rectangle in every entity.

## Profiling

We will see a radical change in the performancing of the program.

If we check with the profiling the performance of that scene during execution:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/profiling_1.png?raw=true">

We see that there is a lot of time wasted in rendering and sorting sprites. And now, here we have a profiling of the same scene but after making the research implementation:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/profiling_2.png?raw=true">

As we can see, there is a huge difference. It is still a lot of time but we will se later how to optimize that.

## Working with Tiled

In order to work with Tiled easily, I have implemented code to import entities and objects to the game. That works from loading object layers in Tiled. This way, we will be able to design our maps and place the entities directly in Tiled, but without having to worry if there's some object that occupies different tiles. By doing this, we can have the different elements of the map that we want to be sorted in one single sprite, and they can be easily managed by the sprite sorting system. Now I will explain how it works.

### Importing dynamic entities from Tiled

We can work with tilesets, tilemaps and spritesheets in Tiled. It allows us some functionalities of which we can take advantage. The only thing we need to do is to study what it provides us and incorporate it to our code.
First, there is the main information of the tileset that we can see on the _Properties_ window.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/player_tsx_properties.JPG?raw=true">

Here we have some general information about the tileset. The most important are:
* Name
* Orientation
* Columns
* Image
  * Source
  * Tile Width
  * Tile Height
  * Margin
  * Spacing

All these variables will be important to import to the program. This is something very important: _Custom Properties_. Here we can assign every variable we want to the code and edit so fast. In my example I use _AnimationSpeed_ but it can be used to many things. It can be of different types: int, float, string, bool...

It is also a powerful tool to implement animations easily. All we have to do is to pick the camera icon, set a reference tile and drag it to the box to set the animation of an action. Each tile has an id that we will use later to assign the animation.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/player_tsx.png?raw=true">

We can also set many colliders and load them after in code, but it won't affect to the research, so we won't touch that utility.

After we save the file, we will get something like that:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.2" name="Player" tilewidth="14" tileheight="21" tilecount="9" columns="3">
 <properties>
  <property name="AnimationSpeed" type="float" value="5"/>
 </properties>
 <image source="player.png" width="42" height="63"/>
 <tile id="0">
  <animation>
   <frame tileid="0" duration="200"/>
  </animation>
 </tile>
 <tile id="3">
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="3" duration="200"/>
   <frame tileid="0" duration="200"/>
   <frame tileid="6" duration="200"/>
  </animation>
 </tile>
</tileset>
```

Here we have in an XML the general information, properties and animations.

Now, in the code we will create some structs to save the data. We will create an entity class and then all the entities that have an special behaviour will inherit from it.
We will create a basic structure that saves the tileset of the entity and the animations.

```cpp
struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint numAnimations = 0;
};
```

Here we have the struct to save the tileset of an entity and all its properties.

```cpp
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
```

For the tileset we will save some information to load the texture. ```SDL_Rect GetTileRect(int id) const;``` returns a rect given an id. For example, id 0 is the first tile, the player looking down, so it will fill the rect with the tileset information given the width and height, and the position where it is in the texture.

```cpp
// Get the rect info of an id of tileset
SDL_Rect TileSetEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}
```

On animations we will save the id where they come from, the number of frames, the position of frames on the texture and the type of the animation. ```uint FrameCount();``` is a simple function that iterates all xml nodes and returns how many frames the animation has.

```cpp
struct EntityAnim {
	uint id = 0;
	uint numFrames = 0;
	SDL_Rect* frames = nullptr;
	EntityState animType;

	uint FrameCount(pugi::xml_node&);
};
```

We won't use colliders, but I will provide the struct to save the information if needed. We must save the collider, the offset from the entity position, the size and the type.

```cpp
struct ColliderInfo {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	ColliderType type;
};
```

In order to load all this information, we will have some functions, some of them will be virtual because every entity will have its animations and properties.

```cpp
bool LoadEntityData(const char*); // Loads entity by tsx file
// Virtual functions because every entity has its properties, variables, animations... ----------------------
virtual void LoadProperties(pugi::xml_node&);
virtual void LoadCollider(pugi::xml_node&);
virtual void IdAnimToEnum();
virtual void PushBack() {};
virtual void AddColliders(Entity* c = nullptr);
//-----------------------------------------------------------------------------------------------------------
```

```LoadEntityData()``` will contain all others functions. First, we save the tileset information:

```cpp
// Fill tileset info
pugi::xml_node node = entityFile.child("tileset");

data.tileset.name.assign(node.attribute("name").as_string());
data.tileset.tileWidth = node.attribute("tilewidth").as_uint();
data.tileset.tileHeight = node.attribute("tileheight").as_uint();
data.tileset.spacing = node.attribute("spacing").as_uint();
data.tileset.margin = node.attribute("margin").as_uint();
data.tileset.tileCount = node.attribute("tilecount").as_uint();
data.tileset.columns = node.attribute("columns").as_uint();
data.tileset.imagePath = folder += node.child("image").attribute("source").as_string();
data.tileset.width = node.child("image").attribute("width").as_uint();
data.tileset.height = node.child("image").attribute("height").as_uint();

size = iPoint(data.tileset.tileWidth, data.tileset.tileHeight);
```

Later we count how many animations there are and reserve memory for all of them.

```cpp
// Count how many animations are in file
node = node.child("tile");
data.numAnimations = 0;
while (node != NULL) {
	data.numAnimations++;
	node = node.next_sibling("tile");
}

// Reserve memory for all animations
data.animations = new EntityAnim[data.numAnimations];
```

Now, we want to save the number of frames of each animation.

```cpp
// Count how many frames for each animation, assign memory for those frames and set id frame start
node = entityFile.child("tileset").child("tile");
for (uint i = 0; i < data.numAnimations; ++i) {
	data.animations[i].FrameCount(node.child("animation").child("frame"));
	data.animations[i].frames = new SDL_Rect[data.animations[i].numFrames];
	data.animations[i].id = node.attribute("id").as_uint();
	node = node.next_sibling("tile");
}

// Fill frame array with current information
node = entityFile.child("tileset").child("tile");
pugi::xml_node nodeFrame;
for (uint i = 0; i < data.numAnimations; ++i) {
	nodeFrame = node.child("animation").child("frame");
	for (uint j = 0; j < data.animations[i].numFrames; ++j) {
		data.animations[i].frames[j] = data.tileset.GetTileRect(nodeFrame.attribute("tileid").as_uint());
		nodeFrame = nodeFrame.next_sibling("frame");
	}
	node = node.next_sibling("tile");
}
```

Later we save the properties.

```cpp
LoadProperties(entityFile.child("tileset").child("properties").child("property")); // Load properties, is a virtual function because every entity has its variables
```
An example of loading properties is:

```cpp
void Player::LoadProperties(pugi::xml_node &node) {
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}
```

Later we load the collider.

```cpp
LoadCollider(entityFile.child("tileset").child("tile").child("objectgroup").child("object")); // Load collider
```

And there is an example:

```cpp
void Player::LoadCollider(pugi::xml_node &node) {
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
```

Now, we must convert id animations to enum animations. To do that we use a virtual function. An example may be:

```cpp
void Player::IdAnimToEnum() {
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
```

After getting all animations, we must make the pushback of the frames. An example:

```cpp
void Player::PushBack() {
	for (uint i = 0; i < data.numAnimations; ++i) {
		for (uint j = 0; j < data.animations[i].numFrames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				animIdle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				animWalking.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}
```

To finish, we have to delete all reserved memory that we won't use.

```cpp
// Deleting entity animation data already loaded in its corresponding animation variables
for (uint i = 0; i < data.numAnimations; ++i) {		// This block of code delete animation data loaded of xml,
	if (data.animations[i].frames != nullptr) {	// is in PushBack() because when load all animation in its
		delete[] data.animations[i].frames;	// corresponding variables, that data is useless.
		data.animations[i].frames = nullptr;
	}
}
if (data.animations != nullptr) {
	delete[] data.animations;
	data.animations = nullptr;
}
```

With that we have finished the load of an entity with Tiled.

## Importing static entities from Tiled

For static entities it is a little different. It could not be that automatic. But it is not difficult.

First, we must prepare the scene. We will work with three layers.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/tiled_layers.png?raw=true">

Background will contain all tiles that won't be affected by entities, the basic ground like sand and small stones. Now, the "Object" layer is useful to see where the objects on the scene will be. All objects will have to be in a single texture, working with an atlas texture of objects. It is so important to have the property ```NoDraw``` in off in order not to render it later. Finally, we have a layer called ```StaticObjects``` and here we will set all objects in scene. Here is an example of putting a building on scene:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/static_object_example.JPG?raw=true" width="500">

As we can see, we have to put the name and set the type to ```static```. We also have to fill all tiles that it occupies. Now we can pass to code.

We will create an entity, setting its position and name. Depending of the name we will assign a rect or another for the texture.

```cpp
Static::Static(int x, int y, std::string name) :Entity(Types::STATIC, x, y, name) {
	// Assign type of static entity, texture rect and pivot
	// Orthogonal map ------------------------
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
	// ...
	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);

	data.tileset.texture = app->tex->Load(app->map->data.properties.objectsPath.data());
```

And that is all. To render we will give the texture and the frame we have loaded. Remember that it is only useful if the object will do nothing about interaction, only will be there.

# Links to more documentation

If you want to know more about Sprite Sorting, here you have some links of interest:

* [Cut Sprites solution](https://trederia.blogspot.com/2014/08/z-ordering-of-sprites-in-tiled-maps.html)
* [Different approaches to Ordering by position](https://eliasdaler.wordpress.com/2013/11/20/z-order-in-top-down-2d-games/)
* [Sprite ordering and tiles](https://love2d.org/forums/viewtopic.php?t=77149)
* [Pocket City solution](https://blog.pocketcitygame.com/cheating-at-z-depth-sprite-sorting-in-an-isometric-game/amp/)

In that case, I will separate links in two sections, because sorting in isometric view could be tricky in some cases:

* [Isometric with objects with more than one tile](https://gamedev.stackexchange.com/questions/103442/how-do-i-determine-the-draw-order-of-isometric-2d-objects-occupying-multiple-til)
* [Isometric sprite ordering with Tiled](https://discourse.mapeditor.org/t/isometric-depth-sorting/736)
* [Drawing isometric boxes in the correct order](https://shaunlebron.github.io/IsometricBlocks/)
* [Isometric depth sorting with big objects](https://stackoverflow.com/questions/11166667/isometric-depth-sorting-issue-with-big-objects)
* [Optimization of sorting sprites in isometric](https://gamedev.stackexchange.com/questions/97442/sorting-sprites-layers-in-isometric-view)
* [Sort with Z-buffer and anchor point, isometric map](https://gamedev.stackexchange.com/questions/69851/isometric-map-draw-sort-with-z-buffer-and-anchor-point)
* [Isometric sort algorithm](https://gamedev.stackexchange.com/questions/8151/how-do-i-sort-isometric-sprites-into-the-correct-order)

# TODOs and Solution

## TODO 1: Create IsOnCamera function

### Explanation
You have to pass to the function a rectangle to determine if it is on camera or not. It is quite important to pass a rectangle, not only a position because tiles and objects have a width and a height. The functioning of the camera could be different, in my case I had to put it in negative position to make it work well. Also, if there is something related to the scale of the pixels you have to put it to be able to know if it is real on camera or not. In my case I am using SDL and that library has a function to know if two rectangles are or not intersecting, ```SDL_HasIntersection```.

### Test
You cannot test if it works until the next TODO.

## TODO 2: Use previous function to only render tiles on camera

### Explanation
Now it is the time to know if it works. You have to pass the position of the tile in pixels and the width and the height of the tiles of the map.

### Test
You can test it moving camera in all directions and looking if the Tile count on the title changes.

## TODO 3: Create a building on Tiled and integrate it in code

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/building.JPG?raw=true">

### Explanation
You must follow the steps we have explained above this to create a static entity. In that case we will create a building. Is quite simple, first put it on Tiled and later follow the same structure that other objects.

### Test
In theory, if it has been well done, it will be where you put on the map. If it's not, look carefully the steps and see if there is any LOG message.

## TODO 4: Save entities on camera during update iteration in the _drawEntities_ vector and iterate after update iteration

### Explanation
We will start sprite ordering area. You must move draw functions to another iteration. You have a vector called ```drawEntities``` that you must use to iterate. During Update() iteration you have to push back entities on camera. Later, iterating that vector, you will have to draw entities.

### Test
You can move the camera out of the map and see if the entity count is 0 or not.

## TODO 5: Use std::sort(Iterator first, Iterator last, Compare comp) before iterating _drawEntities_.

### Explanation
Once you have only entities on camera, you must sort them in order to render before entities above. To do that there is a function in the ```<algorithm>``` library to sort vectors. The function is sort() and you have to pass the beginning and the end of a vector. For that case we also have to pass a function, it is created on ```EntityManager::SortByYPos``` and sorts the position of an entity.

### Test
There will be some buildings and flora that will be correctly sorted. You can also move the player for the scene and see if it is sorting by the position.

## TODO 6: Add the pivot position to general position

### Explanation

Sorting sprites by Y position could be enough, but if we set the point where it will be the sorting will be much better. Just add the pivot y position to its entity.

### Test

Moving player will sort better some objects.

## Solutions

### TODO 1

```cpp
bool Render::IsOnCamera(const int& x, const int& y, const int& w, const int& h) const {
	int scale = App->win->GetScale();

	SDL_Rect r = { x*scale,y*scale,w*scale,h*scale };
	SDL_Rect cam = { -camera.x,-camera.y,camera.w,camera.h };

	return SDL_HasIntersection(&r, &cam);
}
```

### TODO 2

```cpp
if (App->render->IsOnCamera(MapToWorld(i, j).x, MapToWorld(i, j).y, data.tileWidth, data.tileHeight)) {
```

### TODO 3

```cpp
else if (name == "building_3") {
	type = Static::Type::BUILDING;
	SetRect(128, 169, 48, 41);
	SetPivot(24, 39);
}
```

### TODO 4

```cpp
for (std::vector<Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) {
	if (*item != nullptr) {
		ret = (*item)->Update(dt);
			
		if (App->render->IsOnCamera((*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y)) {
			drawEntities.push_back(*item);
		}
	}
}
	
for (std::vector<Entity*>::iterator item = drawEntities.begin(); item != drawEntities.end(); ++item) {
	(*item)->Draw();
	entitiesDrawn++;

	if (App->scene->entitiesBox) {
		DrawDebugQuad(*item);
	}
}
```

### TODO 5

```cpp
std::sort(drawEntities.begin(), drawEntities.end(), EntityManager::SortByYPos);
```

### TODO 6

```cpp
static bool SortByYPos(const Entity* ent1, const Entity* ent2) {
	return ent1->pivot.y + ent1->position.y < ent2->pivot.y + ent2->position.y;
}
```

# Issues

* Objects in isometric with more than one tile of size will have problems to order. In many cases, the best thing to do is to separate objects in one by one tile.

# Improvements

* Loading textures is so expensive and now we have a texture loaded for every entity. A possible solution is to have only the textures we will use, and set by an id the texture that you need. For example texture 0 to background, 1 to player, 2 to objects and 3 to npc's.
* There are a lot of sorting methods and a lot of type of containers. Sorting vectors doing sort() function is expensive. You have to study which container and sorting method adjust better to your game. Here you have some links to different types of sorting methods.
[Brilliant.org](https://brilliant.org/wiki/sorting-algorithms/)
[Geeksforgeeks.org](https://www.geeksforgeeks.org/sorting-algorithms/)
* Iterate all map and know if that tile is on camera is so expensive when map is so big. In order to improve that, you can iterate the map from the beginning of the camera position to the camera position plus viewport size.
* The way that we implement static entities is simple but it is not automatic and it is ugly to see hardcode. First improve is to set all static entities information in a XML file. Second, you can investigate a way to load pivot and frame of static entities in Tiled.

# Author

All the project has been done by Bosco Barber Esbert. You can find me in:
* Github: [boscobarberesbert](https://github.com/boscobarberesbert)
* Contact: boscobarberesbert@gmail.com
