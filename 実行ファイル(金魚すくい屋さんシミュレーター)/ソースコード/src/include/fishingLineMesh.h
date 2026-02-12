//**************************************
//
// 釣り糸メッシュ処理 [fishingLineMesh.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _RAILMESH_H_// このマクロ定義がされていなかったら
#define _RAILMESH_H_// 2重インクルード防止のマクロ定義

//***********************
// インクルード
//***********************
#include "main.h"
#include "object.h"

//***********************
// 釣り糸メッシュクラス
//***********************
class CFishingLineMesh :public CObject
{
public:
	CFishingLineMesh(int nPriority = 2);	// コンストラクタ
	~CFishingLineMesh();					// デストラクタ
	static CFishingLineMesh* Create(D3DXVECTOR3 pos);	// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		// 向きの設定
	D3DXVECTOR3 GetPos(void) { return m_pos; };			// 位置情報
	int GetNumIdx(void) { return m_nNumVtx; }			// 頂点数情報の取得
	void Add(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);		// メッシュの追加
	void SetVtxPos(D3DXVECTOR3 pos,int nCnt);			// 頂点の位置設定
private:
	// ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスへのポインタ
	// 座標
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	// 数値
	int m_nNumVtx;								// 現在の頂点数
	int m_nNumIdx;								// 現在のインデックス数
};

#endif