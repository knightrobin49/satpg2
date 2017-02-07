
/// @file DtpgBase.cc
/// @brief DtpgBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgBase.h"

#include "TpgFault.h"

#include "sa/DtpgStats.h"
#include "sa/BackTracer.h"
#include "sa/NodeValList.h"
#include "sa/ModelValMap.h"

#include "ym/SatSolver.h"
#include "ym/SatStats.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_SA

// @brief コンストラクタ
DtpgBase::DtpgBase(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt) :
  mSatType(sat_type),
  mSatOption(sat_option),
  mSatOutP(sat_outp),
  mBackTracer(bt)
{
  mTimerEnable = false;
}

// @brief デストラクタ
DtpgBase::~DtpgBase()
{
}

// @breif 時間計測を制御する．
void
DtpgBase::timer_enable(bool enable)
{
  mTimerEnable = enable;
}

// @brief タイマーをスタートする．
void
DtpgBase::cnf_begin()
{
  timer_start();
}

// @brief タイマーを止めて CNF 作成時間に加える．
void
DtpgBase::cnf_end(DtpgStats& stats)
{
  USTime time = timer_stop();
  stats.mCnfGenTime += time;
  ++ stats.mCnfGenCount;
}

// @brief 時間計測を開始する．
void
DtpgBase::timer_start()
{
  if ( mTimerEnable ) {
    mTimer.reset();
    mTimer.start();
  }
}

/// @brief 時間計測を終了する．
USTime
DtpgBase::timer_stop()
{
  USTime time(0, 0, 0);
  if ( mTimerEnable ) {
    mTimer.stop();
    time = mTimer.time();
  }
  return time;
}

// @brief 一つの SAT問題を解く．
// @param[in] solver SATソルバ
// @param[in] assumption 仮定(値割り当て)のリスト
// @param[in] fault 対象の故障
// @param[in] root 故障位置のノード
// @param[in] output_list 故障に関係のある外部出力のリスト
// @param[in] gvar_map 正常値の変数番号マップ
// @param[in] fvar_map 故障値の変数番号マップ
// @param[out] nodeval_list 結果の値割り当てリスト
// @param[inout] stats DTPGの統計情報
// @return 結果を返す．
SatBool3
DtpgBase::solve(SatSolver& solver,
		const vector<SatLiteral>& assumptions,
		const TpgFault* fault,
		const TpgNode* root,
		const vector<const TpgNode*>& output_list,
		const VidMap& gvar_map,
		const VidMap& fvar_map,
		NodeValList& nodeval_list,
		DtpgStats& stats)
{
  StopWatch timer;

  SatStats prev_stats;
  solver.get_stats(prev_stats);

  timer.reset();
  timer.start();

  vector<SatBool3> model;
  SatBool3 ans = solver.solve(assumptions, model);

  timer.stop();
  USTime time = timer.time();

  SatStats sat_stats;
  solver.get_stats(sat_stats);
  //sat_stats -= prev_stats;

  if ( ans == kB3True ) {
    timer.reset();
    timer.start();

    // パタンが求まった．
    ModelValMap val_map(gvar_map, fvar_map, model);

    // バックトレースを行う．
    mBackTracer(root, output_list, val_map, nodeval_list);

    timer.stop();
    stats.mBackTraceTime += timer.time();

    stats.update_det(sat_stats, time);
  }
  else if ( ans == kB3False ) {
    // 検出不能と判定された．
    stats.update_red(sat_stats, time);
  }
  else {
    // ans == kB3X つまりアボート
    stats.update_abort(sat_stats, time);
  }

  return ans;
}

END_NAMESPACE_YM_SATPG_SA
