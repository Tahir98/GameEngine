#vertex shader
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColor;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float chosen;

void main() {
	color = vec4(vColor, 1.0);
	vec4 pos = vec4(vPos, 1.0f) * model * view * projection;

	if (chosen == 0.0) {
		gl_PointSize = 5.0;
	}
	else if (chosen == 1.0) {
		pos.z = -pos.w;
	}

	gl_Position = pos;
}


#fragment shader
#version 330 core

in vec4 color;
out vec4 o_color;

uniform float chosen;

void main() {
	if (chosen == 0.0)
		o_color = vec4(1, 0, 0, 1);
	else if (chosen == 1.0)
		o_color = vec4(0, 0, 0, 1);
	else if (chosen == 2.0) {
		o_color = color;
	}
}
