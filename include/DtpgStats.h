﻿#ifndef DTPGSTATS_H
#define DTPGSTATS_H

/// @file DtpgStats.h
/// @brief DtpgStats のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "ym/USTime.h"
#include "ym/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgStats DtpgStats.h "DtpgStats.h"
/// @brief DTPG の統計情報を表すクラス
//////////////////////////////////////////////////////////////////////
struct DtpgStats
{

  /// @brief DetStats を更新する
  void
  update_det(const SatStats& sat_stats,
	     const USTime& time);

  /// @brief RedStats を更新する
  void
  update_red(const SatStats& sat_stats,
	     const USTime& time);

  /// @brief PartRedStats を更新する
  void
  update_partred(const SatStats& sat_stats,
		 const USTime& time);

  /// @brief AbortStats を更新する
  void
  update_abort(const SatStats& sat_stats,
	       const USTime& time);

  /// @brief CNF 式を生成した回数
  ymuint32 mCnfGenCount;

  /// @brief CNF 式の生成に費やした時間
  USTime mCnfGenTime;

  /// @brief テスト生成に成功した回数．
  ymuint32 mDetCount;

  /// @brief テスト生成に成功した時の SAT に要した時間
  USTime mDetTime;

  /// @brief テスト生成に成功した時の SATソルバの統計情報の和
  SatStats mDetStats;

  /// @brief テスト生成に成功した時の SATソルバの統計情報の最大値
  ///
  /// 個々の値は同時に起こったわけではない．
  SatStats mDetStatsMax;

  /// @brief 冗長故障と判定した回数
  ymuint32 mRedCount;

  /// @brief 冗長故障と判定した時の SAT に要した時間
  USTime mRedTime;

  /// @brief 冗長故障と判定した時の SATソルバの統計情報の和
  SatStats mRedStats;

  /// @brief 冗長故障と判定した時の SATソルバの統計情報の最大値
  ///
  /// 個々の値は同時に起こったわけではない．
  SatStats mRedStatsMax;

  /// @brief 部分的な冗長故障と判定した回数
  ymuint32 mPartRedCount;

  /// @brief 部分的な冗長故障と判定した時の SAT に要した時間
  USTime mPartRedTime;

  /// @brief 部分的な冗長故障と判定した時の SATソルバの統計情報の和
  SatStats mPartRedStats;

  /// @brief 部分的な冗長故障と判定した時の SATソルバの統計情報の最大値
  ///
  /// 個々の値は同時に起こったわけではない．
  SatStats mPartRedStatsMax;

  /// @brief アボートした回数
  ymuint32 mAbortCount;

  /// @brief アボートした時の SAT に要した時間
  USTime mAbortTime;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief DetStats を更新する
inline
void
DtpgStats::update_det(const SatStats& sat_stats,
		      const USTime& time)
{
  ++ mDetCount;
  mDetTime += time;
  mDetStats += sat_stats;
  mDetStatsMax.max_assign(sat_stats);
}

// @brief RedStats を更新する
inline
void
DtpgStats::update_red(const SatStats& sat_stats,
		      const USTime& time)
{
  ++ mRedCount;
  mRedTime += time;
  mRedStats += sat_stats;
  mRedStatsMax.max_assign(sat_stats);
}

// @brief PartRedStats を更新する
inline
void
DtpgStats::update_partred(const SatStats& sat_stats,
			  const USTime& time)
{
  ++ mPartRedCount;
  mPartRedTime += time;
  mPartRedStats += sat_stats;
  mPartRedStatsMax.max_assign(sat_stats);
}

// @brief AbortStats を更新する
inline
void
DtpgStats::update_abort(const SatStats& sat_stats,
			const USTime& time)
{
  ++ mAbortCount;
  mAbortTime += time;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGSTATS_H
