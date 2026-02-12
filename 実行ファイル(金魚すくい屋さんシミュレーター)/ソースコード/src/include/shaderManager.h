//*****************************************
// 
// シェーダ読み込み処理 [shaderManager.h]
// Author:Sasaki Soichiro
//
//*****************************************
#ifndef _SHADERMANAGER_H_	// このマクロ定義がされていなかったら
#define _SHADERMANAGER_H_	// 二重インクルード防止のマクロ定義
//***************
// インクルード
//***************
#include "main.h"
#include <string>

//******************************
// シェーダファイルの管理クラス
//******************************
class CShaderManager
{
public:
	// 種類
	typedef enum
	{
		TYPE_MAX = 0,
	}TYPE;
	CShaderManager();	// コンストラクタ
	~CShaderManager();	// デストラクタ
	HRESULT Load(void);	// 読み込み
	void Unload(void);	// 破棄
	int Register(const char* pFilename);	// シェーダファイルの登録
	LPD3DXEFFECT GetAddres(int nIdx);		// シェーダファイルのインデックス
private:
	// 定数
	static constexpr int maxLoadNum = 256;
	// ポインタ
	LPD3DXEFFECT m_pShader[maxLoadNum];	// シェーダファイルへのポインタ
	// 数値
	static int m_nNumAll;			// 最大数
	// 文字
	const char* SHADERFILE[maxLoadNum] ={};// シェーダファイル名
};

#endif