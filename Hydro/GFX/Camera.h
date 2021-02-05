#ifndef CAMERA_H
#define CAMERA_H

#include "ViewportRect.h"
#include "Math/Matrix.h"
#include "Objects/Component.h"

namespace Hydro{
	class Camera : public Component{
	public:
		enum class RenderMode{
			Orthographic,
			Perspective
		};

		Camera(const ViewportRect& vr_ = ViewportRect::fullScreen, RenderMode mode_ = RenderMode::Perspective);
		~Camera();

		void Reset();

		void CalculateViewMatrix();

		//Getters
		Matrix4& GetProjectionMatrix() const;
		Matrix4 GetViewMatrix() const;
		Matrix4 GetPerspective() const;
		Matrix4 GetOrthographic() const;
		ViewportRect GetViewportRect() const;
		RenderMode GetRenderMode() const;
		inline float GetNearPlane() const{ return nearPlane; }
		inline float GetFarPlane() const{ return farPlane; }

		//Setters
		void SetRenderMode(const RenderMode switchMode_);
		void SetViewportRect(const ViewportRect& vr_);
		void SetFOV(float fov_);
		void SetNearPlane(float near_);
		void SetFarPlane(float far_);

	private:
		Matrix4 perspective, orthographic;
		Matrix4 viewMatrix;
		Matrix4& projectionMatrix;

		float fieldOfView, nearPlane, farPlane;

		RenderMode renderMode;
		ViewportRect viewportRect;
	};
}

#endif //!CAMERA_H