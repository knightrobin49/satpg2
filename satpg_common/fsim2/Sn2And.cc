
/// @file Sn2And.cc
/// @brief Sn2And の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2And.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// Sn2And
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2And::Sn2And(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Gate(id, inputs)
{
}

// @brief デストラクタ
Sn2And::~Sn2And()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2And::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2And::_calc_gval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2And::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2And::_calc_lobs(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->gval();
  }
  for (ymuint i = ipos + 1; i < fanin_num(); ++ i) {
    obs &= _fanin(i)->gval();
  }
  return obs;
}

// @brief 内容をダンプする．
void
Sn2And::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "AND(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2And2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2And2::Sn2And2(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Gate2(id, inputs)
{
}

// @brief デストラクタ
Sn2And2::~Sn2And2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2And2::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2And2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 & pat1;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2And2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 & pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2And2::_calc_lobs(ymuint ipos)
{
  // 反対側の入力値が1の時観測される．
  ymuint alt_pos = ipos ^ 1;
  return mFanins[alt_pos]->gval();
}

// @brief 内容をダンプする．
void
Sn2And2::dump(ostream& s) const
{
  s << "AND2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2And3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2And3::Sn2And3(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Gate3(id, inputs)
{
}

// @brief デストラクタ
Sn2And3::~Sn2And3()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2And3::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2And3::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return pat0 & pat1 & pat2;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2And3::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return pat0 & pat1 & pat2;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2And3::_calc_lobs(ymuint ipos)
{
  // 残りの入力値がすべて1の時に観測される．
  switch ( ipos ) {
  case 0: return _fanin(1)->gval() & _fanin(2)->gval();
  case 1: return _fanin(0)->gval() & _fanin(2)->gval();
  case 2: return _fanin(0)->gval() & _fanin(1)->gval();
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
Sn2And3::dump(ostream& s) const
{
  s << "AND3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2And4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2And4::Sn2And4(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Gate4(id, inputs)
{
}

// @brief デストラクタ
Sn2And4::~Sn2And4()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2And4::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2And4::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2And4::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2And4::_calc_lobs(ymuint ipos)
{
  // 残りの入力値がすべて1の時に観測される．
  switch ( ipos ) {
  case 0: return _fanin(1)->gval() & _fanin(2)->gval() & _fanin(3)->gval();
  case 1: return _fanin(0)->gval() & _fanin(2)->gval() & _fanin(3)->gval();
  case 2: return _fanin(0)->gval() & _fanin(1)->gval() & _fanin(3)->gval();
  case 3: return _fanin(0)->gval() & _fanin(1)->gval() & _fanin(2)->gval();
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
Sn2And4::dump(ostream& s) const
{
  s << "AND4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nand::Sn2Nand(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2And(id, inputs)
{
}

// @brief デストラクタ
Sn2Nand::~Sn2Nand()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nand::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nand::_calc_gval()
{
  ymuint n = fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nand::_calc_fval()
{
  ymuint n = fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
Sn2Nand::dump(ostream& s) const
{
  ymuint n = fanin_num();
  s << "NAND(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nand2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nand2::Sn2Nand2(ymuint id,
		   const vector<SimNode2*>& inputs) :
  Sn2And2(id, inputs)
{
}

// @brief デストラクタ
Sn2Nand2::~Sn2Nand2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nand2::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nand2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 & pat1);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nand2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 & pat1);
}

// @brief 内容をダンプする．
void
Sn2Nand2::dump(ostream& s) const
{
  s << "NAND2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nand3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nand3::Sn2Nand3(ymuint id,
		   const vector<SimNode2*>& inputs) :
  Sn2And3(id, inputs)
{
}

// @brief デストラクタ
Sn2Nand3::~Sn2Nand3()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nand3::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nand3::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nand3::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 内容をダンプする．
void
Sn2Nand3::dump(ostream& s) const
{
  s << "NAND3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nand4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nand4::Sn2Nand4(ymuint id,
		   const vector<SimNode2*>& inputs) :
  Sn2And4(id, inputs)
{
}

// @brief デストラクタ
Sn2Nand4::~Sn2Nand4()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nand4::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nand4::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nand4::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 内容をダンプする．
void
Sn2Nand4::dump(ostream& s) const
{
  s << "NAND4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
