//*****************************************
// 
// Xファイル処理 [Xmodel.h]
// Author:Sasaki Soichiro
//
//*****************************************
#ifndef _XMODEL_H_	// このマクロ定義がされていなかったら
#define _XMODEL_H_	// 二重インクルード防止のマクロ定義
//***************
// インクルード
//***************
#include "main.h"
#include <string>

//***********************
// Xファイルの管理クラス
//***********************
class CXModel
{
public:
	// Xファイルの構造体
	typedef struct
	{
		LPD3DXMESH pMesh;		// メッシュへのポインタ
		LPD3DXBUFFER pBuffMat;	// バッファへのポインタ
		DWORD dwNumMat;
		D3DXVECTOR3 vtxMin, vtxMax;// 大きさ
	}XMODEL;
	// 種類
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_MAX
	}TYPE;
	CXModel();	// コンストラクタ
	~CXModel();	// デストラクタ
	HRESULT Load(void);	// 読み込み
	void Unload(void);	// 破棄
	// セッター
	int Register(const char* pFilename);	// Xファイルの登録
	// ゲッター
	XMODEL GetAddres(int nIdx);				// Xファイルのインデックス
private:
	// 定数
	static constexpr int nMaxXModel = 256;
	// ポインタ
	XMODEL m_apXModel[nMaxXModel];	// Xファイルへのポインタ
	// 数値
	static int m_nNumAll;	// 最大数
	// 文字
	std::string XFILE[nMaxXModel] =// Xファイルのファイル名
	{
		"data\\MODEL\\aa.x",
	};
};

#endif