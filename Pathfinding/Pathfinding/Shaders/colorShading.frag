#version 130

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform float time;
uniform sampler2D sampler;

void main() {
    
    vec4 textureColor = texture2D(sampler, fragUV);
    
    // color = textureColor * fragColor;

    color = fragColor + vec4(
        1.0 * (cos(time) + 1.0) * 0.5, 
        1.0 * (cos(time + 2.0) + 1.0) * 0.5, 
        1.0 * (sin(time + 1.0) + 1.0) * 0.5,
        0.0);

    color = color * textureColor;

    // color = vec4(
    //     fragColor.x * (cos(fragPosition.x + time) + 1) * 0.5,
    //     fragColor.y * (cos(fragPosition.y + time) + 1) * 0.5,
    //     fragColor.z * (cos(fragPosition.x + time) + 1) * 0.5,
    //     fragColor.w) * textureColor;
}
