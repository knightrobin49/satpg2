
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

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnXor::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin(i)->fval();
  }
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnXor::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
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

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnXor2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval();
  PackedVal val1 = inode1->fval();
  PackedVal val = val0 ^ val1;
  return val;
}

// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
PackedVal
SnXor2::_calc_gobs(ymuint ipos)
{
  return kPvAll1;
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

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnXnor::_calc_fval()
{
  ymuint n = _fanin_num();
  PackedVal val = _fanin(0)->fval();
  for (ymuint i = 1; i < n; ++ i) {
    val ^= _fanin(i)->fval();
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

// @brief 故障値の計算を行う．(2値版)
PackedVal
SnXnor2::_calc_fval()
{
  SimNode* inode0 = _fanin(0);
  SimNode* inode1 = _fanin(1);
  PackedVal val0 = inode0->fval();
  PackedVal val1 = inode1->fval();
  PackedVal val = val0 ^ val1;
  return ~val;
}

END_NAMESPACE_YM_SATPG_FSIM
