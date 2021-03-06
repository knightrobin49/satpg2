﻿#ifndef FSIM3_H
#define FSIM3_H

/// @file Fsim3.h
/// @brief Fsim3 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "fsim3_nsdef.h"
#include "Fsim.h"
#include "PackedVal.h"
#include "EventQ.h"
#include "SimFault.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class Fsim3 Fsim3.h "Fsim3.h"
/// @brief 故障シミュレーションを行うモジュール
/// @sa ModBase
//////////////////////////////////////////////////////////////////////
class Fsim3 :
  public Fsim
{
public:

  /// @brief コンストラクタ
  Fsim3();

  /// @brief デストラクタ
  virtual
  ~Fsim3();


public:
  //////////////////////////////////////////////////////////////////////
  // Fsim の仮想関数
  /////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] network ネットワーク
  virtual
  void
  set_network(const TpgNetwork& network);

  /// @brief 故障にスキップマークをつける．
  virtual
  void
  set_skip(const TpgFault* f);

  /// @brief 故障リストを設定する．
  /// @param[in] fault_list 対象の故障リスト
  ///
  /// スキップマークは消される．
  virtual
  void
  set_faults(const vector<const TpgFault*>& fault_list);

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(TestVector* tv,
	const TpgFault* f);

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(const NodeValList& assign_list,
	const TpgFault* f);

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(TestVector* tv,
	FsimOp& op);

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  virtual
  void
  sppfp(const NodeValList& assign_list,
	FsimOp& op);

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	FsimOp& op);

  /// @brief WSA を計算する．
  /// @param[in] tv テストベクタ
  virtual
  ymuint
  calc_wsa(TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SPSFP故障シミュレーションの本体
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  bool
  _spsfp(const TpgFault* f);

  /// @brief SPPFP故障シミュレーションの本体
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  void
  _sppfp(FsimOp& op);

  /// @brief FFR 内の故障シミュレーションを行う．
  /// @param[in] ffr 対象のFFR
  ///
  /// ffr 内の対象故障に対して故障が伝搬するかを調べる．
  /// 結果は各故障の mObsMask に設定される．
  /// また，すべての mObsMask の bitwise-OR を返す．
  ///
  /// 故障は SimFFR::fault_list() に格納されているが，
  /// スキップフラグが立った故障はリストから取り除かれる．
  PackedVal
  ffr_simulate(SimFFR* ffr);

  /// @brief 正常値の計算を行う．
  /// @note 値の変わったノードは mGvalClearArray に積まれる．
  void
  calc_gval();

  /// @brief 正常値が更新されたときの処理を行なう．
  void
  update_gval(SimNode* node);

  /// @brief 正常値をクリアする．
  /// @note mGvalClearArray を使う．
  void
  clear_gval();

  /// @brief 故障値の計算を行う．
  /// @note この関数を抜けた時点で故障値はクリアされている．
  PackedVal
  calc_fval();

  /// @brief 故障値が更新されたときの処理を行なう．
  void
  update_fval(SimNode* node);

  /// @brief ffr 内の故障が検出可能か調べる．
  /// @param[in] ffr 対象の FFR
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  ///
  /// ここでは各FFR の fault_list() は変化しない．
  void
  fault_sweep(SimFFR* ffr,
	      FsimOp& op);


private:
  //////////////////////////////////////////////////////////////////////
  // SimNode/SimFault 関係の設定関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  void
  clear();

  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TpgNode* node) const;

  /// @brief 外部入力ノードを作る．
  SimNode*
  make_input();

  /// @brief logic ノードを作る．
  SimNode*
  make_node(GateType type,
	    const vector<SimNode*>& inputs);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TpgNetwork* mNetwork;

  // TpgNode の id をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;

  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 入力からのトポロジカル順に並べた logic ノードの配列
  vector<SimNode*> mLogicArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // イベントキュー
  EventQ mEventQ;

  // 正常値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mGvalClearArray;

  // 故障値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mFvalClearArray;

  // 故障シミュレーション用の故障の配列
  vector<SimFault> mSimFaults;

  // TpgFault::id() をキーにして SimFault を格納する配列
  vector<SimFault*> mFaultArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 正常値が更新されたときの処理を行なう．
inline
void
Fsim3::update_gval(SimNode* node)
{
  mGvalClearArray.push_back(node);
  ymuint no = node->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(node->fanout(i));
  }
}

// @brief 故障値が更新されたときの処理を行なう．
void
Fsim3::update_fval(SimNode* node)
{
  mFvalClearArray.push_back(node);
  ymuint no = node->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(node->fanout(i));
  }
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM3_H
