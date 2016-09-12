float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;
float4x4 gViewProjMatrix;

float4 gWorldLightPosition;
float4 gLightColor;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mUV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mUV : TEXCOORD0;
	float4 mDepth : TEXCOORD1;
	float mDiffuse : TEXCOORD2;
};

VS_OUTPUT ShadowMapping_ApplyShaodw_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	float4 worldPosition = mul(Input.mPosition, gWorldMatrix);
	Output.mPosition = mul(worldPosition, gViewProjMatrix);

	Output.mDepth = mul(worldPosition, gLightViewMatrix);
	Output.mDepth = mul(Output.mDepth, gLightProjMatrix);

	float3 lightDir = normalize(worldPosition.xyz - gWorldLightPosition.xyz);
	float3 worldNormal = normalize(mul(Input.mNormal, (float3x3)gWorldMatrix));

	Output.mDiffuse = dot(-lightDir, worldNormal);

	Output.mUV = float2(Input.mUV.x, 1.0f - Input.mUV.y);

	return Output;
}

texture texShadowMap;

sampler2D ShadowSampler = sampler_state
{
	Texture = texShadowMap;
	ADDRESSU = CLAMP;
	ADDRESSV = CLAMP;
};

texture texDiffuseMap;

sampler2D DiffuseSampler = sampler_state
{
	Texture = texDiffuseMap;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

float4 gMaterialAmbient;
float4 gMaterialDiffuse;

struct PS_INPUT
{
	float2 mUV : TEXCOORD0;
	float4 mDepth : TEXCOORD1;
	float mDiffuse : TEXCOORD2;
};

float4 ShadowMapping_ApplyShaodw_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 diffuse = gMaterialAmbient.rgb + saturate(Input.mDiffuse) *  gMaterialDiffuse.rgb;
	diffuse = saturate(diffuse);

	float3 rgb = diffuse * albedo.rgb * gLightColor.rgb;

	float currentDepth = Input.mDepth.z / Input.mDepth.w;
	float2 uv = Input.mDepth.xy / Input.mDepth.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;

	float shadowDepth = tex2D(ShadowSampler, uv).r;

	if (currentDepth > shadowDepth + 0.0000125f)
	{
		rgb *= 0.5f;
	}

	return float4(rgb, 1.0f);
}
//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
	pass ApplyShaodwTorus
	{
		VertexShader = compile vs_2_0 ShadowMapping_ApplyShaodw_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ShadowMapping_ApplyShaodw_Pixel_Shader_ps_main();
	}

}