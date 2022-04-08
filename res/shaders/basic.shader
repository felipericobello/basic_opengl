#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
uniform mat4 uMVP;

void main()
{
   gl_Position = uMVP * position;
   v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
	vec4 texColor = texture(uTexture, v_TexCoord);
	color = texColor;
}