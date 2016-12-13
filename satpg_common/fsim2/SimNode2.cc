
/// @file SimNode2.cc
/// @brief SimNode2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SimNode2.h"
#include "SimFFR.h"
#include "Sn2Input.h"
#include "Sn2And.h"
#include "Sn2Or.h"
#include "Sn2Xor.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SimNode2::SimNode2(ymuint id) :
  mId(id),
  mFanoutNum(0),
  mFanouts(nullptr),
  mFanoutIpos(0),
  mFFR(nullptr),
  mLevel(0)
{
}

// デストラクタ
SimNode2::~SimNode2()
{
  delete [] mFanouts;
}

// @brief 入力ノードを生成するクラスメソッド
SimNode2*
SimNode2::new_input(ymuint id)
{
  return new SnInput(id);
}

// @brief ノードを生成するクラスメソッド
SimNode2*
SimNode2::new_node(ymuint id,
		   GateType type,
		   const vector<SimNode2*>& inputs)
{
  SimNode2* node = nullptr;
  ymuint ni = inputs.size();
  switch ( type ) {
  case kGateBUFF:
    ASSERT_COND(ni == 1 );
    node = new Sn2Buff(id, inputs);
    break;

  case kGateNOT:
    ASSERT_COND(ni == 1 );
    node = new Sn2Not(id, inputs);
    break;

  case kGateAND:
    switch ( ni ) {
    case 2:  node = new Sn2And2(id, inputs); break;
    case 3:  node = new Sn2And3(id, inputs); break;
    case 4:  node = new Sn2And4(id, inputs); break;
    default: node = new Sn2And(id, inputs);  break;
    }
    break;

  case kGateNAND:
    switch ( ni ) {
    case 2:  node = new Sn2Nand2(id, inputs); break;
    case 3:  node = new Sn2Nand3(id, inputs); break;
    case 4:  node = new Sn2Nand4(id, inputs); break;
    default: node = new Sn2Nand(id, inputs);  break;
    }
    break;

  case kGateOR:
    switch ( ni ) {
    case 2:  node = new Sn2Or2(id, inputs); break;
    case 3:  node = new Sn2Or3(id, inputs); break;
    case 4:  node = new Sn2Or4(id, inputs); break;
    default: node = new Sn2Or(id, inputs);  break;
    }
    break;

  case kGateNOR:
    switch ( ni ) {
    case 2:  node = new Sn2Nor2(id, inputs); break;
    case 3:  node = new Sn2Nor3(id, inputs); break;
    case 4:  node = new Sn2Nor4(id, inputs); break;
    default: node = new Sn2Nor(id, inputs);  break;
    }
    break;

  case kGateXOR:
    switch ( ni ) {
    case 2:  node = new Sn2Xor2(id, inputs); break;
    default: node = new Sn2Xor(id, inputs);  break;
    }
    break;

  case kGateXNOR:
    switch ( ni ) {
    case 2:  node = new Sn2Xnor2(id, inputs); break;
    default: node = new Sn2Xnor(id, inputs);  break;
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  return node;
}

// @brief FFR の根のノードの時 true を返す．
bool
SimNode2::is_ffr_root() const
{
  return mFFR->root() == this;
}

// @brief レベルを設定する．
void
SimNode2::set_level(ymuint level)
{
  mLevel = level;
}

// @brief ファンアウトリストを作成する．
void
SimNode2::set_fanout_list(const vector<SimNode2*>& fo_list,
			  ymuint ipos)
{
  mNfo = fo_list.size();
  mFanouts = new SimNode2*[mNfo];
  ymuint i = 0;
  for (vector<SimNode2*>::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p, ++ i) {
    mFanouts[i] = *p;
  }
  mFanoutIpos |= (ipos << 2);
}

// @brief ローカルな obs の計算を行う．
PackedVal
SimNode2::calc_lobs()
{
  if ( is_ffr_root() ) {
    return kPvAll1;
  }
  if ( !check_lobs() ) {
    SimNode2* onode = fanout(0);
    ymuint pos = fanout_ipos();
    mLobs = onode->calc_lobs() & onode->_calc_lobs(pos);
    set_lobs();
  }
  return mLobs;
}

END_NAMESPACE_YM_SATPG_FSIM
