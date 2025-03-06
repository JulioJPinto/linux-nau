#version 460

in vec3 n; // camera
in vec3 p_c; // camera

uniform vec4 l_dir; // global
uniform vec4 l_pos; // global
uniform vec4 l_sdir; // spotlight
uniform vec4 l_scutoff; // spotlight

uniform mat4 m_view;

uniform vec4 diffuse;

uniform float shininess;


out vec4 color;

void main () {
    vec3 nn = normalize(n);
    vec4 l4;

    if(l_pos == vec4(0, 0, 0, 1)) {
        //directional light
        l4 = m_view * -l_dir; // cam
    }
    else {
        // point and spotlight light
        l4 = m_view * l_pos; // light position cam
        l4 = l4 - vec4(p_c, 1); // light direction cam
    }

    vec3 ll = normalize(vec3(l4));

    vec4 ll_sdir = normalize(vec3(m_view * l_sdir)); // cam
    float spot_cos = dot(ll_sdir, -ll);

    float i;

    if (spot_cos > l_scutoff) {
        i = max(0, dot(nn, ll));
    } else {
        i = 0;
    }

    i = max(0, dot(nn, ll));

    vec4 spec = vec4(0);
    
    if (i > 0) {
        vec3 e = normalize(-p_c);
        vec3 h = normalize(ll + e);
        float cos_beta = max(0.0, dot (h, nn));
        float i_spec = pow(cos_beta, shininess);
        spec = vec4(1) * i_spec;
    }

    color = max(i, 0.25) * diffuse + spec * i;
}