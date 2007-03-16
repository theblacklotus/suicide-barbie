//--------------------------------------------------------------------------------------
// File: MutaliskUberShader.fx
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Global constants
//--------------------------------------------------------------------------------------
#define D3DTADDRESS_WRAP 1
#define D3DTADDRESS_CLAMP 3

#define D3DBLENDOP_ADD 1
#define D3DBLENDOP_SUBTRACT 2
#define D3DBLENDOP_REVSUBTRACT 3
#define D3DBLENDOP_MIN 4
#define D3DBLENDOP_MAX 5

#define D3DBLEND_ZERO 1
#define D3DBLEND_ONE 2
#define D3DBLEND_SRCCOLOR 3
#define D3DBLEND_INVSRCCOLOR 4
#define D3DBLEND_SRCALPHA 5
#define D3DBLEND_INVSRCALPHA 6
#define D3DBLEND_DESTALPHA 7
#define D3DBLEND_INVDESTALPHA 8
#define D3DBLEND_DESTCOLOR 9
#define D3DBLEND_INVDESTCOLOR 10
#define D3DBLEND_SRCALPHASAT 11
#define D3DBLEND_BOTHSRCALPHA 12
#define D3DBLEND_BOTHINVSRCALPHA 13
#define D3DBLEND_BLENDFACTOR 14
#define D3DBLEND_INVBLENDFACTOR 15

#define D3DCULL_NONE 1
#define D3DCULL_CW 2
#define D3DCULL_CCW 3

#define D3DCMP_EQUAL 3


//--------------------------------------------------------------------------------------
// Enum
//--------------------------------------------------------------------------------------

typedef int light_type;
const light_type lightDIRECTIONAL = 0;
const light_type lightPOINT = 1;
const light_type lightSPOT = 2;
const light_type lightDIRECTIONAL_EXT = 3;

typedef int texture_proj_type;
const texture_proj_type texture_projNONE = 0;
const texture_proj_type texture_projPOSITION = 1;
const texture_proj_type texture_projNORMAL = 2;
const texture_proj_type texture_projEYE_REFLECT = 3;
const texture_proj_type texture_projTEX_COORD0 = 4;

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
#define MAX_LIGHTS 4
const int MaxLights = MAX_LIGHTS;
float3 vLightPos[MAX_LIGHTS];				// Light's position in world space
float3 vLightDir[MAX_LIGHTS];				// Light's direction in world space
float3 vLightAttenuation[MAX_LIGHTS];		// Light's attenuations {attn0, attn1, attn2}
float  fLightSpotCutoff[MAX_LIGHTS];		// Light's spot cutoff
float  fLightSpotExp[MAX_LIGHTS];			// Light's spot exponent
float4 vLightDiffuse[MAX_LIGHTS];			// Light's diffuse color
float4 vLightSpecular[MAX_LIGHTS];			// Light's specular color
float4 vLightDiffuseAux0[MAX_LIGHTS];		// Light's diffuse auxilary color 0
float4 vLightDiffuseAux1[MAX_LIGHTS];		// Light's diffuse auxilary color 1
float4 vLightAmbient = float4(1,1,1,1);		// Light's ambient color
int nLightType[MAX_LIGHTS];					// Light's type
int iNumLights = 0;

texture tDiffuse;							// Diffuse texture for mesh
int nAddressU = D3DTADDRESS_WRAP;
int nAddressV = D3DTADDRESS_WRAP;
bool bDiffuseTextureEnabled = false;

float4 vMaterialAmbient = float4(1,1,1,1);	// Material's ambient color
float4 vMaterialDiffuse = float4(1,1,1,1);	// Material's diffuse color
float4 vMaterialSpecular = float4(0,0,0,0);	// Material's diffuse color

float4x4 mWorld;							// World matrix for object
float4x4 mInvWorld;							// Inverse world matrix for object
float4x4 mView;								// View matrix
float4x4 mViewProjection;					// View * Projection matrix
float4x4 mWorldViewProjection;				// World * View * Projection matrix

float4x4 mTextureProjection;				// Texture projection matrix
texture_proj_type nTextureProjType = 0;		// Texture projection type
float2 vUvOffset = float2(0, 0);			// Texture offset, if nTextureProjType == texture_projNONE

float fTime;								// App's time in seconds


