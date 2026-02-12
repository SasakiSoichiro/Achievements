//******************************************
//
// オブジェクトシェーダ2D処理 [objectShader2D.h]
// Author:Sasaki Soichiro
//
//******************************************
#ifndef _OBJECTSHADER2D_H_ // このマクロ定義がされていなかったら
#define _OBJECTSHADER2D_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//**************************************
// オブジェクトシェーダ2Dクラス
//**************************************
class CObjectShader2D :public CObject
{
public:
	CObjectShader2D(int nPriority = 7);	// コンストラクタ
	~CObjectShader2D();				// デストラクタ
	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	virtual void SetShaderParam(void) = 0;// シェーダのパラメータ設定(純数仮想関数)
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置の設定
	void SetRot(D3DXVECTOR3 rot);	// 向きの設定
	void SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4);// UV座標の設定
	void SetCol(D3DXCOLOR col);					// 色の設定
	void SetSize(D3DXVECTOR2 size);				// 大きさの設定
	void BindShader(LPD3DXEFFECT pShader);
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// 位置情報の取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// 向き情報の取得
	D3DXCOLOR GetCol(void) { return m_col; }	// 色情報の取得
	D3DXVECTOR2 GetSize(void) { return m_size; }	// 大きさ情報の取得
	LPD3DXEFFECT GetShader(void) { return m_pShader; }
private:
	// ポインタ
	LPD3DXEFFECT m_pShader;				// シェーダのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR2 m_size;					// 大きさ
	D3DXCOLOR m_col;					// 色
};
#endif