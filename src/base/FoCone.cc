
/// @file FoCone.cc
/// @brief FoCone の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FoCone.h"

#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(const TpgNode* left,
	     const TpgNode* right)
  {
    return left->output_id2() < right->output_id2();
  }
};

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
// @param[in] fnode 故障位置のノード
FoCone::FoCone(ymuint max_node_id,
	       const TpgNode* fnode) :
  mMaxNodeId(max_node_id),
  mMarkArray(max_node_id, 0U)
{
  mNodeList.reserve(max_id());

  // 故障のあるノードの TFO を mNodeList に入れる．
  set_mark(fnode);
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* fonode = node->active_fanout(i);
      if ( !mark(fonode) ) {
	set_mark(fonode);
      }
    }
  }

  sort(mOutputList.begin(), mOutputList.end(), Lt());
}

// @brief デストラクタ
FoCone::~FoCone()
{
}

END_NAMESPACE_YM_SATPG
