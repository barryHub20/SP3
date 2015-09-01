/* ================================== //
//      Bookkeeper - Muzhaffar        //
// ================================== */

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "irrKlang.h"
using namespace irrklang;

#pragma comment(lib, "irrklang/lib/Win32-visualStudio/irrKlang.lib") // link with irrKlang.dll

class SoundManager
{
public:
	//Declaration of sound variables
	
	ISoundEngine* sfxengine;
	ISound *sfx_plyr_step;
	ISound *sfx_fire;
	ISound *sfx_ambience;

	SoundManager();
	~SoundManager();

	// Functions
	void sfx_init(void);

	// ================================ Sound effects ==================================

	void play_ambience(void);
	void play_plyr_steps(void);	// step(max).ogg
	void play_pickup(void);		// pickup.wav
	void play_drop(void);		// drop.ogg
	void play_unlock(void);		// access.ogg
	void play_yummy(void);		// yummy.ogg
	void play_stone_move(void);	// stone_move.ogg

	void play_fire(void);	void stop_fire(void);		// fire.wav
	void play_spike(void);		// spike.wav


	// =================================================================================
};
#endif