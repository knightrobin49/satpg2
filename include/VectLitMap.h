﻿#ifndef VECTLITMAP_H
#define VECTLITMAP_H

/// @file VectLitMap.h
/// @brief VectLitMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class VectLitMap LitMap.h "LitMap.h"
/// @brief すべての変数を指定した LitMap
//////////////////////////////////////////////////////////////////////
class VectLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] ivars 入力の変数のベクタ
  /// @param[in] ovar 出力の変数
  VectLitMap(const vector<SatVarId>& ivars,
	     SatVarId ovar);

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const;

  /// @brief 入力のリテラルを返す．
  virtual
  SatLiteral
  input(ymuint pos) const;

  /// @brief 出力のリテラルを返す．
  virtual
  SatLiteral
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の変数
  vector<SatVarId> mIvars;

  // 出力の変数
  SatVarId mOvar;

};

END_NAMESPACE_YM_SATPG

#endif // LITMAP_H
