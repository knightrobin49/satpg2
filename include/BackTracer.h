﻿#ifndef BACKTRACER_H
#define BACKTRACER_H

/// @file BackTracer.h
/// @brief BackTracer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

class BtImpl;

//////////////////////////////////////////////////////////////////////
/// @class BackTracer BackTracer.h "BackTracer.h"
/// @brief テストパタンを求めるためのバックトレースを行なうファンクター
//////////////////////////////////////////////////////////////////////
class BackTracer
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_id ID番号の最大値
  BackTracer(ymuint max_id);

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

END_NAMESPACE_YM_SATPG

#endif // BACKTRACER_H
