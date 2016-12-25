
/// @file SpOr.cc
/// @brief SpOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpOr.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SpOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpOr::SpOr(const vector<SimPrim*>& fanins) :
  SpGate(fanins)
{
}

// @brief デストラクタ
SpOr::~SpOr()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpOr::_calc_val()
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
SpOr::_calc_gobs(ymuint ipos)
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
// SpOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpOr2::SpOr2(const vector<SimPrim*>& fanins) :
  SpGate2(fanins)
{
}

// @brief デストラクタ
SpOr2::~SpOr2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpOr2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 | val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpOr2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return ~_fanin_val(alt_pos);
}


//////////////////////////////////////////////////////////////////////
// SpOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpOr3::SpOr3(const vector<SimPrim*>& fanins) :
  SpGate3(fanins)
{
}

// @brief デストラクタ
SpOr3::~SpOr3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpOr3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 | val1 | val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpOr3::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return ~(_fanin_val(1) | _fanin_val(2));
  case 1: return ~(_fanin_val(0) | _fanin_val(2));
  case 2: return ~(_fanin_val(0) | _fanin_val(1));
  }
  return kPvAll0;
}


//////////////////////////////////////////////////////////////////////
// SpOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpOr4::SpOr4(const vector<SimPrim*>& fanins) :
  SpGate4(fanins)
{
}

// @brief デストラクタ
SpOr4::~SpOr4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpOr4::_calc_val()
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
SpOr4::_calc_gobs(ymuint ipos)
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
// SpNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNor::SpNor(const vector<SimPrim*>& fanins) :
  SpOr(fanins)
{
}

// @brief デストラクタ
SpNor::~SpNor()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpNor::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val |= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNor2::SpNor2(const vector<SimPrim*>& fanins) :
  SpOr2(fanins)
{
}

// @brief デストラクタ
SpNor2::~SpNor2()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpNor2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 | val1;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNor3::SpNor3(const vector<SimPrim*>& fanins) :
  SpOr3(fanins)
{
}

// @brief デストラクタ
SpNor3::~SpNor3()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpNor3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 | val1 | val2;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNor4::SpNor4(const vector<SimPrim*>& fanins) :
  SpOr4(fanins)
{
}

// @brief デストラクタ
SpNor4::~SpNor4()
{
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SpNor4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 | val1 | val2 | val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
