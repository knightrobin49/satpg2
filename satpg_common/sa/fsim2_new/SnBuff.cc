
/// @file SnBuff.cc
/// @brief SnBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnBuff.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnBuff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnBuff::SnBuff(const vector<PackedVal*>& inputs) :
  SnGate1(inputs)
{
}

// @brief デストラクタ
SnBuff::~SnBuff()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnBuff::_calc_val()
{
  PackedVal val = _fanin_val();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnBuff::_calc_obs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SnNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNot::SnNot(const vector<PackedVal*>& inputs) :
  SnBuff(inputs)
{
}

// @brief デストラクタ
SnNot::~SnNot()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNot::_calc_val()
{
  PackedVal val = _fanin_val();
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
