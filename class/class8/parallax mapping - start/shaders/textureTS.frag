#version 330

uniform sampler2D diffuse, normalMap;

in vec3 ld;
in vec2 tc;

out vec4 colorOut;

void main() {


	vec3 n = normalize(texture(normalMap, tc).xyz * 2.0 - 1.0);

	vec3 l = normalize(ld);
	float intensity = max(dot(n,l), 0.0);
	vec4 color = texture(diffuse, tc);
	colorOut = (intensity * 2) * color ;
}

