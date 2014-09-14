#version 330 core

layout (location=0) out vec4 vFragColor;
smooth in vec2 vUV;
uniform sampler2D textureMap;
uniform vec4 fontColor = vec4(1, 0, 0, 1);

void main()
{
    vFragColor = texture(textureMap, vUV).rrrr * fontColor;
}

