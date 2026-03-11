#pragma once

#include <string>
#include <unordered_map>


// Forward declarations
namespace FMOD
{
	namespace Studio
	{
		class System;
		class Bank;
		class Bus;
		class EventDescription;
		class EventInstance;
	}
}

namespace Dawn
{
	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();

		void LoadBank(const std::string& name);
		void UnloadBank(const std::string& name);
		void UnloadAllBanks();

		void Update();

		// TODO: SetListener();

		void SetBusVolume(const std::string& name, float value);
		void SetBusPaused(const std::string& name, bool value);
		float GetBusVolume(const std::string& name);
		bool GetBusPaused(const std::string& name);

	private:
		bool Init();
		void Shutdown();
	private:
		// used to generate ids for EventInstances
		static unsigned int sNextId;

		std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
		std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;

		FMOD::Studio::System* mSystem = nullptr;
	};
}