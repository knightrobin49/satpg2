
/// @file FvalNode.cc
/// @brief FvalNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "FvalNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
// 故障シミュレーション用のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
FvalNode::FvalNode() :
  mFanoutNum(0),
  mFanouts(nullptr),
  mLevel(0)
{
}

// デストラクタ
FvalNode::~FvalNode()
{
  delete [] mFanouts;
}

// @brief レベルを設定する．
void
FvalNode::set_level(ymuint level)
{
  mLevel = level;
}

// @brief ファンアウトリストを作成する．
void
FvalNode::set_fanout_list(const vector<FvalNode*>& fo_list,
			  ymuint ipos)
{
  ymuint nfo = fo_list.size();
  mFanouts = new FvalNode*[nfo];
  for (ymuint i = 0; i < nfo; ++ i) {
    mFanouts[i] = fo_list[i];
  }

  mFanoutNum |= (nfo << 16) | (ipos << 4);
}

END_NAMESPACE_YM_SATPG_FSIM
