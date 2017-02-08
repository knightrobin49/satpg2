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
#include "ym/SatStats.h"
#include "ym/StopWatch.h"


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
  DtpgBase(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp,
	  BackTracer& bt);

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

  /// @brief SATソルバのタイプを得る．
  string
  sat_type() const;

  /// @brief SATソルバのオプションを得る．
  string
  sat_option() const;

  /// @brief SATソルバのログ出力を得る．
  ostream*
  sat_outp() const;

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end(DtpgStats& stats);

  /// @brief 最後に生成された値割当リストを得る．
  const NodeValList&
  last_assign();

  /// @brief 時間計測を開始する．
  void
  timer_start();

  /// @brief 時間計測を終了する．
  USTime
  timer_stop();

  /// @brief 一つの SAT問題を解く．
  /// @param[in] solver SATソルバ
  /// @param[in] assumption 仮定(値割り当て)のリスト
  /// @param[in] fault 対象の故障
  /// @param[in] root 故障位置のノード
  /// @param[in] output_list 故障に関係のある外部出力のリスト
  /// @param[in] gvar_map 正常値の変数番号マップ
  /// @param[in] fvar_map 故障値の変数番号マップ
  /// @param[out] nodeval_list 結果の値割り当てリスト
  /// @param[inout] stats DTPGの統計情報
  /// @return 結果を返す．
  SatBool3
  solve(SatSolver& solver,
	const vector<SatLiteral>& assumptions,
	const TpgFault* fault,
	const TpgNode* root,
	const vector<const TpgNode*>& output_list,
	const VidMap& gvar_map,
	const VidMap& fvar_map,
	NodeValList& nodeval_list,
	DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT solver のタイプ
  string mSatType;

  // SAT solver のオプション
  string mSatOption;

  // SAT solver の記録用ストリーム
  ostream* mSatOutP;

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

// @brief SATソルバのタイプを得る．
inline
string
DtpgBase::sat_type() const
{
  return mSatType;
}

// @brief SATソルバのオプションを得る．
inline
string
DtpgBase::sat_option() const
{
  return mSatOption;
}

// @brief SATソルバのログ出力を得る．
inline
ostream*
DtpgBase::sat_outp() const
{
  return mSatOutP;
}

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGBASE_H
