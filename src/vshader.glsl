#version 410

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoord;

uniform mat4 denemee;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;


out vec4 color;
out vec3 Normal;
out vec2 TexCoord;

void main() {
   
   vec3 pos = (ModelView * vPosition).xyz;
    
    vec3 L = (denemee * LightPosition).xyz; // light direction if directional light source
    if( LightPosition.w != 0.0 ) L = LightPosition.xyz - pos;  // if point light source

    
    L = normalize(L);
    
    vec3 V = normalize( -pos ); // viewer direction
    vec3 H = normalize( L + V ); // halfway vector

    // Transform vertex normal into camera coordinates
    vec3 N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
    if( dot(L, N) < -0.95 && pos.x > -0.15 && pos.z < -1.86 && pos.x < 0.15 && pos.z > -2.15) {
        N = -1*N;
    } 

    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 ); //set diffuse to 0 if light is behind the surface point
    vec4  diffuse = Kd*DiffuseProduct;

    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    
    //ignore also specular component if light is behind the surface point
    if( dot(L, N) < 0.0 ) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    } 

    gl_Position = Projection * ModelView * vPosition;

    color = ambient + diffuse + specular;
    color.a = 1.0;

    Normal = normalize(vNormal);
    TexCoord = vTexCoord;
}