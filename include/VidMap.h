#ifndef VIDMAP_H
#define VIDMAP_H

/// @file VidMap.h
/// @brief VidMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "ym/SatVarId.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class VidMap VidMap.h "VidMap.h"
/// @brief ノードに関連した変数番号を返すクラス
//////////////////////////////////////////////////////////////////////
class VidMap
{
public:

  /// @brief デストラクタ
  virtual
  ~VidMap() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに関連した変数番号を返す．
  /// @param[in] node 対象のノード
  virtual
  SatVarId
  operator()(const TpgNode* node) const = 0;

};

END_NAMESPACE_YM_SATPG

#endif // VIDMAP_H
