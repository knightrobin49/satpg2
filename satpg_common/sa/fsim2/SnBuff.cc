
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

// @brief 内容をダンプする．
void
SnBuff::dump(ostream& s) const
{
  s << "BUFF(" << _fanin()->id() << ")" << endl;
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

// @brief 内容をダンプする．
void
SnNot::dump(ostream& s) const
{
  s << "NOT(" << _fanin()->id() << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
