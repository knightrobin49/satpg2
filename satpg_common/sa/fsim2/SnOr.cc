﻿
/// @file SnOr.cc
/// @brief SnOr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnOr.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// SnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr::SnOr(ymuint id,
	   const vector<SimNode*>& inputs) :
  SnGate(id, inputs)
{
}

// @brief デストラクタ
SnOr::~SnOr()
{
}

// @brief ゲートタイプを返す．
GateType
SnOr::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr::_calc_gval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->gval();
  }
  return new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnOr::_calc_fval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr::_calc_lobs2(ymuint ipos)
{
  PackedVal obs = kPvAll0;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs |= _fanin(i)->gval();
  }
  for (ymuint i = ipos + 1; i < _fanin_num(); ++ i) {
    obs |= _fanin(i)->gval();
  }
  return ~obs;
}

// @brief 内容をダンプする．
void
SnOr::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "OR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr2::SnOr2(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnGate2(id, inputs)
{
}

// @brief デストラクタ
SnOr2::~SnOr2()
{
}

// @brief ゲートタイプを返す．
GateType
SnOr2::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr2::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 | pat1;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnOr2::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 | pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr2::_calc_lobs2(ymuint ipos)
{
  return ~_fanin(ipos ^ 1)->gval();
}

// @brief 内容をダンプする．
void
SnOr2::dump(ostream& s) const
{
  s << "OR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr3::SnOr3(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnGate3(id, inputs)
{
}

// @brief デストラクタ
SnOr3::~SnOr3()
{
}

// @brief ゲートタイプを返す．
GateType
SnOr3::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr3::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return pat0 | pat1 | pat2;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnOr3::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return pat0 | pat1 | pat2;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr3::_calc_lobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return ~(_fanin(1)->gval() | _fanin(2)->gval());
  case 1: return ~(_fanin(0)->gval() | _fanin(2)->gval());
  case 2: return ~(_fanin(0)->gval() | _fanin(1)->gval());
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnOr3::dump(ostream& s) const
{
  s << "OR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnOr4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnOr4::SnOr4(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnGate4(id, inputs)
{
}

// @brief デストラクタ
SnOr4::~SnOr4()
{
}

// @brief ゲートタイプを返す．
GateType
SnOr4::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnOr4::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnOr4::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnOr4::_calc_lobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return ~(_fanin(1)->gval() | _fanin(2)->gval() | _fanin(3)->gval());
  case 1: return ~(_fanin(0)->gval() | _fanin(2)->gval() | _fanin(3)->gval());
  case 2: return ~(_fanin(0)->gval() | _fanin(1)->gval() | _fanin(3)->gval());
  case 3: return ~(_fanin(0)->gval() | _fanin(1)->gval() | _fanin(2)->gval());
  }
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnOr4::dump(ostream& s) const
{
  s << "OR4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor::SnNor(ymuint id,
	     const vector<SimNode*>& inputs) :
  SnOr(id, inputs)
{
}

// @brief デストラクタ
SnNor::~SnNor()
{
}

// @brief ゲートタイプを返す．
GateType
SnNor::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor::_calc_gval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNor::_calc_fval2()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->fval();
  }
  return ~new_val;
}

// @brief 内容をダンプする．
void
SnNor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "NOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor2::SnNor2(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnOr2(id, inputs)
{
}

// @brief デストラクタ
SnNor2::~SnNor2()
{
}

// @brief ゲートタイプを返す．
GateType
SnNor2::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor2::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 | pat1);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNor2::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 | pat1);
}

// @brief 内容をダンプする．
void
SnNor2::dump(ostream& s) const
{
  s << "NOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor3::SnNor3(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnOr3(id, inputs)
{
}

// @brief デストラクタ
SnNor3::~SnNor3()
{
}

// @brief ゲートタイプを返す．
GateType
SnNor3::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor3::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNor3::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 内容をダンプする．
void
SnNor3::dump(ostream& s) const
{
  s << "NOR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNor4::SnNor4(ymuint id,
	       const vector<SimNode*>& inputs) :
  SnOr4(id, inputs)
{
}

// @brief デストラクタ
SnNor4::~SnNor4()
{
}

// @brief ゲートタイプを返す．
GateType
SnNor4::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．(2値版)
PackedVal
SnNor4::_calc_gval2()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnNor4::_calc_fval2()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 内容をダンプする．
void
SnNor4::dump(ostream& s) const
{
  s << "NOR4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
