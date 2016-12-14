
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

// @brief 正常値の計算を行う．(3値版)
void
SnOr::_calc_gval3()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->gval_0();
  PackedVal val1 = inode0->gval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 &= inode->gval_0();
    val1 |= inode->gval_1();
  }
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnOr::_calc_fval3(PackedVal mask)
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->fval_0();
  PackedVal val1 = inode0->fval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 &= inode->fval_0();
    val1 |= inode->fval_1();
  }
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr::_calc_lobs3(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->gval_0();
  }
  ymuint n = _fanin_num();
  for (ymuint i = ipos + 1; i < n; ++ i) {
    obs &= _fanin(i)->gval_0();
  }
  return obs;
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

// @brief 内容をダンプする．
void
SnOr2::dump(ostream& s) const
{
  s << "OR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
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
  ymuint alt_pos = ipos ^ 1;
  return ~_fanin(alt_pos)->gval();
}

// @brief 正常値の計算を行う．(3値版)
void
SnOr2::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnOr2::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr2::_calc_lobs3(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin(alt_pos)->gval_0();
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

// @brief 内容をダンプする．
void
SnOr3::dump(ostream& s) const
{
  s << "OR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
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

// @brief 正常値の計算を行う．(3値版)
void
SnOr3::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0() & inode2->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1() | inode2->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnOr3::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0() & inode2->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1() | inode2->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr3::_calc_lobs3(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval_0() & _fanin(2)->gval_0();
  case 1: return _fanin(0)->gval_0() & _fanin(2)->gval_0();
  case 2: return _fanin(0)->gval_0() & _fanin(1)->gval_0();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
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

// @brief 正常値の計算を行う．(3値版)
void
SnOr4::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0() & inode2->gval_0() & inode3->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1() | inode2->gval_1() | inode3->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnOr4::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0() & inode2->fval_0() & inode3->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1() | inode2->fval_1() | inode3->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnOr4::_calc_lobs3(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval_0() & _fanin(2)->gval_0() & _fanin(3)->gval_0();
  case 1: return _fanin(0)->gval_0() & _fanin(2)->gval_0() & _fanin(3)->gval_0();
  case 2: return _fanin(0)->gval_0() & _fanin(1)->gval_0() & _fanin(3)->gval_0();
  case 3: return _fanin(0)->gval_0() & _fanin(1)->gval_0() & _fanin(2)->gval_0();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
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

// @brief 正常値の計算を行う．(3値版)
void
SnNor::_calc_gval3()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->gval_0();
  PackedVal val1 = inode0->gval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 &= inode->gval_0();
    val1 |= inode->gval_1();
  }
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNor::_calc_fval3(PackedVal mask)
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->fval_0();
  PackedVal val1 = inode0->fval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 &= inode->fval_0();
    val1 |= inode->fval_1();
  }
  set_fval(val1, val0, mask);
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

// @brief 内容をダンプする．
void
SnNor2::dump(ostream& s) const
{
  s << "NOR2(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ")" << endl;
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

// @brief 正常値の計算を行う．(3値版)
void
SnNor2::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNor2::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1();
  set_fval(val1, val0, mask);
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

// @brief 内容をダンプする．
void
SnNor3::dump(ostream& s) const
{
  s << "NOR3(" << _fanin(0)->id();
  s << ", " << _fanin(1)->id();
  s << ", " << _fanin(2)->id();
  s << ")" << endl;
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

// @brief 正常値の計算を行う．(3値版)
void
SnNor3::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0() & inode2->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1() | inode2->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNor3::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0() & inode2->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1() | inode2->fval_1();
  set_fval(val1, val0, mask);
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

// @brief 正常値の計算を行う．(3値版)
void
SnNor4::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->gval_0() & inode1->gval_0() & inode2->gval_0() & inode3->gval_0();
  PackedVal val1 = inode0->gval_1() | inode1->gval_1() | inode2->gval_1() | inode3->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNor4::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->fval_0() & inode1->fval_0() & inode2->fval_0() & inode3->fval_0();
  PackedVal val1 = inode0->fval_1() | inode1->fval_1() | inode2->fval_1() | inode3->fval_1();
  set_fval(val1, val0, mask);
}

END_NAMESPACE_YM_SATPG_FSIM
