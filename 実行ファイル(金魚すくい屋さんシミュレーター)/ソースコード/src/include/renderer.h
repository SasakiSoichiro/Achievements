//**************************************
//
// レンダリング処理 [renderer.h]
// Author:SasakiSoichiro
//
//**************************************
#ifndef _RENDERER_H_ // このマクロ定義がされていなかったら
#define _RENDERER_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

//********************
// レンダリングクラス
//********************
class CRenderer
{
public:
	CRenderer();	// コンストラクタ
	~CRenderer();	// デストラクタ
	HRESULT Init(HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit(void);						// 終了
	void Update(void);						// 更新
	void Draw(void);						// 描画
	// ゲッター
	LPDIRECT3DDEVICE9 GetDevice(void);		// デバイスの取得
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT[0]; }

private:
	// 定数
	static constexpr int nMaxMulti = 2;
	static constexpr float fPosOffset = -0.5f;
	// ポインタ
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuffMT;// フィードバック用ポリゴンの頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTextureMT[nMaxMulti];	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_pRenderMT[nMaxMulti];	// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;		// テクスチャレンダリング用Zバッファ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	D3DVIEWPORT9 m_viewportMT;			// テクスチャレンダリング用ビューポート
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ

};
#endif