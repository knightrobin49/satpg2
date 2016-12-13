
/// @file Sn2Xor.cc
/// @brief Sn2Xor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Xor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// Sn2Xor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Xor::Sn2Xor(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Gate(id, inputs)
{
}

// @brief デストラクタ
Sn2Xor::~Sn2Xor()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Xor::gate_type() const
{
  return kGateXOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Xor::_calc_gval()
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
Sn2Xor::_calc_fval()
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
Sn2Xor::calc_gobs(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
Sn2Xor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "XOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Xor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Xor2::Sn2Xor2(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Gate2(id, inputs)
{
}

// @brief デストラクタ
Sn2Xor2::~Sn2Xor2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Xor2::gate_type() const
{
  return kGateXOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Xor2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 ^ pat1;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Xor2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 ^ pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Xor2::calc_gobs(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
Sn2Xor2::dump(ostream& s) const
{
  s << "XOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Xnor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Xnor::Sn2Xnor(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Xor(id, inputs)
{
}

// @brief デストラクタ
Sn2Xnor::~Sn2Xnor()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Xnor::gate_type() const
{
  return kGateXNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Xnor::_calc_gval()
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
Sn2Xnor::_calc_fval()
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
Sn2Xnor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "XNOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Xnor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Xnor2::Sn2Xnor2(ymuint id,
		   const vector<SimNode2*>& inputs) :
  Sn2Xor2(id, inputs)
{
}

// @brief デストラクタ
Sn2Xnor2::~Sn2Xnor2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Xnor2::gate_type() const
{
  return kGateXNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Xnor2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 ^ pat1);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Xnor2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 ^ pat1);
}

// @brief 内容をダンプする．
void
Sn2Xnor2::dump(ostream& s) const
{
  s << "XNOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
