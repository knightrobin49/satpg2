#ifndef SNBUFF_H
#define SNBUFF_H

/// @file SnBuff.h
/// @brief SnBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnBuff SimBuff.h
/// @brief BUFFノード
//////////////////////////////////////////////////////////////////////
class SnBuff :
  public SnGate1
{
public:

  /// @brief コンストラクタ
  SnBuff(ymuint id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnBuff();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs2(ymuint ipos);


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_gobs3(ymuint ipos);

};


//////////////////////////////////////////////////////////////////////
/// @class SnNot SimNode.h
/// @brief NOTノード
//////////////////////////////////////////////////////////////////////
class SnNot :
  public SnBuff
{
public:

  /// @brief コンストラクタ
  SnNot(ymuint id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNot();


public:

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_fval2();


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNBUFF_H
