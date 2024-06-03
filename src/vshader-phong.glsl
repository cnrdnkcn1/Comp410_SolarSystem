#version 410

in   vec4 vPosition;
in   vec3 vNormal;
in   vec3 velo;
in   vec3 displacement;
in   vec4 vColor;

in int lightMode;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fV;
out  vec3 fL;
out  vec4 color;

uniform mat4 denemee;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    // Transform vertex position into camera (eye) coordinates
    vec3 pos = (ModelView * vPosition).xyz;
    
    fN = (ModelView * vec4(vNormal, 0.0)).xyz; // normal direction in camera coordinates

    fV = -pos; //viewer direction in camera coordinates


    fL = (denemee * LightPosition).xyz ; // light direction

    if( LightPosition.w != 0.0 ) {
        fL = LightPosition.xyz - pos;  //point light source
    }
    color = vColor;
    gl_Position = Projection * ModelView * vPosition;
}
