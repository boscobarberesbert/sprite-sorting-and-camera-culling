# Who am I
I am [Bosco Barber Esbert](https://es.linkedin.com/in/bosco-barber-esbert-b13876201), student of the [Bachelor's degree in Video Game Design and Development](https://www.citm.upc.edu/ing/estudis/grau-videojocs-bcn/) by [UPC](https://www.upc.edu/en) at [CITM](https://www.citm.upc.edu/). This content is generated for the second year’s subject Project II, under supervision of the lecturer [Ramón Santamaría](https://es.linkedin.com/in/raysan).

# First things first
In this website you will find information about Sprite Sorting and Camera Culling and how to implement them in your project. You can also visit the [main GitHub repository page](https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling) where you will be able to find two Visual Studio projects, one with the Sprite Sorting and Camera Culling systems fully implemented and another one with some parts missing to help you practice.

This research will solve the following questions and provide help to implement them in your code:
* Do you want to change manually all your code every time you want to print something in a different order or you prefer an automatic method?
* It's necessary to print every tile, entity, object, particle or any other element that isn't seen on the screen?
* If I have two entities, one in each side of the screen, do you want to check if they are colliding? What if you have thousands of entities?
* This is the most important question: Do you want your game (or program) to run faster?

Now that you know that these topics are actually very important for a game to be optimized, let me explain them more in depth and give you a focused guide on how to implement these techniques in your code.

# Sprite Sorting

## What is sprite sorting?

As we all know, the start of video games was defined by conceptual graphics that barely conveyed the mechanics of them. Although that, the need and desire to represent reality has always been there from the beginning. This is the path that many developers of the time took for their projects, and nowadays the main focus of game development is still based on the representation of not only realistic but also extraordinary worlds that can work in our minds, more than conceptual games.

The first step was to develop 2D perpendicular games, but possibilities were far beyond that. With the evolution of 2D games, the desire to represent a 3D world in 2D was increasing. This is when we went from side and overhead (perpendicular) view games to 2.5D, top-down (3/4) and isometric games. In the last two cases, it appears a problem, sprite overlapping. Since the sprites order on those games change depending on the game state and the depth of the objects, we will need to be able to sort sprites dynamically.

When we are talking about rendering sprites, we know that the compiler will have to render the images in a certain order, as it can’t render everything at the same time. This causes that if two sprites overlap each other maybe the compiler will render it in the opposite order we want it to work, making the sprite that is supposed to be at the back to be in front of the other one. Depending on the game, to solve that we can manually code which of the sprites we want to render first (remember that the order of the images in the screen will appear in the inverse order that we call our function which prints or renders the image. This means that the first image we have sent to print will be the first image to be rendered), but in other cases we may have games that require to change the render order in real time, so we need to make use of different automatic Sprite Sorting systems to help us choose which sprite must be on top.

In games like _Super Mario_ or _Grand Theft Auto_ we can identify a sprite ordering without considering the depths, for example in _Super Mario_ we can render the turtle before Mario or vice versa, it just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.
For the same reason, _Grand Theft Auto_ does not have to sort sprites. We can follow the order of sprites like this: background->furniture->enemies->guns->player.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/super_mario.gif?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/grand_theft_auto.gif?raw=true" width="500">

On the other hand, we have games like _The Legend of Zelda_ and _Pokémon_, that are a good example of the beginning of sorting sprites in video games.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/the_legend_of_zelda.gif?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pokemon.gif?raw=true">

## Why is sprite sorting so important?

When making a top-down view game, for example, we need to create the feeling that the game has depth when it’s actually a 2D game. To do that we have to take into account the perspective and the enviroment, so if for example the player goes behind a tree in the map, we don't want to print the player on top of the tree, we want to print the player first and the tree afterwards so the tree sprite gets to be on top of the player sprite.

Since we can't be predicting all the time where the player is going to go to, doing it manually is not an option anymore for these kinds of games, and that’s where automatic sprite sorting systems take action and help us to have the sprites properly rendered at each moment of the game.

As we can see in this example, the different entities' sprites are properly sorted taking into account the top-down view. The higher the entity is on screen, the sooner it is rendered in order to convey the illusion of depth. That's why the player located lower on screen overlap the rest. We can see it because each one's heads overlap the feets of the next one.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/sprite_sorting_example.png?raw=true">

Sprite ordering might be like this:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/sprite_sorting.gif?raw=true">

## Different approaches by different games

There are some systems to sort sprites, it depends on the type of game, the resources of the machine and the code structure.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_3.jpg?raw=true">

## Z-order as a concept

### Basics

So, what is **z-order**?
Z-order is an ordering of overlapping 2d objects. Look at the picture:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/z-order.png?raw=true">

**Rectangle B** is drawn after **rectangle A**. The result is **rectB** is drawn “above” **rectA**. **RectB** is said to have higher z-order than **rectA**. Really simple.

### Z-order in games

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_1.png?raw=true">

_Screenshot from Legend of Zelda: A link to the Past (Nintendo, SNES)_

You can tell that Link is behind that tree. But this is actually an illusion of depth achieved by using clever z-ordering. The tree is drawn after Link, so it has higher z-order.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_2.png?raw=true">

And now Link is drawn after the tree. He has higher z-order and it looks like he’s standing closer to the camera. And here’s how you can achieve the same effect.

### Cut sprites

This is the laziest way to solve the sorting sprites problem, but also the one that can become a slow and cumbersome way in the long run. Although that, it can serve ample in many cases. It consists in separating a sprite in two parts, the lower part and the higher part. So, the core of the system is to render first the lower part, later all the entities, and finally the higher part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources. You can see the separated layers and the result, tinted to see where the cut is.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_5.png?raw=true" width="500">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_2.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_6.gif?raw=true">

This is actually how **A Link to the Past** does it(I don’t know about moving objects, but it surely uses layers for static objects. You can see it yourself if you swing your sword at different positions near houses, walls etc.).

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_3.png?raw=true">

_Layer approach: By cutting sprites. We have to take into account that this method is not automatic._

One sprite can be divided in two parts(as shown in the picture) which have different constant z-order. You draw all objects with **z = 0**, then with **z = 1**, etc.
This method works good and it’s easy to implement, but after a while it becomes hard to maintain, because every time you add a new sprite you have to divide it by parts and calculate areas which must be drawn before of after player and other objects.

And this can get quite complicated with lots of objects. And what about moving objects? If some enemy is circling around you, how can you find out who must have higher z-order?

### Sorting layers (dynamic)

This is the most common system used, but it could be used with different approaches.

#### By Position

That consists in sorting entities depending on the position of an entity. It is only focused on the vertical position (Y). In order to make sense of depth, all entities and objects will be sorted by Y position, from the lower Y to the higher Y, from top of the window to down. Entities placed higher will be rendered before entities placed lower. We can see this example of _Chrono Trigger_ that uses this system.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/chrono_trigger_example.png?raw=true" width="500">

Although that, since we may have sprites with different sizes (widths and heights), if we only take into account the vertical position we can get a bad sorting. That's why sometimes we may need to modify it a little bit. This can be done taking into account not only the position but also the height of the sprite. In some cases, if we want to be more accurate with the sorting or if we had an special case in which we wanted to sort also by horizontal position, we can set a pivot on each object, and sort all the sprites depending on it.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_1.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/pocket_city_4.png?raw=true">

This is where it gets smarter. First, let’s look at collision bounding boxes:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_4.png?raw=true">

Look at the Link’s bounding box. Its lowest Y coordinate is higher than tree’s and that’s why Link is drawn after the tree. And look at another situation. Now lowest Y coordinate of tree is higher, so you draw it after Link.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_5.png?raw=true">

So, that’s what you do:

**Sort all objects(or sprites) visible on the screen and sort this list by (boundingBox.top + boundingBox.height) value from lowest to highest and then draw objects in this order.**

If some object doesn’t have boundingBox, you can think like its **boundingBox.top + boundingBox.width equals 0**. 

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

#### Complex objects

Drawing something like arcs is trickier. It requires you to use two bounding boxes for each column. And how do you deal with the part which is above player?
You need to use z axis for this. “Z” has nothing to do with z-order in the context. It tells you how far from the ground is **boundingBox** of an object (**z = 0** is ground level). Here’s how the arc can be separated:

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/zelda_a_link_to_the_past_6.png?raw=true">

The algorithm is the same, but now you can sort by lowest Y coordinate for each Z and then combine them in one list sorted by Z. You’ll get something like this:

```cpp
objects = {

link, (z = 0, lowY = 64)

column_a, (z = 0, lowY = 100)

column_b, (z = 0, lowY = 100)

topPart (z = 1, lowY = 80)

}
```

So arc will consist from three parts and each will be drawn in needed order.

Separating by z axis is actually very useful, because, for example, objects with higher z value don’t have to be checked when you check collision with objects but that’s another thing to talk about. ;)

