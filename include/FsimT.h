#ifndef FSIMT_H
#define FSIMT_H

/// @file FsimT.h
/// @brief FsimT のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimT FsimT.h "FsimT.h"
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
class FsimT
{
public:

  virtual
  ~FsimT() { }


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
  spsfp(TestVector2* tv,
	const TpgFault* f) = 0;

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(const NodeVal2List& assign_list,
	const TpgFault* f) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(TestVector2* tv,
	FsimOp& op) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(const NodeVal2List& assign_list,
	FsimOp& op) = 0;

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  ppsfp(const vector<TestVector2*>& tv_array,
	FsimOp& op) = 0;

  /// @brief WSA を計算する．
  /// @param[in] tv テストベクタ
  virtual
  ymuint
  calc_wsa(TestVector2* tv) = 0;

};


//////////////////////////////////////////////////////////////////////
// FsimT の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

/// @brief 2値版の故障シミュレータ
extern
FsimT*
new_FsimT2();

/// @brief 3値版の故障シミュレータ
extern
FsimT*
new_FsimT3();

END_NAMESPACE_YM_SATPG

#endif // FSIMT_H
