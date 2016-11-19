#ifndef BT2IMPL_H
#define BT2IMPL_H

/// @file Bt2Impl.h
/// @brief Bt2Impl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer2.h"
#include "NodeVal2List.h"
#include "ValMap2.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Bt2Impl Bt2Impl.h "Bt2Impl.h"
/// @brief BackTracer の基本クラス
//////////////////////////////////////////////////////////////////////
class Bt2Impl
{
public:

  /// @brief コンストラクタ
  Bt2Impl();

  /// @brief デストラクタ
  virtual
  ~Bt2Impl();


public:
  //////////////////////////////////////////////////////////////////////
  // Bt2Impl の仮想関数
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
  /// @param[in] val_map ノードの値を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const vector<const TpgNode*>& output_list,
      const ValMap2& val_map,
      NodeVal2List& assign_list) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  static
  void
  record_value(const TpgNode* node,
	       const ValMap2& val_map,
	       NodeVal2List& assign_list);

  /// @brief 入力ノードの値を記録する(1時刻前)．
  /// @param[in] node 対象の外部入力ノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  static
  void
  record_value0(const TpgNode* node,
		const ValMap2& val_map,
		NodeVal2List& assign_list);

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @param[in] val_map ノードの値の割当を保持するクラス
// @param[out] assign_list 値の割当リスト
inline
void
Bt2Impl::record_value(const TpgNode* node,
		      const ValMap2& val_map,
		      NodeVal2List& assign_list)
{
  Val3 v = val_map.gval(node);
  if ( v == kVal0 ) {
    assign_list.add(node, 1, false);
  }
  else if ( v == kVal1 ) {
    assign_list.add(node, 1, true);
  }
}

// @brief 入力ノードの値を記録する(1時刻前)．
// @param[in] node 対象の外部入力ノード
// @param[in] val_map ノードの値の割当を保持するクラス
// @param[out] assign_list 値の割当リスト
inline
void
Bt2Impl::record_value0(const TpgNode* node,
		       const ValMap2& val_map,
		       NodeVal2List& assign_list)
{
  Val3 v = val_map.hval(node);
  if ( v == kVal0 ) {
    assign_list.add(node, 0, false);
  }
  else if ( v == kVal1 ) {
    assign_list.add(node, 0, true);
  }
}

END_NAMESPACE_YM_SATPG

#endif // BT2IMPL_H