# Camera Culling

## What is camera culling?

Camera culling is a basic method that allows the program to only work with entities and objects that are on the camera viewport. This is used to save resources to the machine, only rendering the tiles and sprites that are on screen. It will also be an advantage for the sprite sorting system, because this way we will only manage and sort the sprites of the entities and the objects that are in the camera viewport. It has no sense to render or sort elements that are not on screen at the moment, the only thing it would do is to consume resources unnecessarily. It helps especially in games with large worlds and a lot of entities to render. We will see the implementation and effect in code later. The only thing we have to do is to check if what we are going to render or sort is inside the camera or not, and this can be done just by checking if the rectangle of the camera is intersecting with the rectangle of the tile, entity or object in question.

## Why is camera culling important?

As we mentioned before, camera culling is a way to optimize the game, by using this technique we are going to be able to increase the performance of our game as we are going to be loading less sprites and entities each cycle.

## How do we create a camera culling effect in our game?

To create a camera culling effect in our game is pretty simple, we just have to select what we want to render. To do that we are just going to add a filter to our rendering system. This filter will determine if something is outside the camera by checking if the rectangle of the element that we want to render intersects with the camera rectangle or not and once it has determined it, the sprite will be rendered or ignored. It is so useful for the map rendering, checking if the tile we are going to render is inside the camera or not; but not only that, it's also very useful for the sprite sorting I have explained before, because this way we won't only render the sprites that are on camera, but we will also avoid sorting sprites that are not on screen, which is also very nice for the game performance, especially when we have many entities at the same time. We will see later how to link these two techniques together and achieve to have sprite sorting and camera culling properly connected.

