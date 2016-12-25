
/// @file SpXor.cc
/// @brief SpXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpXor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SpXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpXor::SpXor(const vector<SimPrim*>& fanins) :
  SpGate(fanins)
{
}

// @brief デストラクタ
SpXor::~SpXor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpXor::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin_val(i);
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpXor::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SpXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpXor2::SpXor2(const vector<SimPrim*>& fanins) :
  SpGate2(fanins)
{
}

// @brief デストラクタ
SpXor2::~SpXor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpXor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 ^ val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpXor2::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SpXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpXnor::SpXnor(const vector<SimPrim*>& fanins) :
  SpXor(fanins)
{
}

// @brief デストラクタ
SpXnor::~SpXnor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpXnor::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpXnor2::SpXnor2(const vector<SimPrim*>& fanins) :
  SpXor2(fanins)
{
}

// @brief デストラクタ
SpXnor2::~SpXnor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpXnor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 ^ val1;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
