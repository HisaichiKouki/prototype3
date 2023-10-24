#include "././Header/PlaySoundEffect.h"

std::vector<int>Original::handles;
std::vector<const char*>Original::handleName;

void Original::LoadAudio(const char _path[], const char _handleName[])
{
	Original::handles.push_back(Novice::LoadAudio(_path));
	Original::handleName.push_back(_handleName);
}

int Original::SoundPlay(const char _handleName[])
{
	for (int i = 0; i < Original::handleName.size(); i++)
	{
		// ハンドル名が一致していたら
		if (strcmp(Original::handleName[i], _handleName) == 0)
		{
			Novice::PlayAudio(Original::handles[i], 0, 0.5f);
		}
	}

	return 0;
}
