#version 460

uniform mat4 m_pvm, m_m;
uniform vec3 cam_pos;

in vec4 position;
in vec4 normal;

out vec3 n, incident; //world space

void main() {
    
    mat4 m = inverse(transpose(m_m));
    n = normalize(vec3(m * normal));

    incident = vec3(m * position) - cam_pos;

    gl_Position = m_pvm * position;

}