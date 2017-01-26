
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd::_calc_fval()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal3 val = inode0->fval();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val &= inode->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd::_calc_gobs(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->gval().val1();
  }
  ymuint n = _fanin_num();
  for (ymuint i = ipos + 1; i < n; ++ i) {
    obs &= _fanin(i)->gval().val1();
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val = val0 & val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin(alt_pos)->gval().val1();
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd3::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val2 = inode2->fval();
  PackedVal3 val = val0 & val1 & val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd3::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval().val1() & _fanin(2)->gval().val1();
  case 1: return _fanin(0)->gval().val1() & _fanin(2)->gval().val1();
  case 2: return _fanin(0)->gval().val1() & _fanin(1)->gval().val1();
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnAnd4::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val2 = inode2->fval();
  PackedVal3 val3 = inode3->fval();
  PackedVal3 val = val0 & val1 & val2 & val3;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnAnd4::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->gval().val1() & _fanin(2)->gval().val1() & _fanin(3)->gval().val1();
  case 1: return _fanin(0)->gval().val1() & _fanin(2)->gval().val1() & _fanin(3)->gval().val1();
  case 2: return _fanin(0)->gval().val1() & _fanin(1)->gval().val1() & _fanin(3)->gval().val1();
  case 3: return _fanin(0)->gval().val1() & _fanin(1)->gval().val1() & _fanin(2)->gval().val1();
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

// @brief 故障値の計算を行う．
PackedVal3
SnNand::_calc_fval()
{
  ymuint n = _fanin_num();
  SimNode* inode0 = _fanin(0);
  PackedVal3 val = inode0->fval();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val &= inode->fval();
  }
  return ~val;
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val = val0 & val1;
  return ~val;
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand3::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val2 = inode2->fval();
  PackedVal3 val = val0 & val1 & val2;
  return ~val;
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

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNand4::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val2 = inode2->fval();
  PackedVal3 val3 = inode3->fval();
  PackedVal3 val = val0 & val1 & val2 & val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
