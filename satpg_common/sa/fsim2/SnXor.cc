
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
SnXor::SnXor(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief ゲートタイプを返す．
GateType
SnXor::gate_type() const
{
  return kGateXOR;
}

// @brief 正常値の計算を行う．
PackedVal
SnXor::_calc_gval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val ^= _fanin(i)->gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．
PackedVal
SnXor::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val ^= _fanin(i)->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
SnXor::_calc_lobs(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnXor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "XOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXor2::SnXor2(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnXor2::~SnXor2()
{
}

// @brief ゲートタイプを返す．
GateType
SnXor2::gate_type() const
{
  return kGateXOR;
}

// @brief 正常値の計算を行う．
PackedVal
SnXor2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 ^ pat1;
}

// @brief 故障値の計算を行う．
PackedVal
SnXor2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 ^ pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
SnXor2::_calc_lobs(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnXor2::dump(ostream& s) const
{
  s << "XOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor::SnXnor(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnXor(id, inputs)
{
}

// @brief デストラクタ
SnXnor::~SnXnor()
{
}

// @brief ゲートタイプを返す．
GateType
SnXnor::gate_type() const
{
  return kGateXNOR;
}

// @brief 正常値の計算を行う．
PackedVal
SnXnor::_calc_gval()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin(i)->gval();
  }
  return ~val;
}

// @brief 故障値の計算を行う．
PackedVal
SnXnor::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val ^= _fanin(i)->fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
SnXnor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "XNOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnXnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnXnor2::SnXnor2(ymuint id,
		 const vector<SimNode*>& inputs) :
  SnXor2(id, inputs)
{
}

// @brief デストラクタ
SnXnor2::~SnXnor2()
{
}

// @brief ゲートタイプを返す．
GateType
SnXnor2::gate_type() const
{
  return kGateXNOR;
}

// @brief 正常値の計算を行う．
PackedVal
SnXnor2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 ^ pat1);
}

// @brief 故障値の計算を行う．
PackedVal
SnXnor2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 ^ pat1);
}

// @brief 内容をダンプする．
void
SnXnor2::dump(ostream& s) const
{
  s << "XNOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
