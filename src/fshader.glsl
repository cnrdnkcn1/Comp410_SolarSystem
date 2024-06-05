#version 410

in vec4 color;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture;
uniform int TextureFlag;

out vec4 FragColor;

void main() {
    if (TextureFlag == 1) {
        vec4 texColor = texture2D(texture, TexCoord);
        FragColor = color * texColor;
        if(TexCoord.x == 0){
            FragColor = color;
        }
    } else {
        vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - vec3(gl_FragCoord)); // Direction from fragment to light source
        float diff = max(dot(Normal, lightDir), 0.0);
        FragColor = color * diff;
    }
}
