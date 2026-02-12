//************************************
//
// タイトル処理 [title.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _TITLE_H_ // このマクロ定義がされていなかったら
#define _TITLE_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "scene.h"

//********************
// タイトルクラス
//********************
class CTitle :public CScene
{
public:
	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos) {};	// 位置
	void SetRot(D3DXVECTOR3 rot) {};	// 向き
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return NONE; }	// 位置情報
private:
};
#endif