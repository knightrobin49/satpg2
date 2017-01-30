
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

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnAnd::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin(0)->val();
  for (ymuint i = 1; i < n; ++ i) {
    val &= _fanin(i)->val();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd::_calc_gobs(ymuint ipos)
{
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    obs &= _fanin(i)->val();
  }
  for (ymuint i = ipos + 1; i < _fanin_num(); ++ i) {
    obs &= _fanin(i)->val();
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd2::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val = val0 & val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  return _fanin(alt_pos)->val();
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd3::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val2 = inode2->val();
  PackedVal val = val0 & val1 & val2;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd3::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->val() & _fanin(2)->val();
  case 1: return _fanin(0)->val() & _fanin(2)->val();
  case 2: return _fanin(0)->val() & _fanin(1)->val();
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnAnd4::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val2 = inode2->val();
  PackedVal val3 = inode3->val();
  PackedVal val = val0 & val1 & val2 & val3;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnAnd4::_calc_gobs(ymuint ipos)
{
  switch ( ipos ) {
  case 0: return _fanin(1)->val() & _fanin(2)->val() & _fanin(3)->val();
  case 1: return _fanin(0)->val() & _fanin(2)->val() & _fanin(3)->val();
  case 2: return _fanin(0)->val() & _fanin(1)->val() & _fanin(3)->val();
  case 3: return _fanin(0)->val() & _fanin(1)->val() & _fanin(2)->val();
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand::_calc_val()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin(0)->val();
  for (ymuint i = 1; i < n; ++ i) {
    val &= _fanin(i)->val();
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand2::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val = val0 & val1;
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand3::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val2 = inode2->val();
  PackedVal val = val0 & val1 & val2;
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

// @brief 出力値の計算を行う．(2値版)
PackedVal
SnNand4::_calc_val()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  SimNode* inode2 = _fanin(2);
  SimNode* inode3 = _fanin(3);
  PackedVal val0 = inode0->val();
  PackedVal val1 = inode1->val();
  PackedVal val2 = inode2->val();
  PackedVal val3 = inode3->val();
  PackedVal val = val0 & val1 & val2 & val3;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
