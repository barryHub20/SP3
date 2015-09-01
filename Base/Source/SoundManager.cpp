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

void SoundManager::play_fire(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/fire.wav"))
	{
		sfx_fire = sfxengine->play2D("soundfiles/fire.wav");
	}
}

void SoundManager::stop_fire(void)
{
	if (sfx_fire)
	{
		sfx_fire->stop();
		sfx_fire->drop();
		sfx_fire = 0;
	}
}

void SoundManager::play_yummy(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/yummy.ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/yummy.ogg");
	}
}

void SoundManager::play_stone_move(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/stone_move.ogg"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/stone_move.ogg");
	}
}

void SoundManager::play_spike(void)
{
	if (!sfxengine->isCurrentlyPlaying("soundfiles/spike.wav"))
	{
		sfx_plyr_step = sfxengine->play2D("soundfiles/spike.wav");
	}
}