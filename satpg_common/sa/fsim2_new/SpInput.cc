
/// @file SpInput.cc
/// @brief SpInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpInput.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SpInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpInput::SpInput()
{
}

// @brief デストラクタ
SpInput::~SpInput()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpInput::_calc_val()
{
  ASSERT_NOT_REACHED;
  return kPvAll0;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpInput::_calc_gobs(ymuint ipos)
{
  return kPvAll0;
}

END_NAMESPACE_YM_SATPG_FSIM
