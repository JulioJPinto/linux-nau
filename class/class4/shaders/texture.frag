#version 460

// uniforms
uniform sampler2D tex;
uniform vec4 diffuse;
uniform float width;
uniform int div;

uniform vec4 aColor;
uniform vec4 otherColor;

uniform float gap;
uniform float factor;

// interpolated inputs
in vec2 texCoord;

// output
out vec4 color;

void main() {

    vec2 tc = texCoord * div;
    
    float value = fract(tc.s);

    // if (value < width - gap) {
    //     color = aColor;
    // } else if (value < width) {
    //     float f  = (value - width + gap) / gap;
    //     color = mix(aColor, otherColor, f);
    //     // color = aColor * (1-f) + otherColor * f;
    // } else if (value < 1.0 - gap) {
    //     color = otherColor;
    // } else {
    //     float f = (value - 1.0 + gap) / gap;
    //     color = mix(otherColor, aColor, f);
    //     // color = otherColor * (1-f) + aColor * f;
    // }

    vec2 deriv = vec2(dFdx(tc.s), dFdy(tc.s));
    float len = length(deriv);

    float actualGap = len * factor;

    float a = smoothstep(width - actualGap, width, value);
    float b = smoothstep(1.0 - actualGap, 1.0, value);

    float f = a - b;
    color = mix(aColor, otherColor, f);

    //to use texture for smoothstep
    // float r = texture(tex, vec2(tc.s,0)).r;
    // color = vec4(r,r,r,1);

    // color = mix(aColor, otherColor, r);

}