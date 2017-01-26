
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

// @brief 故障値の計算を行う．
PackedVal3
SnXor::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  PackedVal3 val = inode0->fval();
  ymuint n = _fanin_num();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val ^= inode->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．
PackedVal
SnXor::_calc_gobs(ymuint ipos)
{
  // 条件は ipos 以外が X でないこと
  PackedVal obs = kPvAll1;
  for (ymuint i = 0; i < ipos; ++ i) {
    const SimNode* inode = _fanin(i);
    PackedVal val0 = inode->gval().val0();
    PackedVal val1 = inode->gval().val1();
    obs &= (val0 | val1);
  }
  ymuint n = _fanin_num();
  for (ymuint i = ipos + 1; i < n; ++ i) {
    const SimNode* inode = _fanin(i);
    PackedVal val0 = inode->gval().val0();
    PackedVal val1 = inode->gval().val1();
    obs &= (val0 | val1);
  }
  return obs;
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

// @brief 故障値の計算を行う．
PackedVal3
SnXor2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val = val0 ^ val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnXor2::_calc_gobs(ymuint ipos)
{
  ymuint alt_pos = ipos ^ 1;
  SimNode* inode = _fanin(alt_pos);

  PackedVal val0 = inode->gval().val0();
  PackedVal val1 = inode->gval().val1();

  return val0 | val1;
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

// @brief 故障値の計算を行う．
PackedVal3
SnXnor::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  PackedVal3 val = inode0->fval();
  ymuint n = _fanin_num();
  for (ymuint i = 1; i < n; ++ i) {
    SimNode* inode = _fanin(i);
    val ^= inode->fval();
  }
  return ~val;
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

// @brief 故障値の計算を行う．
PackedVal3
SnXnor2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal3 val0 = inode0->fval();
  PackedVal3 val1 = inode1->fval();
  PackedVal3 val = val0 ^ val1;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