In order to do that we will have to implement a filter similar to the next one and apply it to the render function or the function that pushes the elements into the list if you have done the sprite ordering system before:

```cpp
if ((rect.x < -camera.x + camera.w && rect.x + rect.w > -camera.x) ||
(rect.x < -camera.x + camera.w  && rect.x + rect.w > -camera.x))
{
	if (rect.y < -camera.y + camera.h && rect.y + rect.h > -camera.y)
	{
		// Render
	}
}
else
{
	// Don't render
}
```

Anyway, as in this project we are using SDL, we will take advantage of it and in the implementation we will use the function ```SDL_HasIntersection``` that takes two rects as arguments and returns true if they intersect, or false if they don't. Obviously, one rectangle will be the camera and the other the one of the element we want to render.

<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/camera_culling_1.jpg?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/camera_culling_2.png?raw=true">

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

## Importing Static Entities from Tiled

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

## Quadtree

Once we have done that we will have our camera culling system implemented, the problem is that at this point it’s not going to be really efficient because we are going to be checking if every element on the map is inside of the camera and depending on the number of entities in the map, our game will have to do a lot of work.

Checking a big number of collisions every frame there is a big waste of resources for our game. Every frame, our computer is checking if one entity is colliding with all the rest of the entities and the same for each entity. For example, if we have 200 entities and we want to know how many entities our game is going to check every frame, the solution is 200x200=40.000 operations. It's a really big number of operations.

Here is the question: Do I have to calculate the collision between two objects that are very far one from each other? The answer is **NO** (if you are interested in optimizing your game).

### Space Partitioning

In order to reduce that amount of operations we can implement something called space partitioning. Space partitioning is a technique based on the idea of dividing the space in small cells so then we can check only the entities that are in the same cell. However, there is a problem with this method. If you have a lot of entities in the same rect or bunch of rects then you will have to calculate the collisions between a big number of entities again.

