#ifndef BT2SIMPLE_H
#define BT2SIMPLE_H

/// @file Bt2Simple.h
/// @brief Bt2Simple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Bt2Impl.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Bt2Simple Bt2Simple.h "Bt2Simple.h"
/// @brief 簡単な BackTracer
//////////////////////////////////////////////////////////////////////
class Bt2Simple :
  public Bt2Impl
{
public:

  /// @brief コンストラクタ
  Bt2Simple();


public:
  //////////////////////////////////////////////////////////////////////
  // Bt2Simple の仮想関数
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
  /// @param[in] output_list 故障に関係する出力ノードのリスト
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const vector<const TpgNode*>& output_list,
      const ValMap2& val_map,
      NodeVal2List& assign_list);


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
	    const ValMap2& val_map,
	    NodeVal2List& assign_list);

  /// @brief node のファンインのうち外部入力を記録する．
  /// @param[in] node ノード
  /// @param[in] val_map 値のマップ
  /// @param[out] assign_list 値割当の結果を入れるリスト
  void
  tfi_recur2(const TpgNode* node,
	     const ValMap2& val_map,
	     NodeVal2List& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxId;

  // tfi_recur で用いるマーク
  vector<bool> mMark;

  // tfi_recur2 で用いるマーク
  vector<bool> mMark2;

};

END_NAMESPACE_YM_SATPG

#endif // BT2SIMPLE_H
