#ifndef DAE_OBSERVER
#define DAE_OBSERVER

#include <tuple>
#include <any>
#include "Event.h"

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const Event& event) = 0;
	};
}

#endif