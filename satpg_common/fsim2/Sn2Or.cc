
/// @file Sn2Or.cc
/// @brief Sn2Or の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Sn2Or.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// Sn2Or
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Or::Sn2Or(ymuint id,
	     const vector<SimNode2*>& inputs) :
  Sn2Gate(id, inputs)
{
}

// @brief デストラクタ
Sn2Or::~Sn2Or()
{
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Or::_calc_gval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->gval();
  }
  return new_val;
}

// @brief ゲートタイプを返す．
GateType
Sn2Or::gate_type() const
{
  return kGateOR;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Or::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->fval();
  }
  return new_val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Or::_calc_lobs(ymuint ipos)
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
Sn2Or::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "OR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Or2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Or2::Sn2Or2(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Gate2(id, inputs)
{
}

// @brief デストラクタ
Sn2Or2::~Sn2Or2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Or2::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Or2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return pat0 | pat1;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Or2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return pat0 | pat1;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Or2::_calc_lobs(ymuint ipos)
{
  return ~mFanins[ipos ^ 1]->gval();
}

// @brief 内容をダンプする．
void
Sn2Or2::dump(ostream& s) const
{
  s << "OR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Or3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Or3::Sn2Or3(ymuint id,
	     const vector<SimNode2*>& inputs) :
  Sn2Gate3(id, inputs)
{
}

// @brief デストラクタ
Sn2Or3::~Sn2Or3()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Or3::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Or3::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return pat0 | pat1 | pat2;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Or3::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return pat0 | pat1 | pat2;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Or3::_calc_lobs(ymuint ipos)
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
Sn2Or3::dump(ostream& s) const
{
  s << "OR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Or4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Or4::Sn2Or4(ymuint id,
	     const vector<SimNode2*>& inputs) :
  Sn2Gate4(id, inputs)
{
}

// @brief デストラクタ
Sn2Or4::~Sn2Or4()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Or4::gate_type() const
{
  return kGateOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Or4::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Or4::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return pat0 | pat1 | pat2 | pat3;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
Sn2Or4::_calc_lobs(ymuint ipos)
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
Sn2Or4::dump(ostream& s) const
{
  s << "OR4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nor::Sn2Nor(ymuint id,
	     const vector<SimNode2*>& inputs) :
  Sn2Or(id, inputs)
{
}

// @brief デストラクタ
Sn2Nor::~Sn2Nor()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nor::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nor::_calc_gval()
{
  ymuint n = _fanin_num();
  PackedVal new_val = _fanin(0)->gval();
  for (ymuint i = 1; i < n; ++ i) {
    new_val |= _fanin(i)->gval();
  }
  return ~new_val;
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nor::_calc_fval()
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
Sn2Nor::dump(ostream& s) const
{
  ymuint n = _fanin_num();
  s << "NOR(" << _fanin(0)->id();
  for (ymuint i = 1; i < n; ++ i) {
    s << ", " << _fanin(i)->id();
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nor2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nor2::Sn2Nor2(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Or2(id, inputs)
{
}

// @brief デストラクタ
Sn2Nor2::~Sn2Nor2()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nor2::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nor2::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  return ~(pat0 | pat1);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nor2::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  return ~(pat0 | pat1);
}

// @brief 内容をダンプする．
void
Sn2Nor2::dump(ostream& s) const
{
  s << "NOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nor3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nor3::Sn2Nor3(ymuint id,
	       const vector<SimNode2*>& inputs) :
  Sn2Or3(id, inputs)
{
}

// @brief デストラクタ
Sn2Nor3::~Sn2Nor3()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nor3::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nor3::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nor3::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  return ~(pat0 | pat1 | pat2);
}

// @brief 内容をダンプする．
void
Sn2Nor3::dump(ostream& s) const
{
  s << "NOR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// Sn2Nor4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Sn2Nor4::Sn2Nor4(ymuint id,
		 const vector<SimNode2*>& inputs) :
  Sn2Or4(id, inputs)
{
}

// @brief デストラクタ
Sn2Nor4::~Sn2Nor4()
{
}

// @brief ゲートタイプを返す．
GateType
Sn2Nor4::gate_type() const
{
  return kGateNOR;
}

// @brief 正常値の計算を行う．
PackedVal
Sn2Nor4::_calc_gval()
{
  PackedVal pat0 = _fanin(0)->gval();
  PackedVal pat1 = _fanin(1)->gval();
  PackedVal pat2 = _fanin(2)->gval();
  PackedVal pat3 = _fanin(3)->gval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 故障値の計算を行う．
PackedVal
Sn2Nor4::_calc_fval()
{
  PackedVal pat0 = _fanin(0)->fval();
  PackedVal pat1 = _fanin(1)->fval();
  PackedVal pat2 = _fanin(2)->fval();
  PackedVal pat3 = _fanin(3)->fval();
  return ~(pat0 | pat1 | pat2 | pat3);
}

// @brief 内容をダンプする．
void
Sn2Nor4::dump(ostream& s) const
{
  s << "NOR4(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ", " << _fanin(3)->id();
  s << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIM
