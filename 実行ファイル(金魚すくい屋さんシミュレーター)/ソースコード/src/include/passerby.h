//**********************************************
// 
// 通行人処理 [passerby.h]
// Author:Sasaki Soichiro
//
//**********************************************
#ifndef _PASSERBY_H_ // このマクロ定義がされていなかったら
#define _PASSERBY_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "item.h"
#include "customerState.h"
#include "motion.h"

//******************
// 前方宣言
//******************
class CModel;
class CMotion;

//****************************
// 通行人クラス
//****************************
class CPasserby :CObject
{
public:
	// モーション
	typedef enum
	{
		MOTION_NONE = 0,
		MOTION_MOVE,
		MOTION_DASH,
		MOTION_MAX
	}MOTION;

	CPasserby(int nPriority = 4);	// コンストラクタ
	~CPasserby();					// デストラクタ

	static CPasserby* Create(D3DXVECTOR3 pos, bool bRight,bool bDash);

	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// セッター
	void Death(void) { m_bDeath = true; }	// 更新途中で消すときの処理
private:
	// 定数
	static constexpr int maxModelParts = 12;
	static constexpr int UninitCnt = 2000;
	static constexpr float walkSpeed = 0.5f;
	static constexpr float dashSpeed = 1.5f;
	// ポインタ
	CModel* m_apModel[maxModelParts];
	CMotion* m_pMotion;
	// 座標
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxWorld;
	// フラグ
	bool m_bDeath;
	bool m_bRight;
	bool m_bDash;
	// 数値
	int m_nNumModel;
	int m_nUninitCnt;
	float m_fSpeed;
};
#endif