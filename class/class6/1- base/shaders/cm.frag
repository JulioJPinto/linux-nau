#version 460

uniform samplerCube tex_cm;
uniform float alpha;
uniform float eta;
uniform bool dotflag;

in vec3 n, incident;

out vec4 color;

void main() {

    vec3 nn = normalize(n);
    vec3 i = normalize(incident);

    vec3 refl = reflect(i, nn);
    vec4 color_refl = texture(tex_cm, refl);

    vec3 refr = refract(i, nn, eta);
    vec4 color_refr = texture(tex_cm, refr);

    float opacity = alpha;
    if (dotflag) {
        opacity = 1.0 - dot(-i, nn);
    } 


    color = mix(color_refr, color_refl, opacity);


}
