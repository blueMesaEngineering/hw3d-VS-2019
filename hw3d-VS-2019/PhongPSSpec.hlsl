#include "ShaderOps.hlsl"
#include "LightVectorData.hlsl"

#include "PointLight.hlsl"

cbuffer ObjectCBuf
{
    float specularPowerConst;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D tex;
Texture2D spec;

SamplerState splr;


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
	// Normalize the mesh normal
    viewNormal = normalize(viewNormal);
	// Fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// Specular parameters
    float specularPower = specularPowerConst;
    const float4 specularSample = spec.Sample(splr, tc);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
    if (hasGloss)
    {
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
	// Attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// Diffuse light
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// Specular reflected
    const float3 specularReflected = Speculate(
		  specularReflectionColor
		, 1.0f
		, viewNormal
		, lv.vToL
		, viewFragPos
		, att
		, specularPower
	);
	// Final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected), 1.0f);
}