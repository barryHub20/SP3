#include "SoundManager.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{

}



void SoundManager::sfx_init(void)
{
	sfxengine = createIrrKlangDevice();
}




void SoundManager::play_ambience(void)
{
	if (!sfxengine->isCurrentlyPlaying("musfiles/Verdant_Forest.ogg"))
	{
		sfx_ambience = sfxengine->play2D("musfiles/Verdant_Forest.ogg");
	}
}

void SoundManager::play_plyr_steps(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/step(max).ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/step(max).ogg");
	}
}