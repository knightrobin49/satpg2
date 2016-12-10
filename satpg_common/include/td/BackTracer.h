#ifndef TD_BACKTRACER_H
#define TD_BACKTRACER_H

/// @file td/BackTracer.h
/// @brief BackTracer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "td/td_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_TD

class BtImpl;

//////////////////////////////////////////////////////////////////////
/// @class BackTracer BackTracer.h "td/BackTracer.h"
/// @brief テストパタンを求めるためのバックトレースを行なうファンクター
//////////////////////////////////////////////////////////////////////
class BackTracer
{
public:

  /// @brief コンストラクタ
  /// @param[in] xmode モード
  /// @param[in] max_id ID番号の最大値
  BackTracer(ymuint xmode,
	     ymuint max_id);

  /// @brief デストラクタ
  ~BackTracer();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] output_list 故障に関係する出力ノードのリスト
  /// @param[in] val_map ノードの値を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  void
  operator()(const TpgNode* fnode,
	     const vector<const TpgNode*>& output_list,
	     const ValMap& val_map,
	     NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  BtImpl* mImpl;

};

END_NAMESPACE_YM_SATPG_TD

#endif // TD_BACKTRACER_H
