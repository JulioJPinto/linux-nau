#version 430

layout(binding=1, offset=180) uniform atomic_uint at0;	//Light-Facing Pixels (PVPL)
layout(binding=1, offset=184) uniform atomic_uint at1;	//Non Light-Facing Pixels (PVCL) 
layout(binding=1, offset=188) uniform atomic_uint at2;	//Light Pixels 
layout(binding=1, offset=192) uniform atomic_uint at3;   //Shadow Pixels 
layout(binding=1, offset=196) uniform atomic_uint at4;  	//Correctos em Luz
layout(binding=1, offset=200) uniform atomic_uint at5;	//Incorrectos em Luz
layout(binding=1, offset=204) uniform atomic_uint at6; 	//Correctos em Sombra
layout(binding=1, offset=208) uniform atomic_uint at7;	//Incorrectos em Sombra
layout(binding=1, offset=212) uniform atomic_uint at8;	//Total



layout(std430, binding = 2) buffer hitsBuffer {
	vec4 hits[];
};

uniform sampler2D texUnit; 
uniform int RenderTargetX;
uniform int RenderTargetY;

in vec3 texCoordV;

out vec4 outColor;

void main() {
	
	ivec2 coord = ivec2(texCoordV.xy*vec2(RenderTargetX,RenderTargetY));
	int coordB = coord.x * RenderTargetY + coord.y;
	vec4 resPrime = hits[coordB];
	bool intersectou = (resPrime.y >= 0.0); //y = indice do triangulo intersectado
 
//Combine	
	vec4 colorSM = texture(texUnit, texCoordV.xy);
	bool green = (colorSM.r == 0.0 && colorSM.b == 0.0 && colorSM.g == 1.0);
	outColor = colorSM; 
 	atomicCounterIncrement(at8); //Total
	
	if (green) {
		atomicCounterIncrement(at1); //Non Light-Facing Pixels (PVCL) 
		outColor = vec4(0.0,1.0,0.0,1.0); 
	}
	else {
		atomicCounterIncrement(at0); //Non Light-Facing Pixels (PVCL) 
		if (colorSM.b > 0.0) {
			atomicCounterIncrement(at2); //Light Pixel
			if (intersectou) {
				atomicCounterIncrement(at5); //Incorrecto em Luz
				outColor = vec4(0.0,0.0,1.0,1.0);
			}
			else {
				atomicCounterIncrement(at4); //Correcto em Luz
				outColor = vec4(1.0);
			}
		}
		else {
			atomicCounterIncrement(at3); //Shadow Pixels no SM Encolhido
			if (intersectou) {
				atomicCounterIncrement(at6); //Correcto em Sombra
				outColor = vec4(0,0,0,1.0);
			}
			else {
				atomicCounterIncrement(at7); //Incorrecto em Sombra
				outColor = vec4(1.0,0,0,1.0);
			}
		}
	}
}