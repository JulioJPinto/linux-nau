#version 460

in vec4 position; // local
in vec3 normal; // local

uniform mat4 m_pvm, m_view_model;
uniform mat3 m_normal;

out vec3 n; // camera

out vec3 p_c; // camera


void main() {

    p_c = vec3(m_view_model * position);

    n = normalize(m_normal * normal); // local -> camera

    gl_Position = m_pvm * position;
}