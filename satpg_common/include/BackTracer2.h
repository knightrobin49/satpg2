#ifndef BACKTRACER2_H
#define BACKTRACER2_H

/// @file BackTracer2.h
/// @brief BackTracer2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

class Bt2Impl;

//////////////////////////////////////////////////////////////////////
/// @class BackTracer2 BackTracer2.h "BackTracer2.h"
/// @brief テストパタンを求めるためのバックトレースを行なうファンクター
//////////////////////////////////////////////////////////////////////
class BackTracer2
{
public:

  /// @brief コンストラクタ
  /// @param[in] xmode モード
  /// @param[in] max_id ID番号の最大値
  BackTracer2(ymuint xmode,
	      ymuint max_id);

  /// @brief デストラクタ
  ~BackTracer2();


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
	     const ValMap2& val_map,
	     NodeVal2List& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  Bt2Impl* mImpl;

};

END_NAMESPACE_YM_SATPG

#endif // BACKTRACER2_H
