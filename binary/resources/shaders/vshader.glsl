#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexture;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 uN;
uniform mat4 uM;
uniform mat4 uPVM;

out vec3 vnor;
out vec3 vpos;
out vec2 vtex;
out mat3 vTBN;

void main() {

    vec4 inPositionH = vec4(inPosition,1);

    gl_Position = uPVM * inPositionH;

    vec3 worldPos = (uM * inPositionH).xyz;
    vpos = worldPos;

    vec3 N = normalize((uN * vec4(inNormal,   0.0)).xyz);
    vec3 T = normalize((uN * vec4(inTangent,  0.0)).xyz);
    vec3 B = normalize((uN * vec4(inBitangent,0.0)).xyz);

    vnor = N;
    T = normalize(T - dot(T,N) * N);
    B = cross(N,T);

    vTBN = mat3(T, B, N);
    vtex = inTexture;

}
