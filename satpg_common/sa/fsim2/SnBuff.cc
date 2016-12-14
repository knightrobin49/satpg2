
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
SnBuff::SnBuff(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnGate1(id, inputs)
{
}

// @brief デストラクタ
SnBuff::~SnBuff()
{
}

// @brief ゲートタイプを返す．
GateType
SnBuff::gate_type() const
{
  return kGateBUFF;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnBuff::_calc_gval2()
{
  return _fanin()->gval();
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnBuff::_calc_fval2()
{
  return _fanin()->fval();
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnBuff::_calc_lobs2(ymuint ipos)
{
  return kPvAll1;
}

// @brief 正常値の計算を行う．(3値版)
void
SnBuff::_calc_gval3()
{
  SimNode* inode = _fanin();
  PackedVal val0 = inode->gval_0();
  PackedVal val1 = inode->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnBuff::_calc_fval3(PackedVal mask)
{
  SimNode* inode = _fanin();
  PackedVal val0 = inode->fval_0();
  PackedVal val1 = inode->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnBuff::_calc_lobs3(ymuint ipos)
{
  return kPvAll1;
}


//////////////////////////////////////////////////////////////////////
// SnNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNot::SnNot(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnBuff(id, inputs)
{
}

// @brief デストラクタ
SnNot::~SnNot()
{
}

// @brief ゲートタイプを返す．
GateType
SnNot::gate_type() const
{
  return kGateNOT;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNot::_calc_gval2()
{
  return ~_fanin()->gval();
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNot::_calc_fval2()
{
  return ~_fanin()->fval();
}

// @brief 正常値の計算を行う．(3値版)
void
SnNot::_calc_gval3()
{
  SimNode* inode = _fanin();
  PackedVal val0 = inode->gval_1();
  PackedVal val1 = inode->gval_0();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNot::_calc_fval3(PackedVal mask)
{
  SimNode* inode = _fanin();
  PackedVal val0 = inode->fval_1();
  PackedVal val1 = inode->fval_0();
  set_fval(val0, val1, mask);
}

END_NAMESPACE_YM_SATPG_FSIM
