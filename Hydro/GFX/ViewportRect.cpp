#include "ViewportRect.h"

using namespace Hydro;

const ViewportRect ViewportRect::fullScreen		= ViewportRect(0.0f, 0.0f, 1.0f, 1.0f);
const ViewportRect ViewportRect::top			= ViewportRect(0.0f, 0.5f, 1.0f, 0.5f);
const ViewportRect ViewportRect::bottom			= ViewportRect(0.0f, 0.0f, 1.0f, 0.5f);
const ViewportRect ViewportRect::left			= ViewportRect(0.0f, 0.0f, 0.5f, 1.0f);
const ViewportRect ViewportRect::right			= ViewportRect(0.5f, 0.0f, 0.5f, 1.0f);
const ViewportRect ViewportRect::topLeft		= ViewportRect(0.0f, 0.5f, 0.5f, 0.5f);
const ViewportRect ViewportRect::topRight		= ViewportRect(0.5f, 0.5f, 0.5f, 0.5f);
const ViewportRect ViewportRect::bottomLeft		= ViewportRect(0.0f, 0.0f, 0.5f, 0.5f);
const ViewportRect ViewportRect::bottomRight	= ViewportRect(0.5f, 0.0f, 0.5f, 0.5f);

ViewportRect::ViewportRect(const float x_, const float y_, const float w_, const float h_) : x(x_), y(y_), width(w_), height(h_){
	#ifdef _DEBUG
	CheckValidity();
	#endif //_DEBUG
}

ViewportRect::~ViewportRect(){
}

void ViewportRect::CheckValidity(){
	if(x > 1.0f || x < 0.0f){
		//Debug::LogWarning("ViewportRect X was not between 0 and 1!", __FILE__, __LINE__);
		x = 0.0f;
	}

	if(y > 1.0f || y < 0.0f){
		//Debug::LogWarning("ViewportRect Y was not between 0 and 1!", __FILE__, __LINE__);
		y = 0.0f;
	}

	if(width > 1.0f || width < 0.0f){
		//Debug::LogWarning("ViewportRect width was not between 0 and 1!", __FILE__, __LINE__);
		width = 1.0f;
	}

	if(height > 1.0f || height < 0.0f){
		//Debug::LogWarning("ViewportRect height was not between 0 and 1!", __FILE__, __LINE__);
		height = 1.0f;
	}
}