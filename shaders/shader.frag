#version 330 core

layout (location=0) out vec4 vFragColor;
smooth in vec2 vUV;
uniform sampler2D textureMap;

void main()
{                                               // color: red
    vFragColor = texture(textureMap, vUV);// * vec4(1,0,0,1);
    //vFragColor = vec4(1, 1, 1, texture(textureMap, vUV)) * vec4(1,0,0,1);
}

