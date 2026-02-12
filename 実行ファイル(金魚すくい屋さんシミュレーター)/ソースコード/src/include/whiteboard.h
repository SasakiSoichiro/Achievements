//**************************************
//
// ホワイトボード処理 [whiteboard.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _WHITEBOARD_H_ // このマクロ定義がされていなかったら
#define _WHITEBOARD_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "objectX.h"

//******************
// 前方宣言
//******************
class CObject3D;
class CMoney3D;

//**********************
// ホワイトボードクラス
//**********************
class CWhiteboard :public CObjectX
{
public:
	typedef enum
	{
		TYPE_REVIEW=0,
		TYPE_SCORE,
		TYPE_MAX
	}TYPE;
	CWhiteboard(int nPriority = 4);	// コンストラクタ
	~CWhiteboard();					// デストラクタ
	static CWhiteboard* Create(D3DXVECTOR3 pos);	// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);			// 位置
	void SetRot(D3DXVECTOR3 rot);			// 向き
	void SetXModel(const char* fileName);	// 使うモデルの設定
	// ゲッター
	D3DXVECTOR3 GetPos(void);				// 位置情報の取得
	D3DXVECTOR3 GetRot(void);				// 向き情報の取得
	CXModel::XMODEL GetXModel(void);		// モデル情報の取得
private:
	// 定数
	static constexpr int nMaxUI= 2;
	// ポインタ
	CObject3D* m_pUI[nMaxUI];	// ホワイトボードに映すUI
	CMoney3D* m_pMoney3D;
	// 数値
	int m_nIdxXFile;		// 使うモデルの番号

};
#endif