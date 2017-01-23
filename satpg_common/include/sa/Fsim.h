#ifndef SA_FSIM_H
#define SA_FSIM_H

/// @file sa/Fsim.h
/// @brief Fsim のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/sa_nsdef.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class Fsim Fsim.h "sa/Fsim.h"
/// @brief 故障シミュレーションを行うクラスの基底クラス
///
/// 具体的には故障シミュレーションに特化した回路構造を表すクラスと
/// 故障シミュレーションに特化した故障の情報を表すクラスを持つ．
/// シミュレーションの際に検出された故障を以降のシミュレーションで
/// スキップするかどうかは外からコントロールされるべきなので，
/// このシミュレーションのみ有効な'スキップフラグ'というフラグを
/// 各故障に持たせる．スキップフラグは set_skip(f) で付加され，
/// clear_skip(f) で解除される．
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
  ///
  /// 全ての故障のスキップマークはクリアされる．
  virtual
  void
  set_network(const TpgNetwork& network) = 0;

  /// @brief 全ての故障にスキップマークをつける．
  virtual
  void
  set_skip_all() = 0;

  /// @brief 故障にスキップマークをつける．
  /// @param[in] f 対象の故障
  virtual
  void
  set_skip(const TpgFault* f) = 0;

  /// @brief 複数の故障にスキップマークをつける．
  /// @param[in] fault_list 故障のリスト
  ///
  /// fault_list に含まれない故障のスキップマークは消される．
  void
  set_skip(const vector<const TpgFault*>& fault_list);

  /// @brief 全ての故障のスキップマークを消す．
  virtual
  void
  clear_skip_all() = 0;

  /// @brief 故障のスキップマークを消す．
  /// @param[in] f 対象の故障
  virtual
  void
  clear_skip(const TpgFault* f) = 0;

  /// @brief 複数の故障のスキップマークを消す．
  /// @param[in] fault_list 故障のリスト
  ///
  /// fault_list に含まれない故障のスキップマークは付けられる．
  void
  clear_skip(const vector<const TpgFault*>& fault_list);

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(const TestVector* tv,
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
  /// @return 検出された故障数を返す．
  ///
  /// 検出された故障は det_fault() で取得する．
  virtual
  ymuint
  sppfp(const TestVector* tv) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @return 検出された故障数を返す．
  ///
  /// 検出された故障は det_fault() で取得する．
  virtual
  ymuint
  sppfp(const NodeValList& assign_list) = 0;

  /// @brief ppsfp 用のパタンバッファをクリアする．
  virtual
  void
  clear_patterns() = 0;

  /// @brief ppsfp 用のパタンを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < kPvBitLen )
  /// @param[in] tv テストベクタ
  virtual
  void
  set_pattern(ymuint pos,
	      const TestVector* tv) = 0;

  /// @brief 設定した ppsfp 用のパタンを読み出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < kPvBitLen )
  virtual
  const TestVector*
  get_pattern(ymuint pos) = 0;

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @return 検出された故障数を返す．
  ///
  /// 検出された故障は det_fault() で取得する．<br>
  /// 最低1つのパタンが set_pattern() で設定されている必要がある．<br>
  virtual
  ymuint
  ppsfp() = 0;

  /// @brief 直前の sppfp/ppsfp で検出された故障数を返す．
  virtual
  ymuint
  det_fault_num() = 0;

  /// @brief 直前の sppfp/ppsfp で検出された故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < det_fault_num() )
  virtual
  const TpgFault*
  det_fault(ymuint pos) = 0;

  /// @brief 直前の ppsfp で検出された故障の検出ビットパタンを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < det_fault_num() )
  virtual
  PackedVal
  det_fault_pat(ymuint pos) = 0;

};


//////////////////////////////////////////////////////////////////////
// Fsim の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

/// @brief 2値版の故障シミュレータ
extern
Fsim*
new_Fsim2();

/// @brief 2値版の故障シミュレータ
extern
Fsim*
new_Fsim2New();

/// @brief 3値版の故障シミュレータ
extern
Fsim*
new_Fsim3();

END_NAMESPACE_YM_SATPG_SA

#endif // SA_FSIM_H
