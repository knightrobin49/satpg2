
/// @file Bt2Simple.cc
/// @brief Bt2Simple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2Simple.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Bt2Simple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Bt2Simple::Bt2Simple()
{
}

// @brief ノードID番号の最大値を設定する．
// @param[in] max_id ID番号の最大値
//
// このクラスの実装ではなにもしない．
void
Bt2Simple::set_max_id(ymuint max_id)
{
  mMaxId = max_id;
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] output_list 故障に関係する出力ノードのリスト
// @param[in] val_map ノードの値の割当を保持するクラス
// @param[out] assign_list 値の割当リスト
void
Bt2Simple::run(const TpgNode* fnode,
	       const vector<const TpgNode*>& output_list,
	       const ValMap2& val_map,
	       NodeVal2List& assign_list)
{
  assign_list.clear();

  // output_list のファンインに含まれる入力ノードに印をつける．
  mMark.clear();
  mMark.resize(mMaxId, false);
  for (vector<const TpgNode*>::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const TpgNode* node = *p;
    tfi_recur(node, val_map, assign_list);
  }
}

// @brief node のファンインのうち外部入力を記録する．
// @param[in] node ノード
// @param[in] val_map 値のマップ
// @param[out] assign_list 値割当の結果を入れるリスト
void
Bt2Simple::tfi_recur(const TpgNode* node,
		     const ValMap2& val_map,
		     NodeVal2List& assign_list)
{
  if ( mMark[node->id()] ) {
    return;
  }
  mMark[node->id()] = true;

  if ( node->is_dff_output() ) {
    const TpgNode* alt_node = node->alt_output();
    tfi_recur2(alt_node, val_map, assign_list);
  }
  else if ( node->is_input() ) {
    record_value(node, val_map, assign_list);
  }
  else {
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      tfi_recur(inode, val_map, assign_list);
    }
  }
}

// @brief node のファンインのうち外部入力を記録する．
// @param[in] node ノード
// @param[in] val_map 値のマップ
// @param[out] assign_list 値割当の結果を入れるリスト
void
Bt2Simple::tfi_recur2(const TpgNode* node,
		      const ValMap2& val_map,
		      NodeVal2List& assign_list)
{
  if ( mMark2[node->id()] ) {
    return;
  }
  mMark2[node->id()] = true;

  if ( node->is_input() ) {
    record_value0(node, val_map, assign_list);
  }
  else {
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      tfi_recur2(inode, val_map, assign_list);
    }
  }
}

END_NAMESPACE_YM_SATPG
