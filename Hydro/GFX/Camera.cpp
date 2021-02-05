#include "Camera.h"

#include "App.h"
#include "Vulkan/VKRenderer.h"

using namespace Hydro;

Camera::Camera(const ViewportRect& vr_, RenderMode mode_) : Component(), perspective(Matrix4()), orthographic(Matrix4()), viewMatrix(Matrix4()),
	fieldOfView(45.0f), nearPlane(0.1f), farPlane(1000.0f), projectionMatrix(perspective), renderMode(mode_), viewportRect(vr_){
	App::GetRenderer()->AddCamera(this);
}

Camera::~Camera(){
	App::GetRenderer()->RemoveCamera(this);
}

void Camera::Reset(){
	float aspect = static_cast<float>(App::GetWindowWidth() * viewportRect.width) / static_cast<float>(App::GetWindowHeight() * viewportRect.height);
	perspective = Matrix4::Perspective(fieldOfView, aspect, nearPlane, farPlane);
	orthographic = Matrix4::Orthographic(-10.0f, 10.0f * aspect, -10.0f, 10.0f / aspect, -100.0f, 100.0f);
	SetRenderMode(renderMode);
}

void Camera::CalculateViewMatrix(){
	viewMatrix = Matrix4::Identity();
	//viewMatrix *= gameObject->GlobalRotationQuat().ToMatrix4().Inverse();
	//viewMatrix *= Matrix4::Translate(gameObject->GetTransform()->GlobalPosition()).Inverse();
}

Matrix4& Camera::GetProjectionMatrix() const{
	return projectionMatrix;
}

Matrix4 Camera::GetViewMatrix() const{
	return viewMatrix;
}

Matrix4 Camera::GetPerspective() const{
	return perspective;
}

Matrix4 Camera::GetOrthographic() const{
	return orthographic;
}

ViewportRect Camera::GetViewportRect() const{
	return viewportRect;
}

Camera::RenderMode Camera::GetRenderMode() const{
	return renderMode;
}

void Camera::SetRenderMode(const RenderMode switchMode){
	renderMode = switchMode;
	if(renderMode == RenderMode::Orthographic){
		projectionMatrix = orthographic;
	}else if(renderMode == RenderMode::Perspective){
		projectionMatrix = perspective;
	}
}

void Camera::SetViewportRect(const ViewportRect& vr_){
	viewportRect = vr_;
	Reset();
}

void Camera::SetFOV(float fov_){
	fieldOfView = fov_;
	Reset();
}

void Camera::SetNearPlane(float near_){
	nearPlane = near_;
	Reset();
}

void Camera::SetFarPlane(float far_){
	farPlane = far_;
	Reset();
}