#ifndef DAE_SERVICELOCATOR
#define DAE_SERVICELOCATOR

#include <memory>
#include "SoundSystemBase.h"
#include "NullSoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystemBase& GetSoundSystem() { return *m_SoundSystemInstance.get(); }

		static void RegisterSoundSystem(SoundSystemBase* soundSystem)
		{
			if (soundSystem != nullptr)
			{
				m_SoundSystemInstance.reset(soundSystem);
			}
			else
			{
				m_SoundSystemInstance = std::make_unique<NullSoundSystem>();
			}
		}
	private:
		static std::unique_ptr<SoundSystemBase> m_SoundSystemInstance;
	};

}


#endif