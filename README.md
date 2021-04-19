# Hydro
Fully-featured 3D game engine... eventually.

# So.. what is this, exactly?
The long-term goal of this project is to create a fully-featured game engine that could (theoretically) be used to create a modern AAA game, more specifically, a 3rd-person action game a large streamable environment and online multiplayer. Of course, the engine itself should be as flexibile as possible, that's just one kind of game we want to be able to make with it.

# Core Focus
Obviously, "fully-featured game engine" is a pretty tall order, and has a lot of subcomponents that all take a fair amount of work to create on their own. This is also very much an educational project. So, these are some of the key things I want to focus on:
- Use Modern C++ language features (where appropriate, of course). We're currently targetting C++17.
- Utilize arbitrary numbers of cores/threads in ways that actually makes games faster. Higher and higher core/thread counts are becoming the norm, we should strive to take advantgate of this as much as possible.
- Modern Graphics APIs. Our previous game engine exclusively used OpenGL, and a lot of the engine was hardcoded to work specifically with OpenGL. We should also support Vulkan and DX12, and implement these in a generic way so it's easier to add new APIs in the future.
- Networking and multiplayer features built right into the engine. In addition to all the back-end work for this, we also want it to be trivial to convert a singleplayer game into a multiplayer game.
- Tools. This was sorely lacking in our last engine. Objects were defined in C++, scenes were defined in C++, very little was properly data-driven, which obviously doesn't scale very well.

# Build
You need to install the [Lunar G Vulkan SDK](https://www.lunarg.com/vulkan-sdk/), specifically version 1.2.141.2, in your C: drive root (I'm sure there's a better solution for this, I am open to suggestions).

You'll also need Visual Studio 2019. In theory, the premake file can be used to generate project files for other compilers/platforms, but this has not been tested.
