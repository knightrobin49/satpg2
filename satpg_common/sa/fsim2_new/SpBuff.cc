
/// @file SpBuff.cc
/// @brief SpBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpBuff.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SpBuff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpBuff::SpBuff(const vector<SimPrim*>& fanins) :
  SpGate1(fanins)
{
}

// @brief デストラクタ
SpBuff::~SpBuff()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpBuff::_calc_val()
{
  PackedVal val = _fanin_val();
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpBuff::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SpNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNot::SpNot(const vector<SimPrim*>& fanins) :
  SpBuff(fanins)
{
}

// @brief デストラクタ
SpNot::~SpNot()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpNot::_calc_val()
{
  PackedVal val = _fanin_val();
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
