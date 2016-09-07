
/// @file BackTracer.cc
/// @brief BackTracer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer.h"
#include "BtSimple.h"
#include "BtJust1.h"
#include "BtJust2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BackTracer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ID番号の最大値
BackTracer::BackTracer(ymuint xmode,
		       ymuint max_id)
{
  switch ( xmode ) {
  case 0: mImpl = new BtSimple(); break;
  case 1: mImpl = new BtJust1(); break;
  case 2: mImpl = new BtJust2(); break;
  default: mImpl = new BtJust2(); break;
  }
  mImpl->set_max_id(max_id);
}

// @brief デストラクタ
BackTracer::~BackTracer()
{
  delete mImpl;
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] output_list 故障に関係する出力ノードのリスト
// @param[in] val_map ノードの値を保持するクラス
// @param[out] assign_list 値の割当リスト
void
BackTracer::operator()(const TpgNode* fnode,
		       const vector<const TpgNode*>& output_list,
		       const ValMap& val_map,
		       NodeValList& assign_list)
{
  mImpl->run(fnode, output_list, val_map, assign_list);
}

END_NAMESPACE_YM_SATPG
