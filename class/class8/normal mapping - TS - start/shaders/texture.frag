#version 330

in vec2 tc;
in vec3 norm, ld;

out vec4 colorOut;

void main() {

	// normalize vectors
	
	vec3 n = normalize(norm);
	float i = max(dot(n, ld), 0.0);
	vec4 diffuse = vec4(1.0, 1.0, 0.9, 1);
	colorOut = (i + 0.15) * diffuse;

}

