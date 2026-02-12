//**************************************************
//
// オブジェクトシェーダ3D処理 [objectShader3D.h]
// Author:Sasaki Soichiro
//
//**************************************************
#ifndef _OBJECTSHADER3D_H_// このマクロ定義がされていなかったら
#define _OBJECTSHADER3D_H_// 二重インクルード防止のマクロ定義
//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//*******************************
// オブジェクトシェーダ3Dクラス
//*******************************
class CObjectShader3D :public CObject
{
public:
	CObjectShader3D(int nPriority = 4);	// コンストラクタ
	~CObjectShader3D();					// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	virtual void SetShaderParam(void) = 0;	// シェーダのパラメータ設定(純数仮想関数)
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// 向き
	void SetSize(D3DXVECTOR2 size);					// 大きさ
	void SetCol(D3DXCOLOR col);
	void SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2);	// テクスチャ座標の設定
	void BindShader(LPD3DXEFFECT pShader);
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }		// 位置情報
	D3DXVECTOR3 GetRot(void) { return m_rot; }		// 向き情報
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2{ 0.0f,0.0f }; }		// 大きさ情報
	LPD3DXEFFECT GetShader(void) { return m_pShader; }
private:
	// ポインタ
	LPD3DXEFFECT m_pShader;				// シェーダのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR2 m_size;					// 大きさ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
};

#endif