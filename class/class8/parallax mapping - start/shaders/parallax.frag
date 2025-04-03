#version 330

uniform sampler2D diffuse, normalMap, heightMap;
uniform float scale, bias;

in vec3 ld, eye;
in vec2 tc;

out vec4 colorOut;

void main() {


    vec3 e = normalize(eye);
    float h = texture(heightMap, tc).x;
    vec2 disp = ((1-h) * scale + bias) * e.xy / -e.z;
    vec2 tc_disp = tc + disp;

	vec3 n = normalize(texture(normalMap, tc_disp).xyz * 2.0 - 1.0);

	vec3 l = normalize(ld);
	float intensity = max(dot(n,l), 0.0);
	vec4 color = texture(diffuse, tc_disp);
	colorOut = (intensity * 2) * color ;
}

