#ifndef __RENDER_H__
#define __RENDER_H__

#include "SDL/include/SDL.h"
#include "Point.h"
#include "Module.h"

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	SDL_Rect CameraInitPos();

	// Blit
	bool IsOnCamera(const int &x, const int &y, const int &w, const int &h) const;

	iPoint ScreenToWorld(int x, int y) const;
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, bool scale = true, SDL_RendererFlip = SDL_FLIP_NONE, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(const SDL_Color &color);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Color		background;
};

#endif // __RENDER_H__