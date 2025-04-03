#version 330

uniform sampler2D diffuse;

in vec3 ld, norm;
in vec2 tc;

out vec4 colorOut;

void main() {


	vec3 n = normalize(norm);

	vec3 l = normalize(ld);
	float intensity = max(dot(n,l), 0.0);
	vec4 color = texture(diffuse, tc);
	colorOut = (intensity * 2) * color ;
}

