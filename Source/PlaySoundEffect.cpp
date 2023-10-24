#include "././Header/PlaySoundEffect.h"

std::vector<float>Original::volumes;
std::vector<int>Original::handles;
std::vector<const char*>Original::handleNames;

void Original::LoadAudio(const char _path[], const char _handleName[], float _volume)
{
	Original::handles.push_back(Novice::LoadAudio(_path));
	Original::handleNames.push_back(_handleName);
	Original::volumes.push_back(_volume);
}

int Original::SoundPlay(const char _handleName[])
{
	for (int i = 0; i < Original::handleNames.size(); i++)
	{
		// ハンドル名が一致していたら
		if (strcmp(Original::handleNames[i], _handleName) == 0)
		{
			Novice::PlayAudio(Original::handles[i], 0, Original::volumes[i]);
		}
	}

	return 0;
}
