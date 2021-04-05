#version 450

in vec2 position;
in vec2 vTexture;

out vec2 TexCoords;

void main()
{
    TexCoords = vTexture;
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 
}  