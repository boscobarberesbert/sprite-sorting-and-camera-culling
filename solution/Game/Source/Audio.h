#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include <vector>


#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;


class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	//Save states
	bool Save(pugi::xml_node&) const;

	//Load states
	bool Load(pugi::xml_node&);

	//Raise and Decrease the volume
	int SetVolume(float);
	int GetVolume() const;
	int RaiseVolume();
	int DecreaseVolume();

	bool FadeOutFx(unsigned int fx, int fade = 0);
	bool StopFx(unsigned int fx);

	int SetFx(float);

private:

	_Mix_Music*					music;
	std::vector<Mix_Chunk*>		fx;
	int							volumeMusic;
	
};

#endif // __j1AUDIO_H__