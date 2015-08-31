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
/*	if (!sfxengine->isCurrentlyPlaying("musfiles/Verdant_Forest.ogg"))
	{
		sfx_ambience = sfxengine->play2D("musfiles/Verdant_Forest.ogg");
	} */
}

void SoundManager::play_plyr_steps(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/step(max).ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/step(max).ogg");
	}
}

void SoundManager::play_pickup(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/pickup.wav"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/pickup.wav");
	}
}

void SoundManager::play_drop(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/drop.ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/drop.ogg");
	}
}

void SoundManager::play_unlock(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/access.ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/access.ogg");
	}
}