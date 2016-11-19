
/// @file BackTracer2.cc
/// @brief BackTracer2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer2.h"
#include "Bt2Simple.h"
#include "Bt2Just1.h"
#include "Bt2Just2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス BackTracer2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ID番号の最大値
BackTracer2::BackTracer2(ymuint xmode,
			 ymuint max_id)
{
  switch ( xmode ) {
  case 0: mImpl = new Bt2Simple(); break;
  case 1: mImpl = new Bt2Just1(); break;
  case 2: mImpl = new Bt2Just2(); break;
  default: mImpl = new Bt2Just2(); break;
  }
  mImpl->set_max_id(max_id);
}

// @brief デストラクタ
BackTracer2::~BackTracer2()
{
  delete mImpl;
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] output_list 故障に関係する出力ノードのリスト
// @param[in] val_map ノードの値を保持するクラス
// @param[out] assign_list 値の割当リスト
void
BackTracer2::operator()(const TpgNode* fnode,
			const vector<const TpgNode*>& output_list,
			const ValMap2& val_map,
			NodeVal2List& assign_list)
{
  mImpl->run(fnode, output_list, val_map, assign_list);
}

END_NAMESPACE_YM_SATPG
