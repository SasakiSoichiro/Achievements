//***************************************
// 
// タイトルUI処理 [titleUI.h]
// Author:Sasaki Soichiro
//
//***************************************
#ifndef _TITLEUI_H_ // このマクロ定義がされていなかったら
#define _TITLEUI_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectShader2D.h"

//************************
// タイトルUIクラス
//************************
class CTitleUI :public CObjectShader2D
{
public:
	CTitleUI(int nPriority = 5);	// コンストラクタ
	~CTitleUI();					// デストラクタ
	static CTitleUI* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void SetShaderParam(void);		// パラメータの設定
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置設定
	void SetRot(D3DXVECTOR3 rot);	// 向き設定
	void Death(void) { m_bDeath = true; }
private:
	// 定数
	static constexpr float fDeltaTime = 0.01f;
	// 関数
	void DeathUpdate(void);	// 消すときの更新
	void ScreenUpdate(void);// 表示させるときの更新
	// ポインタ
	LPDIRECT3DTEXTURE9 m_pTextureDissolve;
	LPDIRECT3DTEXTURE9 m_pTextureLogo;
	// フラグ
	bool m_bDeath;
	// 数値
	int m_nIdxShader;	// 使っているテクスチャの番号
	float m_fTime;
};
#endif