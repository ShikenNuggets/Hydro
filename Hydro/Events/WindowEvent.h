#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

#include "Event.h"

namespace Hydro{
	class WindowEvent : public Event{
	protected:
		WindowEvent() : Event(EventCategory::Window){}
	};
}

#endif //!WINDOW_EVENT_H