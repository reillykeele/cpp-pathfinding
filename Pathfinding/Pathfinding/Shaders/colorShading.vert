#version 130

in vec2 vertPosition;
in vec4 vertColor;
in vec2 vertUV;

out vec2 fragPosition;
out vec4 fragColor;
out vec2 fragUV;

void main() {
    gl_Position.xy = vertPosition;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragPosition = vertPosition;
    fragColor = vertColor;        
    fragUV = vec2(vertUV.x, 1.0 - vertUV.y);
}