<p align="center">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/space_partitioning_1.png?raw=true" width="200">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/space_partitioning_2.png?raw=true" width="200">
</p>

### Quadtree

In order to solve this problem and to divide the cells into smaller pieces we will have to create a class called ```Quadtree```. Quadtrees are a type of space partitioning that will allow us to divide the space in 4 smaller fragments of equal parts in order to divide the space into even smaller pieces, this way we won’t have the problem of having many entities in the same quadtree if those are close enough because the quadtree will divide the space in smaller cells to separate the entities.
Basically, a quadtree is an other type of space partitioning, but instead of having a static grid of nodes or cells where the game calculates the collisions of all the entities that are inside, it generates automatically its own partitions. It's a tree data structure, which are one of the fastest data structures. Now I'm going to develop an accurate description of how it works.

* First we have a number of entities in our map, lets say 3. This number will be the maximum number of entities that can be inside a rect.
* If you insert another entity in the map, then the quadtree starts working. It will divide our initial rect in 4 smaller rects. Now, the game will only calculate the collisions of the entities that are in the same new rects.
* If you add more and more entities, the quadtree will be dividing more and more each rect in 4 smaller rects.

<em>Quadtree ramifications represented</em>
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_scheme.png?raw=true">

<em>Quadtree visual representation</em>
<p align="center">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_1.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_2.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_3.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_4.png?raw=true">
<img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_5.png?raw=true">
</p>

As you can see, we are generating smaller regions in order to reduce the amount of collisions checkings. This is a gif that shows how it works in real time.
<p align="center"><img src="https://github.com/boscobarberesbert/sprite-sorting-and-camera-culling/blob/master/docs/images/quadtree_example.gif?raw=true"></p>

### Quadtree in C++

Inside the quadtree we will need the following 5 functions:

* **Clear():** This function deletes all the entities inside the nodes.
* **Split():** This function divides the parent node into 4 children nodes when the current node reaches the maximum number of entities.
* **Insert():** This function inserts all the entities in their appropriate child and if it's necessary calls the Split() function.
* **ColliderList():** This function receives an entity and returns a list with all the entities that the collider manager will have to check.
* **CheckBoundaries()** Check if two rectangles are colliding.

As I've said before, a quadtree is a tree structure, which means that you start in a node and from that node you go to its childrens. The faster way to do it is using recursive functions. The class template will be something like this one:

```
class Quadtree
{
public:

	Quadtree(SDL_Rect rect)
	{
		Space = rect;
		for (int i = 0; i < children.size(); i++)
		{
			children[i] = nullptr;
		}
	}

	~Quadtree(){ Clear();}

	void Clear();
	void Split();
	bool insert();
	bool CheckBoundaries();
	list<Collider*> FillCollisionList();

public:
	
	SDL_Rect		space; 		// Rect that deffine the node space
	list<Collider*>		objects;	// A list to store all the entities
	array<Quadtree*,4>	children;	// An empty array for the childrens
};
```

Once we have the quadtree class we can start to implement it in the camera culling, to do so, we have to initialize a quadtree with a rect of the size of the map or the area where we want to apply it. Then we have to insert all the entities and finally call the ```PushCollisionVector``` with the camera rectangle. This way, it will check which elements are inside the camera and it will return them in a list, and that list will be the elements that will have to be printed.

Now we should be able to only render things inside the camera in an efficent way.

### Using The Quadtree with Camera Culling

If we want to implement our Quadtree class in a camera culling method, we have to follow these steps:
* Initialize the Quadtree with the map size rect.
* Insert all the entities inside the Quadtree using the Insert() function.
* Now that you have all the entities organized inside the quadtree, you can call the ColliderList(cameraRect) function with the camera rectangle. It will check where is the camera and with which rectangles it is colliding. Then the function will return a list of entities that are inside these rectangles. We have to check if an entity can be or not inside the camera. If they are inside the camera we will send it to the Priority queue and then we will render it!

## Other improvements

