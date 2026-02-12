//**********************************************
// 
// 波紋処理 [wave.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _WAVE_H_ // このマクロ定義がされていなかったら
#define _WAVE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

//***********************
// 構造体
//***********************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pMeshVtxBuff;	// メッシュの頂点バッファ
	int nXMesh;								// 分割数
	int nZMesh;								// 分割数
	D3DXVECTOR2 uv;							// uv座標
	D3DXVECTOR2 size;						// サイズ
	float fHeightOffset;					// 高さのオフセット
}WAVE;

//****************************
// 波紋クラス
//****************************
class CWave
{
public:
	CWave();	// コンストラクタ
	~CWave();	// デストラクタ
	static CWave* Create(WAVE wave);
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	// ゲッター
	bool GetIsLife() { return m_bLife; }	// 生きてるか
private:
	// 波紋の輪の構造体
	typedef struct
	{
		VERTEX_3D* pVtx;
		float Xmesh;
		float Zmesh;
		int nCntX;
		int nCntZ;
		int nCntPOS;
		float fDistance;
		bool bWaveRing;
	}WAVERING;
	// 時間が経った処理の構造体
	typedef struct
	{
		VERTEX_3D* pVtx;
		float Xmesh;
		float Zmesh;
		int nCntX;
		int nCntZ;
		int nCntPOS;
	}NOLIFE;
	// 関数
	bool WaveRing(float fDistance);
	float MaxDistance(float fDistance);
	void WaveRingPos(WAVERING waveRing);
	void NoLifePos(NOLIFE noLife);
	// 定数
	static constexpr int nLife = 150;
	static constexpr float fWaveRatio = 15.0f;
	static constexpr float fAmount = 5.0f;
	static constexpr float fSpeed = 0.1f;
	static constexpr float fWidth = 0.2f;
	static constexpr float fScale = 0.5f;
	// 構造体
	WAVE m_Wave;
	// フラグ
	bool m_bLife;
	// 数値
	int m_nTime;
};
#endif