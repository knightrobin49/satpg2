
/// @file SnOr.cc
/// @brief SnOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnOr.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr::SnOr(const vector<PackedVal*>& inputs) :
  SnGate(inputs)
{
}

// @brief デストラクタ
SnOr::~SnOr()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val |= _fanin_val(i);
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr::_calc_gobs(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= ~_fanin_val(i);
  }
  for (ymuint i = ipos + 1; i < _fanin_num(); ++ i) {
    obs &= ~_fanin_val(i);
  }
  return obs;
}


//////////////////////////////////////////////////////////////////////
// SnOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr2::SnOr2(const vector<PackedVal*>& inputs) :
  SnGate2(inputs)
{
}

// @brief デストラクタ
SnOr2::~SnOr2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 | val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return ~_fanin_val(alt_pos);
}


//////////////////////////////////////////////////////////////////////
// SnOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr3::SnOr3(const vector<PackedVal*>& inputs) :
  SnGate3(inputs)
{
}

// @brief デストラクタ
SnOr3::~SnOr3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 | val1 | val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr3::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return ~(_fanin_val(1) | _fanin_val(2));
  case 1: return ~(_fanin_val(0) | _fanin_val(2));
  case 2: return ~(_fanin_val(0) | _fanin_val(1));
  }
  return kPvAll0;
}


//////////////////////////////////////////////////////////////////////
// SnOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr4::SnOr4(const vector<PackedVal*>& inputs) :
  SnGate4(inputs)
{
}

// @brief デストラクタ
SnOr4::~SnOr4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 | val1 | val2 | val3;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr4::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return ~(_fanin_val(1) | _fanin_val(2) | _fanin_val(3));
  case 1: return ~(_fanin_val(0) | _fanin_val(2) | _fanin_val(3));
  case 2: return ~(_fanin_val(0) | _fanin_val(1) | _fanin_val(3));
  case 3: return ~(_fanin_val(0) | _fanin_val(1) | _fanin_val(2));
  }
  return kPvAll0;
}


//////////////////////////////////////////////////////////////////////
// SnNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor::SnNor(const vector<PackedVal*>& inputs) :
  SnOr(inputs)
{
}

// @brief デストラクタ
SnNor::~SnNor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val |= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor2::SnNor2(const vector<PackedVal*>& inputs) :
  SnOr2(inputs)
{
}

// @brief デストラクタ
SnNor2::~SnNor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 | val1;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor3::SnNor3(const vector<PackedVal*>& inputs) :
  SnOr3(inputs)
{
}

// @brief デストラクタ
SnNor3::~SnNor3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 | val1 | val2;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor4::SnNor4(const vector<PackedVal*>& inputs) :
  SnOr4(inputs)
{
}

// @brief デストラクタ
SnNor4::~SnNor4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 | val1 | val2 | val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
