#version 130

in vec2 fragPosition;
in vec4 fragColor;

out vec4 color;

uniform float time;

void main() {
    // color = fragColor + vec4(
    //     1.0 * (cos(time) + 1.0) * 0.5, 
    //     1.0 * (cos(time + 2.0) + 1.0) * 0.5, 
    //     1.0 * (sin(time + 1.0) + 1.0) * 0.5,
    //     0.0);

    color = vec4(
        fragColor.x * (cos(fragPosition.x + time) + 1) * 0.5,
        fragColor.y * (cos(fragPosition.y + time) + 1) * 0.5,
        fragColor.z * (cos(fragPosition.x + time) + 1) * 0.5,
        fragColor.w);
}
