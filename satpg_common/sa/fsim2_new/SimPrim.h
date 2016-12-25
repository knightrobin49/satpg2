#ifndef FSIM_SIMPRIM_H
#define FSIM_SIMPRIM_H

/// @file SimPrim.h
/// @brief SimPrim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

#include "fsim2_nsdef.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SimPrim SimPrim.h "SimPrim.h"
/// @brief 故障シミュレーション用のノード
///
/// 意味的には出力の値のみを持つ．<br>
/// ただし，ゲートタイプの場合，出力の値を計算するメソッドも持つ．<br>
/// そのため副次的にファンインのノードへのポインタも持つことになる．
//////////////////////////////////////////////////////////////////////
class SimPrim
{
protected:

  /// @brief コンストラクタ
  SimPrim();


public:

  /// @brief デストラクタ
  virtual
  ~SimPrim();


public:
  //////////////////////////////////////////////////////////////////////
  // 生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを生成するクラスメソッド
  static
  SimPrim*
  new_input();

  /// @brief 論理ノードを生成するクラスメソッド
  /// @param[in] type ゲートの型
  /// @param[in] fanins ファンインのノードの配列
  static
  SimPrim*
  new_gate(GateType type,
	   const vector<SimPrim*>& fanins);


public:
  //////////////////////////////////////////////////////////////////////
  // 2値の故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値を得る．
  PackedVal
  val() const;

  /// @brief 出力値のセットを行う．
  /// @param[in] val 値
  void
  set_val(PackedVal val);

  /// @brief 出力値を反転させる．
  /// @param[in] mask 反転マスク
  void
  flip_val(PackedVal mask);

  /// @brief 出力値を計算し設定する．
  void
  calc_val();

  /// @brief 出力値を計算し設定する．
  /// @param[in] mask ビットマスク
  /// @return 変更されたビットパタンを返す．
  PackedVal
  calc_val(PackedVal mask);


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値の計算を行う．(2値版)
  /// @return 計算結果を返す．
  virtual
  PackedVal
  _calc_val() = 0;

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs(ymuint ipos) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の値
  PackedVal mVal;

};


//////////////////////////////////////////////////////////////////////
// SimPrim のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 出力値を得る．
inline
PackedVal
SimPrim::val() const
{
  return mVal;
}

// @brief 出力値のセットを行う．
// @param[in] val 値
inline
void
SimPrim::set_val(PackedVal val)
{
  mVal = val;
}

// @brief 出力値を反転させる．
// @param[in] mask 反転マスク
inline
void
SimPrim::flip_val(PackedVal mask)
{
  mVal ^= mask;
}

// @brief 出力値を計算する．
inline
void
SimPrim::calc_val()
{
  PackedVal val = _calc_val();
  set_val(val);
}

// @brief 出力値を計算し設定する．
// @param[in] mask ビットマスク
// @return 変更されたビットパタンを返す．
inline
PackedVal
SimPrim::calc_val(PackedVal mask)
{
  PackedVal val = _calc_val();
  PackedVal dval = (val ^ mVal) & mask;
  mVal ^= dval;
  return dval;
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_SIMPRIM_H
