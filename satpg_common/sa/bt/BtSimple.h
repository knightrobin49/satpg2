#ifndef BTSIMPLE_H
#define BTSIMPLE_H

/// @file BtSimple.h
/// @brief BtSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtImpl.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class BtSimple BtSimple.h "BtSimple.h"
/// @brief 簡単な BackTracer
///
/// 構造上，関係のある外部入力ノードの値を全て記録する．
//////////////////////////////////////////////////////////////////////
class BtSimple :
  public BtImpl
{
public:

  /// @brief コンストラクタ
  BtSimple();


public:
  //////////////////////////////////////////////////////////////////////
  // BtSimple の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  ///
  /// このクラスの実装ではなにもしない．
  virtual
  void
  set_max_id(ymuint max_id);

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] assign_list 値の割り当てリスト
  /// @param[in] output_list 故障に関係する出力ノードのリスト
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] pi_assign_list 外部入力上の値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const NodeValList& assign_list,
      const vector<const TpgNode*>& output_list,
      const ValMap& val_map,
      NodeValList& pi_assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief node のファンインのうち外部入力を記録する．
  /// @param[in] node ノード
  /// @param[in] val_map 値のマップ
  /// @param[out] assign_list 値割当の結果を入れるリスト
  void
  tfi_recur(const TpgNode* node,
	    const ValMap& val_map,
	    NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxId;

  // tfi_recur で用いるマーク
  vector<bool> mMark;

};

END_NAMESPACE_YM_SATPG_SA

#endif // BTSIMPLE_H
