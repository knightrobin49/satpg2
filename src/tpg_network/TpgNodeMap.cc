
/// @file TpgNodeMap.cc
/// @brief TpgNodeMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNodeMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgNodeMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ノード番号の最大値 + 1
TpgNodeMap::TpgNodeMap(ymuint max_id) :
  mNodeArray(max_id, nullptr)
{
}

// @brief デストラクタ
TpgNodeMap::~TpgNodeMap()
{
}

// @brief 登録する．
// @param[in] bnnode_id BnNodeのID番号
// @param[in] tpgnode TpgNode
void
TpgNodeMap::reg(ymuint bnnode_id,
		TpgNode* tpgnode)
{
  ASSERT_COND( bnnode_id < mNodeArray.size() );
  mNodeArray[bnnode_id] = tpgnode;
}

// @brief 対応するノードを得る．
TpgNode*
TpgNodeMap::get(ymuint bnnode_id) const
{
  ASSERT_COND( bnnode_id < mNodeArray.size() );
  return mNodeArray[bnnode_id];
}

END_NAMESPACE_YM_SATPG
