#ifndef FSIM2_H
#define FSIM2_H

/// @file Fsim2.h
/// @brief Fsim2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sa/Fsim.h"

#include "fsim2_nsdef.h"
#include "PackedVal.h"
#include "EventQ.h"
#include "SimFault.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class Fsim2 Fsim2.h "Fsim2.h"
/// @brief 故障シミュレーションを行うモジュール
//////////////////////////////////////////////////////////////////////
class Fsim2 :
  public Fsim
{
public:

  /// @brief コンストラクタ
  Fsim2();

  /// @brief デストラクタ
  virtual
  ~Fsim2();


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
  set_network(const TpgNetwork& network);

  /// @brief 全ての故障にスキップマークをつける．
  virtual
  void
  set_skip_all();

  /// @brief 故障にスキップマークをつける．
  virtual
  void
  set_skip(const TpgFault* f);

  /// @brief 全ての故障のスキップマークを消す．
  virtual
  void
  clear_skip_all();

  /// @brief 故障のスキップマークを消す．
  /// @param[in] f 対象の故障
  virtual
  void
  clear_skip(const TpgFault* f);


public:
  //////////////////////////////////////////////////////////////////////
  // 2値の故障シミュレーションを行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(const TestVector* tv,
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
  /// @param[out] fault_list 検出された故障のリスト
  virtual
  void
  sppfp(const TestVector* tv,
	vector<const TpgFault*>& fault_list);

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] assign_list 値の割当リスト
  /// @param[out] fault_list 検出された故障のリスト
  virtual
  void
  sppfp(const NodeValList& assign_list,
	vector<const TpgFault*>& fault_list);

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] num テストベクタの数
  /// @param[in] tv_array テストベクタの配列
  /// @param[out] fault_list 検出された故障とその時のビットパタンのリスト
  ///
  /// num は高々 kBvBitLen 以下<br>
  virtual
  void
  ppsfp(ymuint num,
	const TestVector* tv_array[],
	vector<pair<const TpgFault*, PackedVal> >& fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる2値用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つのパタンを全ビットに展開して設定する．
  /// @param[in] tv 設定するテストベクタ
  void
  _set_sp2(const TestVector* tv);

  /// @brief 一つのパタンを全ビットに展開して設定する．
  /// @param[in] assign_list 設定する値の割り当てリスト
  void
  _set_sp2(const NodeValList& assign_list);

  /// @brief 複数のパタンを設定する．
  /// @param[in] num テストベクタの数
  /// @param[in] tv_array テストベクタの配列
  void
  _set_pp2(ymuint num,
	   const TestVector* tv_array[]);

  /// @brief SPSFP故障シミュレーションの本体
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  bool
  _spsfp2(const TpgFault* f);

  /// @brief SPPFP故障シミュレーションの本体
  /// @param[out] fault_list 検出された故障のリスト
  void
  _sppfp2(vector<const TpgFault*>& fault_list);

  /// @brief 正常値の計算を行う．
  ///
  /// 入力ノードに gval の設定は済んでいるものとする．
  void
  _calc_gval2();

  /// @brief イベントキューにイベントを追加する．
  /// @param[in] node イベントの起こったノード
  /// @param[in] mask 反転マスク
  void
  eventq_put2(SimNode* node,
	      PackedVal mask);

  /// @brief イベントキューを用いてシミュレーションを行う．
  PackedVal
  eventq_simulate2();


private:
  //////////////////////////////////////////////////////////////////////
  // SimNode / SimFault の設定に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
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
  make_gate(GateType type,
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
  // サイズは mNetwork->pseudo_input_num()
  vector<SimNode*> mInputArray;

  // 正常値計算用の配列
  vector<SimPrim*> mGvalArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // SimNode->id() をキーにして所属する FFR を納めた配列
  vector<SimFFR*> mFFRMap;

  // イベントキュー
  EventQ mEventQ;

  // 値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mClearArray;

  // 故障シミュレーション用の故障の配列
  vector<SimFault> mSimFaults;

  // TpgFault::id() をキーとして SimFault を格納する配列
  vector<SimFault*> mFaultArray;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM2_H
