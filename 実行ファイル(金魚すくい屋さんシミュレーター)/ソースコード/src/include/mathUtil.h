//**************************************************
// 
// 数式汎用処理処理 [mathUtil.h]
// Author:Sasaki Soichiro
//
//**************************************************
#ifndef _MATHUTIL_H_ // このマクロ定義がされていなかったら
#define _MATHUTIL_H_ // 二重インクルード防止のマクロ定義

//******************
// インクルード
//******************
#include "main.h"

namespace MathUtil
{
	// 数値を2分の1にする
	template <class T>
	T Half(const T data)
	{
		return data / static_cast<T>(2);
	}

	// 数値を2倍にする
	template <class T>
	T Double(const T data)
	{
		return data * static_cast<T>(2);
	}
}

#endif