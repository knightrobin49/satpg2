
/// @file Bt2JustBase.cc
/// @brief Bt2JustBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2JustBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Bt2Just1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Bt2JustBase::Bt2JustBase()
{
}

// @brief デストラクタ
Bt2JustBase::~Bt2JustBase()
{
}

// @brief ノードID番号の最大値を設定する．
// @param[in] max_id ID番号の最大値
void
Bt2JustBase::set_max_id(ymuint max_id)
{
  mJustifiedMarkArray.clear();
  mJustifiedMarkArray.resize(max_id, false);
  mJustifiedNodeList.reserve(max_id);
  mJustified0MarkArray.clear();
  mJustified0MarkArray.resize(max_id, false);
  mJustified0NodeList.reserve(max_id);
}

// @brief justified マークを消す．
// @param[in] node 対象のノード
void
Bt2JustBase::clear_justified()
{
  for (vector<const TpgNode*>::iterator p = mJustifiedNodeList.begin();
       p != mJustifiedNodeList.end(); ++ p) {
    const TpgNode* node = *p;
    ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
    mJustifiedMarkArray[node->id()] = false;
    clear_justified_hook(node);
  }
  mJustifiedNodeList.clear();

  for (vector<const TpgNode*>::iterator p = mJustified0NodeList.begin();
       p != mJustified0NodeList.end(); ++ p) {
    const TpgNode* node = *p;
    ASSERT_COND( node->id() < mJustified0MarkArray.size() );
    mJustified0MarkArray[node->id()] = false;
    clear_justified_hook(node);
  }
  mJustified0NodeList.clear();
}

// @brief clear_justified() 中で呼ばれるフック関数
// @note デフォルトの実装はなにもしない．
void
Bt2JustBase::clear_justified_hook(const TpgNode* node)
{
}

END_NAMESPACE_YM_SATPG
