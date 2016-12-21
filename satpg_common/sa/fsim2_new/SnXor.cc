
/// @file SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnXor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor::SnXor(const vector<PackedVal*>& inputs) :
  SnGate(inputs)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnXor::_calc_val()
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
SnXor::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SnXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor2::SnXor2(const vector<PackedVal*>& inputs) :
  SnGate2(inputs)
{
}

// @brief デストラクタ
SnXor2::~SnXor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnXor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 ^ val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnXor2::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SnXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor::SnXnor(const vector<PackedVal*>& inputs) :
  SnXor(inputs)
{
}

// @brief デストラクタ
SnXnor::~SnXnor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnXnor::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor2::SnXnor2(const vector<PackedVal*>& inputs) :
  SnXor2(inputs)
{
}

// @brief デストラクタ
SnXnor2::~SnXnor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnXnor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 ^ val1;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
