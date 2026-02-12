//**************************************
//
// お金3D処理 [money3D.h]
// Author:Sasaki Soichiro
//
//**************************************
#ifndef _MONEY3D_H_ // このマクロ定義がされていなかったら
#define _MONEY3D_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"
#include "object.h"
#include "scene.h"

class CNumber3D;
class CObject3D;

//*****************
// マクロ定義
//*****************
#define MAX_DIGIT (5)	// 最大桁数

//********************
// お金3Dクラス
//********************
class CMoney3D :public CObject
{
public:
	CMoney3D(int nPriority = 5);	// コンストラクタ
	~CMoney3D();					// デストラクタ
	static CMoney3D* Create(D3DXVECTOR3 pos);// インスタンス生成
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	// セッター
	void SetPos(D3DXVECTOR3 pos);	// 位置
	void SetRot(D3DXVECTOR3 rot);	// 向き
	int Digit(int nScore);			// 桁数情報の取得
	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }		// 位置情報の取得
	D3DXVECTOR2 GetSize(void) { return m_size; }	// 大きさ情報の取得
private:
	// 定数
	static constexpr int nTextureNum = 2;	// 使うテクスチャの数
	static constexpr float fSizeX = 5.0f;
	static constexpr float fSizeY = 10.0f;
	static constexpr float fOffsetX00 = 40.0f;
	static constexpr float fOffsetX01 = 10.0f;
	static constexpr float fTextureSize_X = 10.0f;
	static constexpr float fTextureSize_Y = 10.0f;
	// ポインタ
	static CNumber3D* m_apNumber[MAX_DIGIT];	// ナンバーへのポインタ
	CObject3D* m_pObject3D[2];					// オブジェクト2Dのポインタ
	// 座標
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR2 m_size;							// 大きさ
	// 数値
	int  m_nIdxTexture;							// テクスチャのインデックス

};
#endif