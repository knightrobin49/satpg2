﻿#ifndef FSIM_H
#define FSIM_H

/// @file Fsim.h
/// @brief Fsim のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Fsim Fsim.h "Fsim.h"
/// @brief 故障シミュレーションを行うクラスの基底クラス
///
/// 具体的には故障シミュレーションに特化した回路構造を表すクラスと
/// 故障シミュレーションに特化した故障の情報を表すクラスを持つ．
/// シミュレーションの際に検出された故障を以降のシミュレーションで
/// スキップするかどうかは外からコントロールされるべきなので，
/// このシミュレーションのみ有効な'スキップフラグ'というフラグを
/// 各故障に持たせる．スキップフラグは set_skip(f) で付加され，
/// clear_skip() で解除される．
//////////////////////////////////////////////////////////////////////
class Fsim
{
public:

  virtual
  ~Fsim() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] network ネットワーク
  virtual
  void
  set_network(const TpgNetwork& network) = 0;

  /// @brief 故障にスキップマークをつける．
  virtual
  void
  set_skip(const TpgFault* f) = 0;

  /// @brief 故障リストを設定する．
  /// @param[in] fault_list 対象の故障リスト
  ///
  /// スキップマークは消される．
  virtual
  void
  set_faults(const vector<const TpgFault*>& fault_list) = 0;

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(TestVector* tv,
	const TpgFault* f) = 0;

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(const NodeValList& assign_list,
	const TpgFault* f) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(TestVector* tv,
	FsimOp& op) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(const NodeValList& assign_list,
	FsimOp& op) = 0;

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	FsimOp& op) = 0;

  /// @brief WSA を計算する．
  /// @param[in] tv テストベクタ
  virtual
  ymuint
  calc_wsa(TestVector* tv) = 0;

};


//////////////////////////////////////////////////////////////////////
// Fsim の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

/// @brief 2値版の故障シミュレータ
extern
Fsim*
new_Fsim2();

/// @brief 3値版の故障シミュレータ
extern
Fsim*
new_Fsim3();

/// @brief 遷移故障用の2値版の故障シミュレータ
extern
Fsim*
new_TFsim2();

END_NAMESPACE_YM_SATPG

#endif // FSIM_H
