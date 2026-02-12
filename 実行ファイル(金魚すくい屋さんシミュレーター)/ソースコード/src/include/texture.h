//*****************************
//
// テクスチャ処理 [texture.h]
// Author Sasaki Soichiro
//
//*****************************
#ifndef _TEXTURE_H_//このマクロ定義がされていなかったら
#define _TEXTURE_H_//二重インクルード防止のマクロ定義

//*******************
// インクルード
//*******************
#include "main.h"

//********************
// テクスチャクラス
//********************
class CTexture
{
public:
	// 種類
	typedef enum
	{
		TYPE_PLAYER=0,
		TYPE_ENEMY00,
		TYPE_ENEMY01,
		TYPE_BG00,
		TYPE_BG01,
		TYPE_BG02,
		TYPE_BULLET00,
		TYPE_BULLET01,
		TYPE_EFFECT,
		TYPE_NUMBER00,
		TYPE_NUMBER01,
		TYPE_NUMBER02,
		TYPE_COLON,
		TYPE_EXPLOSION,
		TYPE_NIWA,
		TYPE_MAX
	}TYPE;
	CTexture();		// コンストラクタ
	~CTexture();	// デストラクタ
	HRESULT Load(void);	// ロード
	void Unload(void);	// アンロード
	// セッター
	int Register(const char* pFilename);	// テクスチャの登録
	// ゲッター
	LPDIRECT3DTEXTURE9 GetAddres(int nIdx);	// テクスチャ情報取得
private:
	// 定数
	static constexpr int nMaxTexture = 256;
	// ポインタ
	LPDIRECT3DTEXTURE9 m_apTexture[nMaxTexture];// テクスチャのポインタ
	// 数値
	static int m_nNumAll;	// 現在のテクスチャ枚数
	// 文字
	const char* TEXTURE[nMaxTexture] =// テクスチャの名前
	{
		"data\\TEXTURE\\player000.png",
		"data\\TEXTURE\\enemy003.png",
		"data\\TEXTURE\\enemy001.png",
		"data\\TEXTURE\\umi002.png",
		"data\\TEXTURE\\umi001.png",
		"data\\TEXTURE\\umi000.png",
		"data\\TEXTURE\\bullet000.png",
		"data\\TEXTURE\\bullet001.png",
		"data\\TEXTURE\\effect000.jpg",
		"data\\TEXTURE\\number000.png",
		"data\\TEXTURE\\number002.png",
		"data\\TEXTURE\\number003.png",
		"data\\TEXTURE\\colon.png",
		"data\\TEXTURE\\icon.png",
		"data\\TEXTURE\\niwatori.jpeg",
	};
};



#endif