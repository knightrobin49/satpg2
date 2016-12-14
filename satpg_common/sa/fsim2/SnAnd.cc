
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
SnAnd::SnAnd(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnAnd::~SnAnd()
{
}

// @brief ゲートタイプを返す．
GateType
SnAnd::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_gval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_fval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd::_calc_lobs2(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->gval();
  }
  for (ymuint i = ipos + 1; i < _fanin_num(); ++ i) {
    obs &= _fanin(i)->gval();
  }
  return obs;
}

// @brief 内容をダンプする．
void
SnAnd::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "AND(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd2::SnAnd2(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnAnd2::~SnAnd2()
{
}

// @brief ゲートタイプを返す．
GateType
SnAnd2::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 & pat1;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 & pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd2::_calc_lobs2(ymuint ipos)
{
  return _fanin(ipos ^ 1)->gval();
}

// @brief 内容をダンプする．
void
SnAnd2::dump(ostream& s) const
{
  s << "AND2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd3::SnAnd3(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnAnd3::~SnAnd3()
{
}

// @brief ゲートタイプを返す．
GateType
SnAnd3::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return pat0 & pat1 & pat2;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return pat0 & pat1 & pat2;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd3::_calc_lobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval() & _fanin(2)->gval();
  case 1: return _fanin(0)->gval() & _fanin(2)->gval();
  case 2: return _fanin(0)->gval() & _fanin(1)->gval();
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnAnd3::dump(ostream& s) const
{
  s << "AND3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnAnd4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnAnd4::SnAnd4(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnAnd4::~SnAnd4()
{
}

// @brief ゲートタイプを返す．
GateType
SnAnd4::gate_type() const
{
  return kGateAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return pat0 & pat1 & pat2 & pat3;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd4::_calc_lobs2(ymuint ipos)
{
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
SnAnd4::dump(ostream& s) const
{
  s << "AND4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand::SnNand(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnAnd(id, inputs)
{
}

// @brief デストラクタ
SnNand::~SnNand()
{
}

// @brief ゲートタイプを返す．
GateType
SnNand::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand::_calc_gval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand::_calc_fval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val &= _fanin(i)->fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
SnNand::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "NAND(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand2::SnNand2(ymuint id,
		 const vector<SimNode*>& inputs) :
  SnAnd2(id, inputs)
{
}

// @brief デストラクタ
SnNand2::~SnNand2()
{
}

// @brief ゲートタイプを返す．
GateType
SnNand2::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 & pat1);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 & pat1);
}

// @brief 内容をダンプする．
void
SnNand2::dump(ostream& s) const
{
  s << "NAND2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand3::SnNand3(ymuint id,
		 const vector<SimNode*>& inputs) :
  SnAnd3(id, inputs)
{
}

// @brief デストラクタ
SnNand3::~SnNand3()
{
}

// @brief ゲートタイプを返す．
GateType
SnNand3::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return ~(pat0 & pat1 & pat2);
}

// @brief 内容をダンプする．
void
SnNand3::dump(ostream& s) const
{
  s << "NAND3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNand4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNand4::SnNand4(ymuint id,
		 const vector<SimNode*>& inputs) :
  SnAnd4(id, inputs)
{
}

// @brief デストラクタ
SnNand4::~SnNand4()
{
}

// @brief ゲートタイプを返す．
GateType
SnNand4::gate_type() const
{
  return kGateNAND;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return ~(pat0 & pat1 & pat2 & pat3);
}

// @brief 内容をダンプする．
void
SnNand4::dump(ostream& s) const
{
  s << "NAND4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
