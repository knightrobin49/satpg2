
/// @file TpgDffControl.cc
/// @brief TpgDffControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgDffControl.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgDffControl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] dff 接続しているDFF
// @param[in] fanin ファンインのノード
TpgDffControl::TpgDffControl(ymuint id,
			     TpgDff* dff,
			     TpgNode* fanin) :
  TpgNode(id, vector<TpgNode*>(1, fanin), 0),
  mDff(dff)
{
}

// @brief デストラクタ
TpgDffControl::~TpgDffControl()
{
}

// @brief 接続している DFF を返す．
//
// is_dff_input() | is_dff_output() | is_dff_clock() | is_dff_clear() | is_dff_preset()
// の時に意味を持つ．
TpgDff*
TpgDffControl::dff() const
{
  return mDff;
}

END_NAMESPACE_YM_SATPG
