#ifndef FSIM_H
#define FSIM_H

/// @file Fsim2.h
/// @brief Fsim2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる2値用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つのパタンを全ビットに展開して設定する．
  /// @param[in] tv 設定するテストベクタ
  void
  _set_sp2(TestVector* tv);

  /// @brief 一つのパタンを全ビットに展開して設定する．
  /// @param[in] assign_list 設定する値の割り当てリスト
  void
  _set_sp2(const NodeValList& assign_list);

  /// @brief 複数のパタンを設定する．
  /// @param[in] tv_array テストベクタの配列
  void
  _set_pp2(const vector<TestVector*>& tv_array);

  /// @brief SPSFP故障シミュレーションの本体
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  bool
  _spsfp2(const TpgFault* f);

  /// @brief SPPFP故障シミュレーションの本体
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト
  void
  _sppfp2(FsimOp& op);

  /// @brief 正常値の計算を行う．
  ///
  /// 入力ノードに gval の設定は済んでいるものとする．
  void
  _calc_gval2();


private:
  //////////////////////////////////////////////////////////////////////
  // SimNode / SimFault の設定に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
  void
  clear();

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

  // SimNode->id() をキーにして所属する FFR を納めた配列
  vector<SimFFR*> mFFRMap;

  // イベントキュー
  EventQ mEventQ;

  // 故障シミュレーション用の故障の配列
  vector<SimFault> mSimFaults;

  // TpgFault::id() をキーとして SimFault を格納する配列
  vector<SimFault*> mFaultArray;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_H
