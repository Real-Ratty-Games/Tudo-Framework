/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef SOUND_HPP_
#define SOUND_HPP_
#include "SystemTypes.hpp"
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#include <vector>

namespace Tudo
{
	typedef SoLoud::handle		SoundHandle;
	typedef SoLoud::AudioSource SoundSrc;
	typedef SoLoud::Soloud		SoundCore;
	typedef SoLoud::result		SoundResult;

	struct SoundWav
	{
		SoLoud::Wav Handle;
	};

	struct SoundWavStream
	{
		SoLoud::WavStream Handle;
	};

	/// <summary>
	/// Manages audio
	/// </summary>
	class SoundManager
	{
	public:
		void Initialize();
		~SoundManager();

		SoundCore* Core();

	private:
		SoundCore* mCore;
	};
}
#endif