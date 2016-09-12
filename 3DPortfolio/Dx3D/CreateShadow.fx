texture texShadowMap;

float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix;
float4x4 gLightProjMatrix;

struct VS_INPUT
{
	float4 mPosition : POSITION;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float4 mDepth : TEXCOORD1;
};

VS_OUTPUT ShadowMapping_CreateShadow_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gLightViewMatrix);
	Output.mPosition = mul(Output.mPosition, gLightProjMatrix);

	Output.mDepth = Output.mPosition;

	return Output;
}

struct PS_INPUT
{
	float4 mDepth : TEXCOORD1;
};

float4 ShadowMapping_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float depth = Input.mDepth.z / Input.mDepth.w;
return float4(depth.xxx, 1.0f);
}

technique ShadowMapping
{
	pass CreateShadow
	{
		VertexShader = compile vs_2_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
	}
}