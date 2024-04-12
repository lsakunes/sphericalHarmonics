#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo{
    mat4 projectionViewMatrix;
    vec3 directionToLight;
    float time;
} ubo;

// later check out possibility of
// mat4 projectionMatrix;
// mat4 viewMatrix;
// mat4 modelMatrix;
// vec3 color
layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

///////////////////////////////////
//////SPHERICAL HARMONICS/////////
/////////////////////////////////
//////aziumthal, polar//////////

// Y 0,1
vec2 sphericalHarmonicL1M0(vec2 coord){
    float normalization = 0.5f * sqrt(3/(3.1415926535897932384626433832795));
    float theta = cos(coord.y);
    vec2 phi = vec2(1,1);
    return normalization * theta * phi;
}   

// Y 1,2
vec2 sphericalHarmonicL2M1(vec2 coord){
    float normalization = -0.5f * sqrt(15/(2*3.1415926535897932384626433832795));
    float theta = sin(coord.y) * cos(coord.y);
    vec2 phi = vec2(cos(coord.x), sin(coord.x));
    return normalization * theta * phi;
}   
//Y 0,2
vec2 sphericalHarmonicL2M0(vec2 coord){
    float normalization = 0.25f * sqrt(5/(3.1415926535897932384626433832795));
    float theta = 3*cos(coord.y)*cos(coord.y)-1;
    vec2 phi = vec2(1,1);
    return normalization * theta * phi;
}  
//
//Y -1,3
vec2 sphericalHarmonicL3Mminus1(vec2 coord){
    float normalization = 0.125f * sqrt(105/(2*3.1415926535897932384626433832795));
    float theta = sin(coord.y)*(5*cos(coord.y)*cos(coord.y)-1);
    vec2 phi = -1 * vec2(cos(coord.x), sin(coord.x));
    return normalization * theta * phi;
} 
//Y -4, 8
vec2 sphericalHarmonicL8Mminus4(vec2 coord){
    float normalization = (3.f/128.f) * sqrt(1309/(2*(3.1415926535897932384626433832795)));
    float theta = pow(sin(coord.y),4) * (65*pow(cos(coord.y),4) - 26*cos(coord.y)*cos(coord.y) + 1);
    vec2 phi = -4 * vec2(cos(coord.x), sin(coord.x));
    return normalization * theta * phi;
}    
/////////////////////////
////////////////////////

vec2 sphericalHarmonic(vec2 coord){
    return sphericalHarmonicL1M0(coord);
}


vec4 interpretComplexNumber(vec2 number){
    float value = length(number); 
    //number = normalize(number);
    float angle = atan(number.y, number.x);
    if (angle < 0.0)
        angle += 2.0 * 3.1415926535897932384626433832795;
    float hue = angle /(2.0 * 3.14159); // Convert angle to a value between 0 and 1
    vec3 c = vec3(hue, 1, value*value);
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return vec4(c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y), value);
}

vec2 multiplyComplexNums(vec2 num1, vec2 num2){
    return vec2(num1.x*num2.x - num1.y*num2.y, num1.x*num2.y + num2.x*num1.y);
}

vec2 timeFunction(){
    float mult = -1.f;
    return vec2(cos(mult * ubo.time), sin(mult * ubo.time));
}

//aziumthal, polar
//phi, theta
vec2 cartesianToSpherical(vec3 cartesianCoords){
    float rho = length(cartesianCoords);
    float phi = atan(cartesianCoords.z, -cartesianCoords.x); 
    float theta = acos(cartesianCoords.y / rho);
    
    if (phi < 0.0) {
        phi += 2.0 * 3.14159;
    }
    
    return vec2(phi, theta);
}

vec4 sphereColor(){
    vec2 spherical = sphericalHarmonic(cartesianToSpherical(position));
    return interpretComplexNumber(multiplyComplexNums(spherical, timeFunction()));
}

//phi, theta, rho
//azimuthal, polar, magnitude
vec3 sphericalToCartesian(vec3 sphericalCoords){
    float y = cos(sphericalCoords.y) * sphericalCoords.z;
    float z = cos(sphericalCoords.x) * sqrt(sphericalCoords.z * sphericalCoords.z - y * y);
    float x = sin(sphericalCoords.x) * sqrt(sphericalCoords.z * sphericalCoords.z - y * y);
    return vec3(x, y, z);
}
/////////////////////////
////////////////////////


///////////////////////////////////
/////////////MAIN/////////////////
/////////////////////////////////
const float AMBIENT = 0.2;
void main(){
    gl_Position = ubo.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0);
    
    mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
    vec3 normalWorldSpace = normalize(normalMatrix * normal);

    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.directionToLight), 0);

    if (color.x != 0 || color.y != 0 || color.z != 0)
        fragColor = lightIntensity * color;
    else{
        vec4 colors = sphereColor();
        fragColor = colors.xyz;
        vec2 sphericalAngles = cartesianToSpherical(position);
        float rho = colors.w;
        gl_Position = ubo.projectionViewMatrix * push.modelMatrix * vec4(sphericalToCartesian(vec3(sphericalAngles, rho)), 1.0);
    }
}
/////////////////
////////////////

