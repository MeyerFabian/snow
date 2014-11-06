in vec4 Position0;
in vec4 Normal0;
in vec2 Texture0;
in vec4 LightSpacePos;

uniform vec3 gLightPosition;
uniform vec3 gCameraPos;
uniform sampler2D gSampler;
uniform sampler2D gShadowMap;
uniform float gAmbient;
uniform vec3 gColor;
uniform float gDiffuse;
uniform int gSpecPower;
uniform vec3 gSpecInt;

void main(void)
{

    vec4 textureColor = texture2D(gSampler,Texture0);

    vec4 ambientColor = vec4(gColor,0.0) * gAmbient;

    vec3 I = normalize(gLightPosition.xyz-Position0.xyz);
    vec3 N = normalize(Normal0).xyz;

    //Diffuse
    float diffuseIntensity = dot(N, I);
    vec4 diffuseColor;
    if(diffuseIntensity<0.0){
        diffuseColor = vec4(0,0,0,0);
    }else{
        diffuseColor= vec4(gColor,1.0)*gDiffuse*diffuseIntensity;
    }

    //Specular
    vec3 PosToCam = normalize(gCameraPos-Position0.xyz);
    float specDot = dot(normalize(reflect(-I,N)), PosToCam);
    float specFactor = pow(specDot,gSpecPower);
    vec4 specColor;
    if(specFactor<0){
        specColor = vec4(0,0,0,0);
    }else{
        specColor= clamp(vec4(gColor * gSpecInt* specFactor,1.0),0.0,1.0) ;
    }

    //ShadowMap
    vec3 LightSpacePos0 = LightSpacePos.xyz/LightSpacePos.w;
    vec2 UV= vec2(0.5 + 0.5 * LightSpacePos0.x,0.5 + 0.5 * LightSpacePos0.y);
    float z = 0.5 + 0.5 * LightSpacePos0.z;
    float ShadowFactor =1.0f;
    float Depth = texture2D(gShadowMap,UV).x;
    if(Depth < (z-0.001)){
        ShadowFactor = 0.5f;
    }


    gl_FragColor = clamp(textureColor * (ambientColor + ShadowFactor * (diffuseColor + specColor)),0.0,1.0);
}
