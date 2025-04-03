#version 330

layout(triangles) in;
layout (triangle_strip, max_vertices=6) out;

uniform mat4 m_proj;

uniform vec4 camPos, camView, camUp;
uniform vec4 l_dir;

uniform float eye_separation = 0.03;

in DataV2G {
	vec4 normal;
} DataIn[];

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

// this example assumes the model matrix to be the identity matrix,
// hence, we can use the view matrix as the normal matrix

void main() {

	mat4 mT, mR, mView;

	vec3 camRight = normalize(cross(camView, camUp));
	mR = transpose(mat4(camRight, 0, camUp, -camView, vec3(0), 1));

	pos = vec3(camPos) - camRight * eye_separation;

	mT = mat4(1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  vec3(-pos), 1);
	
	mView = mR * mT;

	gl_Layer = 0;
	for (int i = 0; i < 3; i++) {
		gl_Position = m_proj * mView * gl_in[i].gl_Position;
		DataOut.eye = vec3(mView * gl_in[i].gl_Position);
		DataOut.normal = DataIn[i].normal;
		DataOut.lightDir = l_dir.xyz;
		EmitVertex();
	}


}