int nCullMode = D3DCULL_CW;//D3DCULL_NONE;
bool bAlphaBlendEnable = true;//false;
int nBlendOp = D3DBLENDOP_ADD;
int nDestBlend = D3DBLEND_ONE;
int nSrcBlend = D3DBLEND_ONE;
//int nDestBlend = D3DBLEND_INVSRCALPHA;
//int nSrcBlend = D3DBLEND_SRCALPHA;
float fOpacity;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler sDiffuse = 
sampler_state
{
	Texture = <tDiffuse>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	
	AddressU = CLAMP;//<nAddressU>;
	AddressV = CLAMP;//<nAddressV>;
};


//--------------------------------------------------------------------------------------
// Vertex shader structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos			: POSITION;
	float3 Normal		: NORMAL;
	float2 TexCoord0	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Diffuse		: COLOR0;
	float4 Specular		: COLOR1;
	float4 TexCoord0	: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct DirectionalLight
{
	float4 diffuse;
	float4 specular;
	float3 dir;
};
struct DirectionalExtLight
{
	float4 diffuseFront;
	float4 diffuseBack;
	float4 equator;
	float4 specular;
	float3 dir;
};
struct PointLight
{
	float4 diffuse;
	float4 specular;
	float3 pos;
	float3 attn;
};
struct SpotLight
{
	float4 diffuse;
	float4 specular;
	float3 pos;
	float3 dir;
	float3 attn;
	float cutoff;
	float exponent;
};

DirectionalLight fetchDirectionalLight(int lightIndex)
{
	DirectionalLight o;
	o.diffuse = vLightDiffuse[lightIndex];
	o.specular = vLightSpecular[lightIndex];
	o.dir = vLightDir[lightIndex];
	return o;
}
DirectionalExtLight fetchDirectionalExtLight(int lightIndex)
{
	DirectionalExtLight o;
	o.diffuseFront = vLightDiffuse[lightIndex];
	o.diffuseBack = vLightDiffuseAux0[lightIndex];
	o.equator = vLightDiffuseAux1[lightIndex];
	o.specular = vLightSpecular[lightIndex];
	o.dir = vLightDir[lightIndex];
	return o;
}
PointLight fetchPointLight(int lightIndex)
{
	PointLight o;
	o.diffuse = vLightDiffuse[lightIndex];
	o.specular = vLightSpecular[lightIndex];
	o.pos = vLightPos[lightIndex];
	o.attn = vLightAttenuation[lightIndex];
	return o;
}
SpotLight fetchSpotLight(int lightIndex)
{
	SpotLight o;
	o.diffuse = vLightDiffuse[lightIndex];
	o.specular = vLightSpecular[lightIndex];
	o.pos = vLightPos[lightIndex];
	o.dir = vLightDir[lightIndex];
	o.attn = vLightAttenuation[lightIndex];
	o.cutoff = fLightSpotCutoff[lightIndex];
	o.exponent = fLightSpotExp[lightIndex];	
	return o;
}

float attenuation(float d, float3 attn)
{
	return 1.0f / ( attn.x + attn.y * d + attn.z * (d*d));
}

float3 diffuseLight(float3 pos, float3 normal, int lightIndex)
{
	float3 o = 0.0f;
	float lightType = nLightType[lightIndex];
	if(lightType == (float)lightDIRECTIONAL)
	{
		o = float3(1,0,0);
		DirectionalLight light = fetchDirectionalLight(lightIndex);
		o = max(0, dot(normal, light.dir)) *
			light.diffuse;
	}
	if(lightType == (float)lightDIRECTIONAL_EXT)
	{
		o = float3(1,0,0);
		DirectionalExtLight light = fetchDirectionalExtLight(lightIndex);
		float dotL = dot(normal, light.dir);
		if(dotL >= 0)
			o = lerp(light.equator, light.diffuseFront, dotL);
		else
			o = lerp(light.equator, light.diffuseBack, -dotL);
	}
	else if(lightType == (float)lightPOINT)
	{
		o = float3(0,1,0);
		PointLight light = fetchPointLight(lightIndex);
		float3 toLight = light.pos - pos;
		o = max(0, dot(normal, normalize(toLight))) *
			attenuation(length(toLight), light.attn) *
			light.diffuse;
	}
	else if(lightType == (float)lightSPOT)
	{
		o = float3(0,0,1);
		SpotLight light = fetchSpotLight(lightIndex);
		float3 toLight = light.pos - pos;
		float spotFactor = pow(max(0, dot(-light.dir, normalize(toLight)) - light.cutoff), light.exponent);
		o = max(0, dot(normal, normalize(toLight))) * spotFactor *
			attenuation(length(toLight), light.attn) *
			light.diffuse;
	}
	return o;
}

