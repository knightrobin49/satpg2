
/// @file SnAnd.cc
/// @brief SnAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnAnd.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd::SnAnd(const vector<PackedVal*>& inputs) :
  SnGate(inputs)
{
}

// @brief デストラクタ
SnAnd::~SnAnd()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_val()
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
SnAnd::_calc_gobs(ymuint ipos)
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
// SnAnd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd2::SnAnd2(const vector<PackedVal*>& inputs) :
  SnGate2(inputs)
{
}

// @brief デストラクタ
SnAnd2::~SnAnd2()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 & val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin_val(alt_pos);
}


//////////////////////////////////////////////////////////////////////
// SnAnd3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd3::SnAnd3(const vector<PackedVal*>& inputs) :
  SnGate3(inputs)
{
}

// @brief デストラクタ
SnAnd3::~SnAnd3()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 & val1 & val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd3::_calc_gobs(ymuint ipos)
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
// SnAnd4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd4::SnAnd4(const vector<PackedVal*>& inputs) :
  SnGate4(inputs)
{
}

// @brief デストラクタ
SnAnd4::~SnAnd4()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_val()
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
SnAnd4::_calc_gobs(ymuint ipos)
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
// SnNand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand::SnNand(ymuint id,
	       const vector<PackedVal*>& inputs) :
  SnAnd(inputs)
{
}

// @brief デストラクタ
SnNand::~SnNand()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin_val(0);
  for (ymuint i = 1; i < n; ++ i) {
    val &= _fanin_val(i);
  }
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNand2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand2::SnNand2(const vector<PackedVal*>& inputs) :
  SnAnd2(inputs)
{
}

// @brief デストラクタ
SnNand2::~SnNand2()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val = val0 & val1;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNand3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand3::SnNand3(const vector<PackedVal*>& inputs) :
  SnAnd3(inputs)
{
}

// @brief デストラクタ
SnNand3::~SnNand3()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val = val0 & val1 & val2;
  return ~val;
}


//////////////////////////////////////////////////////////////////////
// SnNand4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand4::SnNand4(const vector<PackedVal*>& inputs) :
  SnAnd4(inputs)
{
}

// @brief デストラクタ
SnNand4::~SnNand4()
{
}

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_val()
{
  PackedVal val0 = _fanin_val(0);
  PackedVal val1 = _fanin_val(1);
  PackedVal val2 = _fanin_val(2);
  PackedVal val3 = _fanin_val(3);
  PackedVal val = val0 & val1 & val2 & val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
