
/// @file Sn2Input.cc
/// @brief Sn2Input の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Input.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// Sn2Input
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Input::Sn2Input(ymuint id) :
  SimNode(id)
{
  set_level(0);
}

// @brief デストラクタ
Sn2Input::~Sn2Input()
{
}

// @brief ゲートタイプを返す．
//
// ここでは kGateBUFF を返す．
GateType
Sn2Input::gate_type() const
{
  return kGateBUFF;
}

// @brief ファンイン数を得る．
ymuint
Sn2Input::fanin_num() const
{
  return 0;
}

// @brief pos 番めのファンインを得る．
SimNode*
Sn2Input::fanin(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Input::_calc_gval()
{
  return kPvAll0;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Input::_calc_fval()
{
  return kPvAll0;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Input::_calc_lobs(ymuint ipos)
{
  return kPvAll0;
}

// @brief 内容をダンプする．
void
Sn2Input::dump(ostream& s) const
{
  s << "INPUT" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Buff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Buff::Sn2Buff(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Gate1(id, inputs)
{
}

// @brief デストラクタ
Sn2Buff::~Sn2Buff()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Buff::gate_type() const
{
  return kGateBUFF;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Buff::_calc_gval()
{
  return mFanin->gval();
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Buff::_calc_fval()
{
  return mFanin->fval();
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Buff::_calc_lobs(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
Sn2Buff::dump(ostream& s) const
{
  s << "BUFF(" << mFanin->id() << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Not
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Not::Sn2Not(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Buff(id, inputs)
{
}

// @brief デストラクタ
Sn2Not::~Sn2Not()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Not::gate_type() const
{
  return kGateNOT;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Not::_calc_gval()
{
  return ~mFanin->gval();
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Not::_calc_fval()
{
  return ~mFanin->fval();
}

// @brief 内容をダンプする．
void
Sn2Not::dump(ostream& s) const
{
  s << "NOT(" << mFanin->id() << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
