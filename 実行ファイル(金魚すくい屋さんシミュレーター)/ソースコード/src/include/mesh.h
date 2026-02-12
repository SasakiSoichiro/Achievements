//******************************************
//
//	メッシュ処理 [mesh.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _MESHFIELD_H_// このマクロ定義がされていなかったら
#define _MESHFIELD_H_// 2重インクルード防止のマクロ定義

//***********************
// インクルード
//***********************
#include "main.h"
#include "object.h"
#include <fstream>
#include <vector>
#include <list>
#include <iostream>
#include "mathUtil.h"

//**********************
// マクロ定義
//**********************
#define OBJ_X (55.0f)	// X方向の大きさ
#define OBJ_Z (75.0f)	// Z方向の大きさ
#define XMESH (100)		// X方向の分割数
#define ZMESH (100)		// Z方向の分割数
#define VTXCNT ((XMESH+1)*(ZMESH+1))				// 頂点数の計算
#define POLYCNT ((2*XMESH*ZMESH)+4*(ZMESH-1))		// ポリゴン数の計算
#define IDXCNT (((XMESH+1)*2*ZMESH)+(ZMESH-1)*4)	// インデックス数の計算

//**********************
// 前方宣言
//**********************
class CWave;

//**********************
// メッシュクラス
//**********************
class CMesh :public CObject
{
public:
	CMesh(int nPriority = 5);
	~CMesh();
	static CMesh* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetShaderParam(void);				// パラメータの設定
	void WaveTrigger(D3DXVECTOR3 pos);		// 波紋の発生
	D3DXVECTOR3 CursorCollision(void);		// 波紋を発生させるための当たり判定
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };	// 位置
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };	// 向き
	void SetWave(bool bTrigger) { m_bWaveTirgger = bTrigger; }
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; };		// 位置情報
	D3DXVECTOR2 GetSize(void) { return m_size; }	// 大きさの取得
	float GetHeight(D3DXVECTOR3 pos);				// 高さ情報取得
	bool GetWaveTrigger(void) { return m_bWaveTirgger; }

private:
	// 関数
	void NorUpdate(void);			// 法線の更新
	void MeshNor(VERTEX_3D* pVtx, int nCntX, int nCntZ, int nCntPOS);// メッシュの法線設定
	void WaveTriggerUpdate(void);	// 波のトリガー判定の更新
	void WaveUpdate(void);			// 波紋の更新
	D3DXVECTOR3 VtxNor(VERTEX_3D* pVtx, int nCount0, int nStride0, int nStride1);// 法線計算用
	// 定数
	static constexpr int nStartXMeshOffset = 3;	// 最初のメッシュのオフセット
	static constexpr int nNorCenterRatio = 5;	// 法線の平均に使う数(中央)
	static constexpr int nNorSideRatio = 2;		// 法線の平均に使う数(辺)
	static constexpr int nNorNum = 4;			// 法線計算に使う角の数
	static constexpr int nNorPNum = 6;			// 法線計算に使う角の数(あたり判定)
	static constexpr int nStrideX = 1;			// X方向にずれる幅
	static constexpr int nStrideZ = XMESH + 1;	// Z方向にずれる幅
	static constexpr int nMeshLoopNumX = XMESH + 1;	// Xメッシュのループ回数
	static constexpr int nMeshLoopNumZ = ZMESH + 1;	// Zメッシュのループ回数
	static constexpr int nPolygonRightUp = 1;			// 法線計算の左上からのオフセット(右上)
	static constexpr int nPolygonLeftDown = XMESH + 1;	// 法線計算の左上からのオフセット(左下)
	static constexpr int nPolygonRightDown = XMESH + 2;	// 法線計算の左上からのオフセット(右下)
	static constexpr int nIsMeshCount = (XMESH + 1) * (ZMESH + 1);// 子のメッシュの数以下だったら判定
	static constexpr float fDeltaTime = 0.01f;	// 1フレームに進める時間
	static constexpr float fParallel = 0.0001f;	// 平行とみなすベクトル値
	// ポインタ
	LPDIRECT3DTEXTURE9 m_pTextureWater;	// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureNoise;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	LPD3DXEFFECT m_pShader;				// シェーダ
	std::list<CWave*> m_pWave;			// 波紋
	// 座標
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR2 m_size;		// サイズ
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	// フラグ
	bool  m_bWaveTirgger;	// 波を出すフラグ
	// 数値
	int m_WaveCnt;			// 波
	int m_nTime;			// 時間
	float m_fTime;			// 時間(float)
	float m_fHeightOffset;	// 高さのオフセット
};

#endif