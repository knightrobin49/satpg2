
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
FoCone::FoCone(ymuint max_node_id) :
  mMaxNodeId(max_node_id),
  mMarkArray(max_node_id, 0U)
{
}

// @brief デストラクタ
FoCone::~FoCone()
{
}

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] fnode_list 故障位置のノードのリスト
//
// 結果は mNodeList に格納される．
// 故障位置の TFO が mNodeList の [0: mTfoEnd1 - 1] に格納される．
void
FoCone::mark_region(const vector<const TpgNode*>& fnode_list)
{
  mNodeList.clear();
  mNodeList.reserve(max_id());

  mOutputList.clear();

  mDomNode = nullptr;

  // 故障のあるノードの TFO を mNodeList に入れる．
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgNode* fnode = fnode_list[i];
    if ( !mark(fnode) ) {
      set_mark(fnode);
    }
  }

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

// @brief 故障位置を与えてその TFO の TFI リストを作る．
// @param[in] fnode 故障位置のノード
// @param[in] dom_node dominator ノード
//
// 結果は mNodeList に格納される．
// こちらは fnode の直近の dominator までを対象とする．
void
FoCone::mark_region2(const TpgNode* fnode,
		     const TpgNode* dom_node)
{
  mNodeList.clear();
  mNodeList.reserve(max_id());

  mOutputList.clear();

  mDomNode = dom_node;

  // 故障のあるノードの TFO を mNodeList に入れる．
  if ( !mark(fnode) ) {
    set_mark(fnode);
  }
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    if ( node == dom_node ) {
      continue;
    }
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* fonode = node->active_fanout(i);
      if ( !mark(fonode) ) {
	set_mark(fonode);
      }
    }
  }
}

END_NAMESPACE_YM_SATPG
