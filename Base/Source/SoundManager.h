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
	ISound *sfx_ambience;

	SoundManager();
	~SoundManager();

	// Functions
	void sfx_init(void);

	// ================================ Sound effects ==================================

	void play_ambience(void);
	void play_plyr_steps(void);
	void play_pickup(void);
	void play_drop(void);
	void play_unlock(void);

	// =================================================================================
};
#endif