#ifndef HYDRO_EVENT_H
#define HYDRO_EVENT_H

#include <string>
#include <functional>

namespace Hydro{
	enum class EventCategory{
		None = 0,
		Application,
		Window,
		Input
	};

	enum class EventType{
		None = 0,
		ApplicationQuit,
		WindowShown, WindowHidden, WindowMinimized, WindowMaximized, WindowRestored, WindowResized,
		InputKeyDown, InputKeyUp
	};

	class Event{
	protected:
		Event(EventCategory cat_ = EventCategory::None, EventType type_ = EventType::None) : category(cat_), type(type_), handled(false){}

		EventCategory category;
		EventType type;
		bool handled;
	};
}

#endif //!HYDRO_EVENT_H