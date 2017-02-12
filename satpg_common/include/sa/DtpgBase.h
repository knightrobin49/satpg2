#ifndef DTPGBASE_H
#define DTPGBASE_H

/// @file DtpgBase.h
/// @brief DtpgBase のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/Dtpg.h"
#include "TpgNode.h"
#include "sa/DtpgStats.h"
#include "sa/NodeValList.h"
#include "ym/ym_sat.h"
#include "ym/SatBool3.h"
#include "ym/SatLiteral.h"
#include "ym/SatSolver.h"
#include "ym/SatStats.h"
#include "ym/StopWatch.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgBase DtpgBase.h "DtpgBase.h"
/// @brief DtpgBase の実装用の基底クラス
//////////////////////////////////////////////////////////////////////
class DtpgBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] network 対象のネットワーク
  /// @param[in] root 故障伝搬の起点となるノード
  DtpgBase(const string& sat_type,
	   const string& sat_option,
	   ostream* sat_outp,
	   BackTracer& bt,
	   const TpgNetwork& network,
	   const TpgNode* root);

  /// @brief デストラクタ
  ~DtpgBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end(DtpgStats& stats);

  /// @brief 時間計測を開始する．
  void
  timer_start();

  /// @brief 時間計測を終了する．
  USTime
  timer_stop();

  /// @brief SATソルバを返す．
  SatSolver&
  solver();

  /// @brief 正常値の変数を返す．
  /// @param[in] node 対象のノード
  SatVarId
  gvar(const TpgNode* node);

  /// @brief 故障値の変数を返す．
  /// @param[in] node 対象のノード
  SatVarId
  fvar(const TpgNode* node);

  /// @brief 伝搬条件の変数を返す．
  /// @param[in] node 対象のノード
  SatVarId
  dvar(const TpgNode* node);

  /// @brief 正常値の変数を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] var 設定する変数
  void
  set_gvar(const TpgNode* node,
	   SatVarId var);

  /// @brief 故障値値の変数を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] var 設定する変数
  void
  set_fvar(const TpgNode* node,
	   SatVarId var);

  /// @brief 故障伝搬条件の変数を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] var 設定する変数
  void
  set_dvar(const TpgNode* node,
	   SatVarId var);

  /// @brief 起点となるノードを返す．
  const TpgNode*
  root_node() const;

  /// @brief root の影響が外部出力まで伝搬する条件のCNF式を作る．
  void
  gen_cnf_base();

  /// @brief 故障伝搬条件を表すCNF式を生成する．
  /// @param[in] node 対象のノード
  void
  make_dchain_cnf(const TpgNode* node);

  /// @brief 故障の影響がFFRの根のノードまで伝搬する条件を作る．
  /// @param[in] fault 対象の故障
  /// @param[out] assign_list 結果の値割り当てリスト
  void
  make_ffr_condition(const TpgFault* fault,
		     NodeValList& assign_list);

  /// @brief 一つの SAT問題を解く．
  /// @param[in] assumptions 値の決まっている変数のリスト
  /// @param[in] assign_list 仮定(値割り当て)のリスト
  /// @param[in] fault 対象の故障
  /// @param[out] nodeval_list 結果の値割り当てリスト
  /// @param[inout] stats DTPGの統計情報
  /// @return 結果を返す．
  SatBool3
  solve(const vector<SatLiteral>& assumptions,
	const NodeValList& assign_list,
	const TpgFault* fault,
	NodeValList& nodeval_list,
	DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief TFO マークを調べる．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(const TpgNode* node) const;

  /// @brief TFO マークをつける．
  /// @param[in] node 対象のノード
  ///
  /// と同時に mNodeList に入れる．<br>
  /// 出力ノードの場合は mOutputList にも入れる．<br>
  /// すでにマークされていたら何もしない．
  void
  set_tfo_mark(const TpgNode* node);

  /// @brief TFI マークを調べる．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(const TpgNode* node) const;

  /// @brief TFI マークをつける．
  /// @param[in] node 対象のノード
  ///
  /// と同時に mNodeList に入れる．
  void
  set_tfi_mark(const TpgNode* node);

  /// @brief TFO マークと TFI マークのいづれかがついていたら true を返す．
  /// @param[in] node 対象のノード
  bool
  mark(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver mSolver;

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 故障伝搬の起点となるノード
  const TpgNode* mRoot;

  // 関係するノードを入れておくリスト
  vector<const TpgNode*> mNodeList;

  // 関係する出力ノードを入れておくリスト
  vector<const TpgNode*> mOutputList;

  // 作業用のマークを入れておく配列
  // サイズは mMaxNodeId
  vector<ymuint8> mMarkArray;

  // 正常値を表す変数のマップ
  GenVidMap mGvarMap;

  // 故障値を表す変数のマップ
  GenVidMap mFvarMap;

  // 故障伝搬条件を表す変数のマップ
  GenVidMap mDvarMap;

  // バックトレーサー
  BackTracer& mBackTracer;

  // 時間計測を行なうかどうかの制御フラグ
  bool mTimerEnable;

  // 時間計測用のタイマー
  StopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバを返す．
inline
SatSolver&
DtpgBase::solver()
{
  return mSolver;
}

// @brief ノード番号の最大値を返す．
inline
ymuint
DtpgBase::max_node_id() const
{
  return mMaxNodeId;
}

// @brief 起点となるノードを返す．
inline
const TpgNode*
DtpgBase::root_node() const
{
  return mRoot;
}

// @brief 正常値の変数を返す．
// @param[in] node 対象のノード
inline
SatVarId
DtpgBase::gvar(const TpgNode* node)
{
  return mGvarMap(node);
}

// @brief 故障値の変数を返す．
// @param[in] node 対象のノード
inline
SatVarId
DtpgBase::fvar(const TpgNode* node)
{
  return mFvarMap(node);
}

// @brief 伝搬条件の変数を返す．
// @param[in] node 対象のノード
inline
SatVarId
DtpgBase::dvar(const TpgNode* node)
{
  return mDvarMap(node);
}

// @brief 正常値の変数を設定する．
// @param[in] node 対象のノード
// @param[in] var 設定する変数
inline
void
DtpgBase::set_gvar(const TpgNode* node,
		   SatVarId var)
{
  mGvarMap.set_vid(node, var);
}

// @brief 故障値値の変数を設定する．
// @param[in] node 対象のノード
// @param[in] var 設定する変数
inline
void
DtpgBase::set_fvar(const TpgNode* node,
		   SatVarId var)
{
  mFvarMap.set_vid(node, var);
}

// @brief 故障伝搬条件の変数を設定する．
// @param[in] node 対象のノード
// @param[in] var 設定する変数
inline
void
DtpgBase::set_dvar(const TpgNode* node,
		   SatVarId var)
{
  mDvarMap.set_vid(node, var);
}

// @brief TFO マークを調べる．
inline
bool
DtpgBase::tfo_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// @brief TFO マークをつける．
inline
void
DtpgBase::set_tfo_mark(const TpgNode* node)
{
  ymuint id = node->id();
  if ( ((mMarkArray[id] >> 0) & 1U) == 0U ) {
    mMarkArray[id] = 1U;
    mNodeList.push_back(node);
    if ( node->is_ppo() ) {
      mOutputList.push_back(node);
    }
  }
}

// @brief TFI マークを調べる．
inline
bool
DtpgBase::tfi_mark(const TpgNode* node) const
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief TFI マークをつける．
inline
void
DtpgBase::set_tfi_mark(const TpgNode* node)
{
  ymuint id = node->id();
  if ( mMarkArray[id] == 0U ) {
    mMarkArray[node->id()] = 2U;
    mNodeList.push_back(node);
  }
}

// @brief TFO マークと TFI マークのいづれかがついていたら true を返す．
inline
bool
DtpgBase::mark(const TpgNode* node)
{
  if ( mMarkArray[node->id()] ) {
    return true;
  }
  return false;
}

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGBASE_H