* Loading textures is so expensive and now we have a texture loaded for every entity. A possible solution is to have only the textures we will use, and set by an id the texture that you need. For example texture 0 to background, 1 to player, 2 to objects and 3 to npc's.
* There are a lot of sorting methods and a lot of type of containers. Sorting vectors doing sort() function is expensive. You have to study which container and sorting method adjust better to your game. Here you have some links to different types of sorting methods.
[Brilliant.org](https://brilliant.org/wiki/sorting-algorithms/)
[Geeksforgeeks.org](https://www.geeksforgeeks.org/sorting-algorithms/)
* Iterate all map and know if that tile is on camera is so expensive when map is so big. In order to improve that, you can iterate the map from the beginning of the camera position to the camera position plus viewport size.
* The way that we implement static entities is simple but it is not automatic and it is ugly to see hardcode. First improve is to set all static entities information in a XML file. Second, you can investigate a way to load pivot and frame of static entities in Tiled.

# Alternatives

When talking about sprite sorting, another algorithm that is also very used is the priority queue. In order to implement it, we have to use the Standard Template Library, which is going to let us use the ```priority_queue```. A priority queue is an abstract data type really similar to a normal queue but with the small difference that each elements inside of it has a certain priority. That means that the data that we insert inside the priority queue will be ordered from least to greatest according to the priority we gave each element. Basically, it reorders the elements that are stored inside with a predefined condition.

Now it's time to implement that in C++. The first thing that we have to do is to create a new class called "ObjectToPrint". The variables of this class will be the same as the parameters of your render funtion (position x and y, scale, rectangle...) and the new parameter Z or priority.

The ```priority_queue``` will need 3 parameters:

**1. [T] Type of elements in the queue.**

**2. [Container vector] Type of container to store the data.**

**3. [Compare] A binary predicate that takes two elements (of type T) as arguments and returns a bool. It determines which of the two elements has to go before the other.**

The syntaxis would look something like that:

```
std::priority_queue <class T, class Container = vector<T>, class Compare = less<typename Container::value_type> name;
```

Before implementing our priority queue we will have to create a class that will be the elements with all the data that we are going to print, a struct with a boolean operator that will compare 2 elements from the class that we previously created and will return true or false according to our priority condition.

We will have to define the function that compares the elements and the best way is to create an struct with a boolean operator that needs the type of elements that we want to compare, in this case the struct will be:

```
struct OrderPriority
{
    bool operator()(const ObjectToPrint* Object1, const ObjectToPrint* Object2)
    {
    	// Priority condition
    }
}
```

In the end, our priority queue will have this declaration:

```
std::priority_queue <ObjectToPrint*, vector<ObjectToPrint*>, OrderPriority> SpriteSorterQueue;
```

After doing that we will be able to implement the priority_queue with our 3 parameters:

**1. The class that we created.**

**2. A vector of the class that we created.**

**3. The struct that we created with the boolean opperator.**

With that we will be able to order our sprites, the only thing that is left to do is to create 2 functions.
The first function will have to create an element of the class we created and and push it into the priority queue.
The second function will be the one that will take the elements that are in the queue in the correct order, send them to the renderer and pop them form the queue.

Now that we have defined the priority queue we have to create a method that fills it and then, when all the frame logic has been done, print the sprites in the correct order. This can be done in two steps:
* **void FillQueue():** This function has to replace our render function, because now we are going to send the sprites to the queue before the rendering. This function has to have the same arguments as our render funtion. It has to create a new ```ObjectToPrint``` element and push it to the queue using SpriteOrdererQueue.push(ObjectToPrint*).

* **Render the Queue:** Once our priority queue has all your sprites in the correct order, it's time to render the sprites. You have to modify your Render function. Our render function now will recieve your priority queue as a parameter and it will have to render our sprites one by one while our queue is not empty. Remember to make pop of each element that has been printed. 

After that is done we will have our sprite ordering system. Now we should be able to see our sprites rendered as we wanted.

As we can see, the process is basically the same, the only thing that changes is the sorting algorithm of the abstract data type.

# Documentation and Reference links (Webgraphy)

## Sprite Sorting

If you want to know more about Sprite Sorting, here you have some links of interest:

* [Cut sprites solution](https://trederia.blogspot.com/2014/08/z-ordering-of-sprites-in-tiled-maps.html)
* [Different approaches to sorting by position](https://eliasdaler.wordpress.com/2013/11/20/z-order-in-top-down-2d-games/)
* [Sprite sorting and tiles](https://love2d.org/forums/viewtopic.php?t=77149)
* [Pocket City solution](https://blog.pocketcitygame.com/cheating-at-z-depth-sprite-sorting-in-an-isometric-game/amp/)
* [Depth sorting for Game Maker Studio](https://forum.yoyogames.com/index.php?threads/depth-sorting-method-for-gms2-objects-sprites.42868/)
* [Stack Overflow discussion about the sprite sorting idea](https://stackoverflow.com/questions/11002811/sorting-objects-by-y-value-before-rendering)
* [Images of The Legend of Zelda](https://eliasdaler.wordpress.com/2013/11/20/z-order-in-top-down-2d-games)

In that case, I will separate links in two sections, because sorting in isometric view could be tricky in some cases:

* [Isometric with objects with more than one tile](https://gamedev.stackexchange.com/questions/103442/how-do-i-determine-the-draw-order-of-isometric-2d-objects-occupying-multiple-til)
* [Isometric sprite sorting with Tiled](https://discourse.mapeditor.org/t/isometric-depth-sorting/736)
* [Drawing isometric boxes in the correct order](https://shaunlebron.github.io/IsometricBlocks/)
* [Isometric depth sorting with big objects](https://stackoverflow.com/questions/11166667/isometric-depth-sorting-issue-with-big-objects)
* [Optimization of sorting sprites in isometric](https://gamedev.stackexchange.com/questions/97442/sorting-sprites-layers-in-isometric-view)
* [Sort with Z-buffer and anchor point, isometric map](https://gamedev.stackexchange.com/questions/69851/isometric-map-draw-sort-with-z-buffer-and-anchor-point)
* [Isometric sorting algorithm, maths for isometric view](https://gamedev.stackexchange.com/questions/8151/how-do-i-sort-isometric-sprites-into-the-correct-order)

### Another useful algorithm we could implement to sort sprites

* [Cplusplus priority_queue wiki](http://www.cplusplus.com/reference/queue/priority_queue/)
* [cppreference priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue)
* [Video: priority_queue explanation](https://www.youtube.com/watch?v=wptevk0bshY)

## Camera Culling

* [Camera culling](https://www.youtube.com/watch?v=zCaurIC49I4)
* [Camera culling idea](https://www.youtube.com/watch?v=zCaurIC49I4)

### Spatial partition and Quadtree

* [Spatial partition:](https://www.youtube.com/watch?v=ELUZ60zZJzM) In this video you will find a visual representation of how spatial partitioning works.
* [Quadtree Demo:](https://google.github.io/closure-library/source/closure/goog/demos/quadtree.html) In this website you will be able to play around with a representation of a quadtree.
* [Spatial partition and quadtree idea](https://youtu.be/RN1GRX2ByLM)
* [Quadtree explanation and images](https://www.genbeta.com/desarrollo/teoria-de-colisiones-2d-quadtree)
* [Quadtree explanation and pseudocode](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)
* [C++ Quadtree Class repository](https://github.com/MarcusMathiassen/P2D/tree/master/src)
* [Quadtree explanation](https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det)
* [Quadtree collision optimization:](https://www.youtube.com/embed/TJzq_kjtGTc) This video shows how many times we reduce the amount of collisions checkings.
* [Easy way to create your Quadtree class and implementate it](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)
* [Efficient and well explained quadtree collision](https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det)
* [What is a Quadtree? (spanish version)](https://www.genbetadev.com/programacion-de-videojuegos/teoria-de-colisiones-2d-quadtree)
* [Spatial partition tutorial](https://www.youtube.com/watch?v=RN1GRX2ByLM&t=1950s)

# Author

All the project has been done by Bosco Barber Esbert. You can find me in:
* GitHub: [boscobarberesbert](https://github.com/boscobarberesbert)
* Contact: boscobarberesbert@gmail.com
