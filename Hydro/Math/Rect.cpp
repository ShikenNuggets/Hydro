#include "Rect.h"

#include <rttr/registration.h>

using namespace PizzaBox;

//Suppress meaningless and unavoidable warning
#pragma warning( push )
#pragma warning( disable : 26444 )
RTTR_REGISTRATION{
	rttr::registration::class_<Rect>("Rect")
		.constructor()(rttr::policy::ctor::as_raw_ptr)
		.constructor<float, float, float, float>()(rttr::policy::ctor::as_raw_ptr)
		.property("x", &Rect::x)
		.property("y", &Rect::y)
		.property("w", &Rect::w)
		.property("h", &Rect::h)
		.method("Intersects", &Rect::Intersects);
}
#pragma warning( pop )