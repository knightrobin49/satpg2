
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
  mNum(0)
{
  // 適当なサイズで初期化しておく
  init(128);
}

// @brief デストラクタ
EventQ::~EventQ()
{
  delete [] mArray;
}

// @brief 初期化を行う．
// @param[in] max_level 最大レベル
void
EventQ::init(ymuint max_level)
{
  if ( max_level >= mArraySize ) {
    delete [] mArray;
    mArraySize = max_level + 1;
    mArray = new SimNode*[mArraySize];
  }
  mCurLevel = 0;
  for (ymuint i = 0; i < mArraySize; i ++) {
    mArray[i] = nullptr;
  }
  mNum = 0;
}

// @brief ファンアウトのノードをキューに積む．
// @param[in] node 対象のノード
void
EventQ::put_fanouts(SimNode* node)
{
  ymuint no = node->fanout_num();
  if ( no > 0 ) {
    put(node->fanout_top());
    -- no;
    for (ymuint i = 0; i < no; ++ i) {
      put(node->fanout(i));
    }
  }
}

END_NAMESPACE_YM_SATPG_FSIM
