#vertex shader
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;

out vec2 texCoord;

uniform mat4 projection;

void main() {
	texCoord = vTexCoord;
	
	vec4 pos = vec4(vPos, -1.0);
	pos = pos * projection;
	gl_Position = vec4(pos);
}

#fragment shader
#version 330 core
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D tex;
void main() {
	outColor = texture(tex, texCoord);
}