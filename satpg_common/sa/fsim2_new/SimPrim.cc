
/// @file SimPrim.cc
/// @brief SimPrim の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SimPrim.h"
#include "SpInput.h"
#include "SpBuff.h"
#include "SpAnd.h"
#include "SpOr.h"
#include "SpXor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimPrim::SimPrim()
{
}

// デストラクタ
SimPrim::~SimPrim()
{
}

// @brief 入力ノードを生成するクラスメソッド
SimPrim*
SimPrim::new_input()
{
  return new SpInput();
}

// @brief ゲートを生成するクラスメソッド
SimPrim*
SimPrim::new_gate(GateType type,
		  const vector<SimPrim*>& fanins)
{
  SimPrim* node = nullptr;
  ymuint ni = fanins.size();
  switch ( type ) {
  case kGateBUFF:
    ASSERT_COND(ni == 1 );
    node = new SpBuff(fanins);
    break;

  case kGateNOT:
    ASSERT_COND(ni == 1 );
    node = new SpNot(fanins);
    break;

  case kGateAND:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpAnd2(fanins); break;
    case 3:  node = new SpAnd3(fanins); break;
    case 4:  node = new SpAnd4(fanins); break;
    default: node = new SpAnd(fanins);  break;
    }
    break;

  case kGateNAND:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpNand2(fanins); break;
    case 3:  node = new SpNand3(fanins); break;
    case 4:  node = new SpNand4(fanins); break;
    default: node = new SpNand(fanins);  break;
    }
    break;

  case kGateOR:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpOr2(fanins); break;
    case 3:  node = new SpOr3(fanins); break;
    case 4:  node = new SpOr4(fanins); break;
    default: node = new SpOr(fanins);  break;
    }
    break;

  case kGateNOR:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpNor2(fanins); break;
    case 3:  node = new SpNor3(fanins); break;
    case 4:  node = new SpNor4(fanins); break;
    default: node = new SpNor(fanins);  break;
    }
    break;

  case kGateXOR:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpXor2(fanins); break;
    default: node = new SpXor(fanins);  break;
    }
    break;

  case kGateXNOR:
    switch ( ni ) {
    case 0:
    case 1: ASSERT_NOT_REACHED; break;
    case 2:  node = new SpXnor2(fanins); break;
    default: node = new SpXnor(fanins);  break;
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  return node;
}

END_NAMESPACE_YM_SATPG_FSIM
