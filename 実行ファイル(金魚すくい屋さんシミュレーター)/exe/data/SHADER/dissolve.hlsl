//******************************************
//
// ディゾルブマテリアル処理 [dissolve.hlsl]
// Author:Sasaki Soichiro
//
//******************************************
// 定数
static const float width = 0.05f;
static const float minAlpha = 0.01f;
static const float4 edgeColor = { 0.5f, 0.5f, 8.0f, 0.0f };

float g_fTime;

// ロゴのテクスチャ
texture g_logoMap;
sampler logoSampler = sampler_state
{
    texture = <g_logoMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// 法線のテクスチャ
texture g_dissolveMap;
sampler dissolveSampler = sampler_state
{
    texture = <g_dissolveMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//**********************************
// 頂点シェーダのインプット情報
//**********************************
struct VS_IN
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

//***********************************
// 頂点シェーダのアウトプット情報
//***********************************
struct VS_OUT
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

//***********************************
// ピクセルシェーダのインプット情報
//***********************************
struct PS_IN
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

//********************
// 頂点シェーダ
//********************
VS_OUT VS(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    Out = In;
    
    return Out;
}

//********************
// ピクセルシェーダ
//********************
float4 PS(PS_IN In):COLOR0
{
    // ノイズテクスチャの数値を出してg_fTimeと比較して描画するか決める
    float fNoise = tex2D(dissolveSampler, In.uv).r;
    clip(fNoise - g_fTime);
    
    // ロゴのテクスチャ
    float4 logoColor = tex2D(logoSampler, In.uv);
    // 透明な部分は描画しない(エッジが出るとき透明な部分も描画してしまう)
    clip(logoColor.a - minAlpha);
    
    // エッジの部分を光らせる
    float edge = smoothstep(g_fTime, g_fTime + width, fNoise);
    float4 finalColor = lerp(edgeColor, logoColor, edge);

    return finalColor;
}

technique Basic
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}