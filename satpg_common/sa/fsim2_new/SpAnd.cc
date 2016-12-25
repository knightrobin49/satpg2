
/// @file SpAnd.cc
/// @brief SpAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpAnd.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SpAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpAnd::SpAnd(const vector<SimPrim*>& fanins) :
  SpGate(fanins)
{
}

// @brief デストラクタ
SpAnd::~SpAnd()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpAnd::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val &= _fanin_val(i);
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpAnd::_calc_gobs(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin_val(i);
  }
  for (ymuint i = ipos + 1; i < _fanin_num(); ++ i) {
    obs &= _fanin_val(i);
  }
  return obs;
}


//////////////////////////////////////////////////////////////////////
// SpAnd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpAnd2::SpAnd2(const vector<SimPrim*>& fanins) :
  SpGate2(fanins)
{
}

// @brief デストラクタ
SpAnd2::~SpAnd2()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpAnd2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 & val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpAnd2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin_val(alt_pos);
}


//////////////////////////////////////////////////////////////////////
// SpAnd3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpAnd3::SpAnd3(const vector<SimPrim*>& fanins) :
  SpGate3(fanins)
{
}

// @brief デストラクタ
SpAnd3::~SpAnd3()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpAnd3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 & val1 & val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpAnd3::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin_val(1) & _fanin_val(2);
  case 1: return _fanin_val(0) & _fanin_val(2);
  case 2: return _fanin_val(0) & _fanin_val(1);
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
}


//////////////////////////////////////////////////////////////////////
// SpAnd4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpAnd4::SpAnd4(const vector<SimPrim*>& fanins) :
  SpGate4(fanins)
{
}

// @brief デストラクタ
SpAnd4::~SpAnd4()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpAnd4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 & val1 & val2 & val3;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SpAnd4::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin_val(1) & _fanin_val(2) & _fanin_val(3);
  case 1: return _fanin_val(0) & _fanin_val(2) & _fanin_val(3);
  case 2: return _fanin_val(0) & _fanin_val(1) & _fanin_val(3);
  case 3: return _fanin_val(0) & _fanin_val(1) & _fanin_val(2);
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
}


//////////////////////////////////////////////////////////////////////
// SpNand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNand::SpNand(const vector<SimPrim*>& fanins) :
  SpAnd(fanins)
{
}

// @brief デストラクタ
SpNand::~SpNand()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpNand::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val &= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNand2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNand2::SpNand2(const vector<SimPrim*>& fanins) :
  SpAnd2(fanins)
{
}

// @brief デストラクタ
SpNand2::~SpNand2()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpNand2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 & val1;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNand3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNand3::SpNand3(const vector<SimPrim*>& fanins) :
  SpAnd3(fanins)
{
}

// @brief デストラクタ
SpNand3::~SpNand3()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpNand3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 & val1 & val2;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SpNand4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SpNand4::SpNand4(const vector<SimPrim*>& fanins) :
  SpAnd4(fanins)
{
}

// @brief デストラクタ
SpNand4::~SpNand4()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SpNand4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 & val1 & val2 & val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
