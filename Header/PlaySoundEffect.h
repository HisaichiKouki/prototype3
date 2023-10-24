#pragma once
#include <Novice.h>
#include <vector>
#include <string.h>



class Original {

public:
	
	static void LoadAudio(const char _path[], const char _handleName[]);

	static int SoundPlay(const char _handleName[]);

private:

	static std::vector<int>handles;
	static std::vector<const char*>handleName;
};