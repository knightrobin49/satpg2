#ifndef SPBUFF_H
#define SPBUFF_H

/// @file SpBuff.h
/// @brief SpBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SpGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SpBuff SimBuff.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class SpBuff :
  public SpGate1
{
public:

  /// @brief コンストラクタ
  SpBuff(const vector<SimPrim*>& fanins);

  /// @brief デストラクタ
  virtual
  ~SpBuff();


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


//////////////////////////////////////////////////////////////////////
/// @class SpNot SimPrim.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class SpNot :
  public SpBuff
{
public:

  /// @brief コンストラクタ
  SpNot(const vector<SimPrim*>& fanins);

  /// @brief デストラクタ
  virtual
  ~SpNot();


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_val();

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SPBUFF_H
