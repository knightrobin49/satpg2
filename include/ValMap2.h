#ifndef VALMAP2_H
#define VALMAP2_H

/// @file ValMap.h
/// @brief ValMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class ValMap2 ValMap2.h "ValMap2.h"
/// @brief ノードに関連付けられた値を保持するクラス
///
/// 下を見ればわかるけどインターフェイスを定義しただけの
/// 純粋仮想クラス
//////////////////////////////////////////////////////////////////////
class ValMap2
{
public:

  /// @brief デストラクタ
  virtual
  ~ValMap2() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの1時刻前の正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  hval(const TpgNode* node) const = 0;

  /// @brief ノードの正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  gval(const TpgNode* node) const = 0;

  /// @brief ノードの故障値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  fval(const TpgNode* node) const = 0;

};

END_NAMESPACE_YM_SATPG

#endif // VALMAP2_H
