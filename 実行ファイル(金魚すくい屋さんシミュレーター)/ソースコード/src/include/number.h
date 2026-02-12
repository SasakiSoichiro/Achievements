//********************************
// 
// ナンバークラス
// Author Sasaki Soichiro
// 
//********************************
#ifndef _NUMBER_H_ // このマクロ定義がされていなかったら
#define _NUMBER_H_ // 二重インクルード防止のマクロ定義

//****************
// インクルード
//****************
#include "main.h"

//******************
// ナンバークラス
//******************
class CNumber
{
public:
	CNumber();	// コンストラクタ
	~CNumber();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos);	// 位置の設定
	void SetNumber(int nNum);		// ナンバーの設定
	void SetSize(D3DXVECTOR2 size);	// 大きさの設定
	void SetCol(D3DXCOLOR col);		// 色の設定
private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR2 m_size;					// 大きさ
	int nNum;							// 数情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
};

//******************
// 3Dナンバークラス
//******************
class CNumber3D
{
public:
	CNumber3D();	// コンストラクタ
	~CNumber3D();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Draw(void);	// 描画
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// 向きの設定
	void SetSize(D3DXVECTOR2 size);	// 大きさの設定
	void SetCol(D3DXCOLOR col);		// 色の設定
	void SetNumber(int nNum);		// ナンバーの設定
private:
	// ポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 位置
	D3DXVECTOR2 m_size;					// 大きさ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	// 数値
	int nNum;							// 数情報
};

//****************
// 記号クラス
//****************
class CSymbol
{
public:
	CSymbol();	// コンストラクタ
	~CSymbol();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);					// 位置の設定
	void SetSize(D3DXVECTOR2 size);					// 大きさの設定
	void SetCol(D3DXCOLOR col);						// 位置の設定
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);	// テクスチャの設定
private:
	// ポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報のポインタ
	// 座標
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR2 m_size;					// 大きさ
};
#endif