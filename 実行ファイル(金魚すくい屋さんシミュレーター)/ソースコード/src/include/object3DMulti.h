//**********************************************
//
// オブジェクト3Dマルチ処理 [object3DMulti.h]
// Author : Sasaki Soichiro
//
//**********************************************
#ifndef _OBJECT3DMULTI_H_ // このマクロ定義がされていなかったら
#define _OBJECT3DMULTI_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"

//****************************
// オブジェクト3Dマルチクラス
//****************************
class CObject3DMulti :public CObject
{
public:
	CObject3DMulti(int nPriority);	// コンストラクタ
	~CObject3DMulti();				// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }// 位置
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }// 向き
	void SetTex(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4);	// テクスチャ座標の設定1
	void SetTex2(D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR2 uv3, D3DXVECTOR2 uv4);	// テクスチャ座標の設定2
	void SetCol(D3DXCOLOR col);					// 色の設定
	void SetSize(D3DXVECTOR2 size);				// 大きさの設定
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture0, LPDIRECT3DTEXTURE9 pTexture1);// テクスチャの割り当て
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// 位置の設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// 向きの設定
	D3DXCOLOR GetCol(void) { return m_col; }	// 色の設定
	D3DXVECTOR2 GetSize(void) { return m_size; }// 大きさの設定
private:
	// ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture[2];	// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR2 m_size;					// 大きさ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXCOLOR m_col;					// 色

};
#endif