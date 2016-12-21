#ifndef FSIM_SIMNODE_H
#define FSIM_SIMNODE_H

/// @file SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

#include "fsim2_nsdef.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class SimNode
{
protected:

  /// @brief コンストラクタ
  SimNode();


public:

  /// @brief デストラクタ
  virtual
  ~SimNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを生成するクラスメソッド
  static
  SimNode*
  new_input();

  /// @brief 論理ノードを生成するクラスメソッド
  static
  SimNode*
  new_gate(GateType type,
	   const vector<PackedVal*>& ivals);


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

  /// @brief 出力値を計算する．
  void
  calc_val();


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値の計算を行う．(2値版)
  /// @return 計算結果を返す．
  virtual
  PackedVal
  _calc_val2() = 0;

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs2(ymuint ipos) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の値
  PackedVal mVal;

};


//////////////////////////////////////////////////////////////////////
// SimNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 出力値を得る．
inline
PackedVal
SimNode::val() const
{
  return mVal;
}

// @brief 出力値のセットを行う．
// @param[in] val 値
inline
void
SimNode::set_val(PackedVal val)
{
  mVal = val;
}

// @brief 出力値を計算する．
inline
void
SimNode::calc_val()
{
  PackedVal val = _calc_val();
  set_val(val);
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_SIMNODE_H
