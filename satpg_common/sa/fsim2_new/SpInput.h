#ifndef SPINPUT_H
#define SPINPUT_H

/// @file SpInput.h
/// @brief SpInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SimPrim.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SpInput SimPrim.h
/// @brief 入力ノード
//////////////////////////////////////////////////////////////////////
class SpInput :
  public SimPrim
{
public:

  /// @brief コンストラクタ
  SpInput();

  /// @brief デストラクタ
  virtual
  ~SpInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_val();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs(ymuint ipos);

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SPINPUT_H