float3 specularLight(float3 pos, float3 normal, int lightIndex)
{
	float3 o = 0.0f;
	if(nLightType[lightIndex] == lightDIRECTIONAL)
	{
		DirectionalLight light = fetchDirectionalLight(lightIndex);
	}
	if(nLightType[lightIndex] == lightDIRECTIONAL_EXT)
	{
		DirectionalLight light = fetchDirectionalLight(lightIndex);
	}
	else if(nLightType[lightIndex] == lightPOINT)
	{
		PointLight light = fetchPointLight(lightIndex);
	}
	else if(nLightType[lightIndex] == lightSPOT)
	{
		SpotLight light = fetchSpotLight(lightIndex);
	}
	return o;
}

//--------------------------------------------------------------------------------------
VS_OUTPUT mainVS( VS_INPUT i )
{
	VS_OUTPUT o;
	
	o.Pos = mul(float4(i.Pos, 1.0f), mWorldViewProjection);
	float3 worldPos = mul(float4(i.Pos, 1.0f), mWorld).xyz;
	float3 worldNormal = normalize(mul(i.Normal, (float3x3)mWorld));
	float3 eyeToPos = mul(float4(worldPos, 1.0f), mView).xyz;
	
	float3 totalLightDiffuse = float3(0,0,0);
	float3 totalLightSpecular = float3(0,0,0);
	for(int q = 0; q < iNumLights; q++)
	{
		totalLightDiffuse += diffuseLight(worldPos, worldNormal, q);
		totalLightSpecular += specularLight(worldPos, worldNormal, q);
	}
	
	o.Diffuse.rgb = vMaterialDiffuse * totalLightDiffuse + 
					vMaterialAmbient * vLightAmbient;
	o.Diffuse.a = 1.0f;
	
	o.Specular.rgb = vMaterialSpecular * totalLightSpecular;
	o.Specular.a = 1.0f;
	
	o.TexCoord0 = 0;
	if(nTextureProjType == texture_projNONE)
	{
		o.TexCoord0.xy = i.TexCoord0.xy + vUvOffset;
	}
	else 
	{
		float4 texProjSrc = float4(0,0,0,0);
		if(nTextureProjType == texture_projPOSITION)
		{
			texProjSrc = float4(worldPos, 1.0f);
		}
		else if(nTextureProjType == texture_projNORMAL)
		{
			texProjSrc = float4(worldNormal, 1.0f);
		}
		else if(nTextureProjType == texture_projEYE_REFLECT)
		{
			texProjSrc.xyz = normalize(reflect(normalize(eyeToPos), worldNormal));
			texProjSrc.w = 1;
		}
		else if(nTextureProjType == texture_projTEX_COORD0)
		{
			texProjSrc.xy = i.TexCoord0;
		}
		
		o.TexCoord0 = mul(texProjSrc, mTextureProjection);
	}
	return o;
}

//--------------------------------------------------------------------------------------
float4 mainPS( VS_OUTPUT i ) : COLOR0
{
	float4 o;

	float4 diffuse = float4(1,1,1,1);
	if(bDiffuseTextureEnabled)
	{	
		if(nTextureProjType == texture_projNONE)
			diffuse = tex2D(sDiffuse, i.TexCoord0.xy);
		else
			diffuse = tex2Dproj(sDiffuse, i.TexCoord0);
	}
	
	o.rgb = diffuse * i.Diffuse;
	o.a = fOpacity * diffuse.a;

	return o;
}

//--------------------------------------------------------------------------------------
technique Main
{
	pass P0
	{
		VertexShader		= compile vs_3_0 mainVS();
		PixelShader			= compile ps_3_0 mainPS();
		CullMode			= <nCullMode>;

		AlphaBlendEnable	= <bAlphaBlendEnable>;
		BlendOp				= <nBlendOp>;
		DestBlend			= <nDestBlend>;
		SrcBlend			= <nSrcBlend>;
	}
	
	pass P1
	{
		VertexShader		= compile vs_3_0 mainVS();
		PixelShader			= compile ps_3_0 mainPS();
		CullMode			= <nCullMode>;

		AlphaBlendEnable	= <bAlphaBlendEnable>;
		BlendOp				= <nBlendOp>;
		DestBlend			= <nDestBlend>;
		SrcBlend			= <nSrcBlend>;
		
		ZWriteEnable		= False;
		ZFunc				= Equal;
	}	
}