#ifndef TD_VALMAP_H
#define TD_VALMAP_H

/// @file td/ValMap.h
/// @brief ValMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "td/satpg_td.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_SATPG_TD

//////////////////////////////////////////////////////////////////////
/// @class ValMap ValMap.h "td/ValMap.h"
/// @brief ノードに関連付けられた値を保持するクラス
///
/// 下を見ればわかるけどインターフェイスを定義しただけの
/// 純粋仮想クラス
//////////////////////////////////////////////////////////////////////
class ValMap
{
public:

  /// @brief デストラクタ
  virtual
  ~ValMap() { }


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

END_NAMESPACE_YM_SATPG_TD

#endif // TD_VALMAP_H
