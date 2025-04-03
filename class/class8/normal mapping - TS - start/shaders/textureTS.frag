#version 330

uniform sampler2D normalMap;


in vec2 tc;
in vec3 norm, tang, ld;

out vec4 colorOut;

void main() {

	// normalize vectors and compute bitangent
	vec3 n = normalize(norm);
	vec3 t = normalize(tang);
	vec3 b = normalize(cross(n, t));

	mat3 tbn = mat3(t, b, n);


	// get normal from the map data
	vec3 texNormal = vec3(texture(normalMap, tc) * 2.0 - 1.0);
	vec3 l = normalize(transpose(tbn) * ld);
	
	// transform normal to camera space and normalize
	vec3 nt = texNormal;

	float i = max(dot(nt,l), 0.0);

	vec4 diffuse = vec4(1.0, 1.0, 0.9, 1);
	colorOut = (i + 0.15) * diffuse;

}

