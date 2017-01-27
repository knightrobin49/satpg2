
/// @file EventQ.cc
/// @brief EventQ の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "EventQ.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のイベントキューを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EventQ::EventQ() :
  mArraySize(0),
  mArray(nullptr),
  mNum(0),
  mClearArraySize(0),
  mClearArray(nullptr),
  mClearPos(0)
{
}

// @brief デストラクタ
EventQ::~EventQ()
{
  delete [] mArray;
  delete [] mClearArray;
}

// @brief 初期化を行う．
// @param[in] max_level 最大レベル
// @param[in] node_num ノード数
void
EventQ::init(ymuint max_level,
	     ymuint node_num)
{
  if ( max_level >= mArraySize ) {
    delete [] mArray;
    mArraySize = max_level + 1;
    mArray = new SimNode*[mArraySize];
  }
  if ( node_num > mClearArraySize ) {
    delete [] mClearArray;
    mClearArraySize = node_num;
    mClearArray = new SimNode*[mClearArraySize];
  }

  mCurLevel = 0;
  for (ymuint i = 0; i < mArraySize; i ++) {
    mArray[i] = nullptr;
  }
  mNum = 0;
}

// @brief 初期イベントを追加する．
// @param[in] node 対象のノード
// @param[in] valmask 反転マスク
void
EventQ::put_trigger(SimNode* node,
		    PackedVal valmask)
{
  node->flip_fval(valmask);
  add_to_clear_list(node);
  put_fanouts(node);
}

// @brief イベントドリブンシミュレーションを行う．
// @param[in] target 目標のノード
// @retval 出力における変化ビットを返す．
//
// target が nullptr でない時にはイベントが target まで到達したら
// シミュレーションを終える．
// target が nullptr の時には出力ノードまでイベントを伝える．
PackedVal
EventQ::simulate(SimNode* target)
{
  // どこかの外部出力で検出されたことを表すビット
  PackedVal obs = kPvAll0;
  for ( ; ; ) {
    SimNode* node = get();
    // イベントが残っていなければ終わる．
    if ( node == nullptr ) break;

    // すでに検出済みのビットはマスクしておく
    // これは無駄なイベントの発生を抑える．
    PackedVal diff = node->calc_fval(~obs);
    if ( diff != kPvAll0 ) {
      add_to_clear_list(node);
      if ( node->is_output() || node == target ) {
	obs |= diff;
      }
      else {
	put_fanouts(node);
      }
    }
  }
  // 今の故障シミュレーションで値の変わったノードを元にもどしておく
  for (ymuint i = 0; i < mClearPos; ++ i) {
    SimNode* node = mClearArray[i];
    node->clear_fval();
  }
  mClearPos = 0;

  return obs;
}

END_NAMESPACE_YM_SATPG_FSIM
