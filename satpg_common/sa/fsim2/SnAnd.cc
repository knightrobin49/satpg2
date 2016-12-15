
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
SnAnd::_calc_gobs2(ymuint ipos)
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

// @brief 正常値の計算を行う．(3値版)
void
SnAnd::_calc_gval3()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->gval_0();
  PackedVal val1 = inode0->gval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 |= inode->gval_0();
    val1 &= inode->gval_1();
  }
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnAnd::_calc_fval3(PackedVal mask)
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->fval_0();
  PackedVal val1 = inode0->fval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 |= inode->fval_0();
    val1 &= inode->fval_1();
  }
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd::_calc_gobs3(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->gval_1();
  }
  ymuint n = _fanin_num();
  for (ymuint i = ipos + 1; i < n; ++ i) {
    obs &= _fanin(i)->gval_1();
  }
  return obs;
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
SnAnd2::_calc_gobs2(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin(alt_pos)->gval();
}

// @brief 正常値の計算を行う．(3値版)
void
SnAnd2::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnAnd2::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd2::_calc_gobs3(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin(alt_pos)->gval_1();
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
SnAnd3::_calc_gobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval() & _fanin(2)->gval();
  case 1: return _fanin(0)->gval() & _fanin(2)->gval();
  case 2: return _fanin(0)->gval() & _fanin(1)->gval();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
}

// @brief 正常値の計算を行う．(3値版)
void
SnAnd3::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0() | inode2->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1() & inode2->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnAnd3::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0() | inode2->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1() & inode2->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd3::_calc_gobs3(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval_1() & _fanin(2)->gval_1();
  case 1: return _fanin(0)->gval_1() & _fanin(2)->gval_1();
  case 2: return _fanin(0)->gval_1() & _fanin(1)->gval_1();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
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
SnAnd4::_calc_gobs2(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval() & _fanin(2)->gval() & _fanin(3)->gval();
  case 1: return _fanin(0)->gval() & _fanin(2)->gval() & _fanin(3)->gval();
  case 2: return _fanin(0)->gval() & _fanin(1)->gval() & _fanin(3)->gval();
  case 3: return _fanin(0)->gval() & _fanin(1)->gval() & _fanin(2)->gval();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
}

// @brief 正常値の計算を行う．(3値版)
void
SnAnd4::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0() | inode2->gval_0() | inode3->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1() & inode2->gval_1() & inode3->gval_1();
  set_gval(val0, val1);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnAnd4::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0() | inode2->fval_0() | inode3->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1() & inode2->fval_1() & inode3->fval_1();
  set_fval(val0, val1, mask);
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd4::_calc_gobs3(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval_1() & _fanin(2)->gval_1() & _fanin(3)->gval_1();
  case 1: return _fanin(0)->gval_1() & _fanin(2)->gval_1() & _fanin(3)->gval_1();
  case 2: return _fanin(0)->gval_1() & _fanin(1)->gval_1() & _fanin(3)->gval_1();
  case 3: return _fanin(0)->gval_1() & _fanin(1)->gval_1() & _fanin(2)->gval_1();
  default: ASSERT_NOT_REACHED; break;
  }
  return kPvAll0;
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

// @brief 正常値の計算を行う．(3値版)
void
SnNand::_calc_gval3()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->gval_0();
  PackedVal val1 = inode0->gval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 |= inode->gval_0();
    val1 &= inode->gval_1();
  }
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNand::_calc_fval3(PackedVal mask)
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal val0 = inode0->fval_0();
  PackedVal val1 = inode0->fval_1();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val0 |= inode->fval_0();
    val1 &= inode->fval_1();
  }
  set_fval(val1, val0, mask);
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

// @brief 正常値の計算を行う．(3値版)
void
SnNand2::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNand2::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1();
  set_fval(val1, val0, mask);
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

// @brief 正常値の計算を行う．(3値版)
void
SnNand3::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0() | inode2->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1() & inode2->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNand3::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0() | inode2->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1() & inode2->fval_1();
  set_fval(val1, val0, mask);
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

// @brief 正常値の計算を行う．(3値版)
void
SnNand4::_calc_gval3()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->gval_0() | inode1->gval_0() | inode2->gval_0() | inode3->gval_0();
  PackedVal val1 = inode0->gval_1() & inode1->gval_1() & inode2->gval_1() & inode3->gval_1();
  set_gval(val1, val0);
}

// @brief 故障値の計算を行う．(3値版)
// @param[in] mask マスク
//
// 結果は mFval0, mFval1 に格納される．
void
SnNand4::_calc_fval3(PackedVal mask)
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->fval_0() | inode1->fval_0() | inode2->fval_0() | inode3->fval_0();
  PackedVal val1 = inode0->fval_1() & inode1->fval_1() & inode2->fval_1() & inode3->fval_1();
  set_fval(val1, val0, mask);
}

END_NAMESPACE_YM_SATPG_FSIM
