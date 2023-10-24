#pragma once
#include <Novice.h>
#include <vector>
#include <string.h>



class Original {

public:
	
	static void LoadAudio(const char _path[], const char _handleName[], float _volume);

	static int SoundPlay(const char _handleName[]);

private:
	static std::vector<float> volumes;
	static std::vector<int>	handles;
	static std::vector<const char*>	handleNames;
};