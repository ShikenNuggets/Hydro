#include "GLRenderer.h"

#include "Debug.h"
#include "FileSystem.h"
#include "GLRenderInfo.h"

using namespace Hydro;

GLRenderer::GLRenderer(Window* window_) : Renderer(window_), mainFBO(nullptr){
	Debug::Assert(FileSystem::FileExists(sharedShaderName));

	sharedShaderCode = FileSystem::ReadFileToString(sharedShaderName);
	Debug::Assert(!sharedShaderCode.empty());

	//Print out the graphics card being used
	std::string gpuName = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	Debug::Log("Active GPU: " + gpuName);

#ifdef _DEBUG
	if(glDebugMessageCallback){
		//Setup our debug callback if debug messages are available
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)GLDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		Debug::Log("Registered OpenGL Debug Callback function", __FILE__, __LINE__);
	}
	else{
		Debug::LogWarning("Could not setup OpenGL Debug Callback");
	}
#endif //_DEBUG

	//Set OpenGL stuff
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//multisampleFBO = new MultisampleFBO(screenSize);
	//if(multisampleFBO->Initialize() == false){
	//	Debug::LogError("Multisample FBO could not be initialized!", __FILE__, __LINE__);
	//	return false;
	//}

	//mainFBO = new GLMainFBO(window->Size());

	//if(PostProcessing::Initialize("ScreenShader") == false){
	//	Debug::LogError("PostProcessing could not be initialized!", __FILE__, __LINE__);
	//	return false;
	//}

	//shadowHandler = new Shadows("DepthShader", 8192);
	//if(shadowHandler->Initialize() == false) {
	//	Debug::LogError("Shadows could not be initialized!", __FILE__, __LINE__);
	//	return false;
	//}

	SetClearColor(Color::Black);

	//Set the clear color to dark gray if we're in the Debug configuration
#ifdef _DEBUG
	SetClearColor(Color::DarkGray);
#endif //_DEBUG

	//Clear and update the screen to that's it's filled in with something
	ClearScreen();
	window->GL_UpdateWindow();
}

GLRenderer::~GLRenderer(){
	//I'm not sure if or how many of these disables are necessary
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_CLAMP);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_MULTISAMPLE);

	/*if(shadowHandler != nullptr){
		shadowHandler->Destroy();
		delete shadowHandler;
		shadowHandler = nullptr;
	}

	PostProcessing::Destroy();*/

	//if(mainFBO != nullptr){
	//	delete mainFBO;
	//	mainFBO = nullptr;
	//}

	/*if(multisampleFBO != nullptr){
		multisampleFBO->Destroy();
		delete multisampleFBO;
		multisampleFBO = nullptr;
	}*/
}

void GLRenderer::Render(){
	ClearScreen();
	for(Camera* cam : cameras){
		cam->CalculateViewMatrix();
		SetViewport(cam->GetViewportRect());

		for(MeshRenderer* mesh : meshes){
			dynamic_cast<GLRenderInfo*>(mesh->renderInfo)->Render();
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);

	//mainFBO->Bind();
	//for(Camera* cam : cameras){
	//	SetViewport(cam->GetViewportRect());

	//	//Clear the depth buffer before drawing text
	//	//This guarantees that text will always be visible no matter how close anything else is to the camera
	//	//glClear(GL_DEPTH_BUFFER_BIT);
	//	//Loop through all TextRender components in the scene and render them if they're enabled
	//	for(TextRender* tr : trList){
	//		tr->Render(cam);
	//	}
	//}

	//Reset the viewport to fullscreen before rendering UI
	SetViewport(ViewportRect::fullScreen);

	//mainFBO->Unbind();

	//PostProcessing::DoPostProcessing(info.depthMap); 
	//PostProcessing::DoPostProcessing(postProcessFBO->GetColorTexture());

	//print out the id of the frame buffer being used
		/*GLint result;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &result);
		std::cout << "FrameBuffer being used: " << result << std::endl;*/

		//Update the window to show the new rendered frame
	window->GL_UpdateWindow();
}

void GLRenderer::CreateRenderInfo(MeshRenderer* mesh_){
	mesh_->SetRenderInfo(new GLRenderInfo(mesh_->GetModel(), mesh_->GetTexture()));
}

void GLRenderer::OnResize(int width_, int height_){
	/*if(multisampleFBO != nullptr){
		multisampleFBO->Destroy();
		delete multisampleFBO;
		multisampleFBO = nullptr;
	}

	multisampleFBO = new MultisampleFBO(window->GetSize());
	if(multisampleFBO->Initialize() == false){
		Debug::LogError("Multi sampling FBO could not be initialized!", __FILE__, __LINE__);
	}*/

	//if(mainFBO != nullptr){
	//	delete mainFBO;
	//	mainFBO = nullptr;
	//}
	//mainFBO = new GLMainFBO(window->Size());

	for(Camera* cam : cameras){
		cam->Reset();
	}

	//PostProcessing::OnResize();

	ClearScreen();
	window->GL_UpdateWindow();
}

void GLRenderer::ClearScreen(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::SetClearColor(const Color& color_){
	glClearColor(color_.r, color_.g, color_.b, color_.a);
}

void GLRenderer::SetViewport(const ViewportRect& v_){
	glViewport(	static_cast<GLint>(window->Width() * v_.x),
				static_cast<GLint>(window->Height() * v_.y),
				static_cast<GLsizei>(window->Width() * v_.width),
				static_cast<GLsizei>(window->Height() * v_.height)
	);
}

void __stdcall GLRenderer::GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
	//Suppress useless warnings by ID here
	//Fallthroughs here are deliberate
	switch(id){
	case 131169: //The driver allocated multisample storage for renderbuffer - Treated as low severity warning, but should just be info
	case 131185: //Buffer object will use VIDEO memory - Irrelevant and obvious (we do this on purpose nowadays), probably a legacy thing
	case 131218: //Vertex shader is being recompiled based on GL state - Poorly defined NVIDIA thing, useless
	case 131204: //Texture object does not have a defined base level - Nobody seems to have any idea what this means or how to fix it
		return;
	default:
		break;
	}

	std::string finalMessage;
	finalMessage.append(std::to_string(id) + " OpenGL ");

	switch(source){
		case GL_DEBUG_SOURCE_API:
			finalMessage.append("API ");
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			finalMessage.append("Window System ");
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			finalMessage.append("Third Party ");
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			finalMessage.append("Application ");
			break;
		case GL_DEBUG_SOURCE_OTHER:
			break;
		default:
			break;
	}

	switch(type){
		case GL_DEBUG_TYPE_ERROR:
			finalMessage.append("Error: ");
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			finalMessage.append("Deprecated Behaviour: ");
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			finalMessage.append("Undefined Behaviour: ");
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			finalMessage.append("Portability: ");
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			finalMessage.append("Performance: ");
			break;
		case GL_DEBUG_TYPE_MARKER:
			finalMessage.append("Marker: ");
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			finalMessage.append("Push Group: ");
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			finalMessage.append("Push Group: ");
			break;
		case GL_DEBUG_TYPE_OTHER:
			//Intentional Fallthrough
		default:
			finalMessage.append("Other: ");
			break;
	}

	finalMessage.append(message);
	finalMessage.append("!");

	switch(severity){
		case GL_DEBUG_SEVERITY_HIGH:
			Debug::LogError(finalMessage);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Debug::LogWarning(finalMessage);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Debug::LogWarning(finalMessage);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Debug::Log(finalMessage);
			break;
	}
}