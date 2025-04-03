#version 330

uniform sampler2D diffuse, normalMap, heightMap;
uniform float bumpScale;
uniform int linSteps, binSteps;

in vec3 ld, eye;
in vec2 tc;

out vec4 colorOut;

float getHeight(vec2 tc) {
    return texture(heightMap, tc).x;
}

void main() {


    vec3 e = normalize(eye);

    float step = 1.0 / linSteps;
    vec2 disp = step * bumpScale * e.xy / -e.z;


    vec2 tc_disp = tc;
    float height = 1.0;
    float h = getHeight(tc_disp);

    while (height > h) {
        height -= step;
        tc_disp += disp;
        h = getHeight(tc_disp);
    }

    vec2 after = tc_disp;
    vec2 before = tc_disp - disp;
    vec2 middle = after;
    int k = 0;

    while (k < binSteps && abs(h - height) > 0.00001) {
        middle = before + 0.5 * (after - before);
        step *= 0.5;
        height += step;
        h = getHeight(middle);
        if ( h > height)
            after = middle;
        else {
            before = middle;
            height += step;
        }
        k++;
    }

    tc_disp = middle;

	vec3 n = normalize(texture(normalMap, tc_disp).xyz * 2.0 - 1.0);

	vec3 l = normalize(ld);
	float intensity = max(dot(n,l), 0.0);
	vec4 color = texture(diffuse, tc_disp);
	colorOut = (intensity * 2) * color ;
}

