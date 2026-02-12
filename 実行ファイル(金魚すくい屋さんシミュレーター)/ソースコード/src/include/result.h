//************************************
//
// リザルト処理 [result.h]
// Author:Sasaki Soichiro
//
//************************************
#ifndef _RESULT_H_ // このマクロ定義がされていなかったら
#define _RESULT_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "scene.h"

//**********************
// 前方宣言
//**********************
class CObject2D;

//********************
// リザルトクラス
//********************
class CResult :public CScene
{
public:
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
private:
};
#endif