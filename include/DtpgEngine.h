﻿#ifndef DTPGENGINE_H
#define DTPGENGINE_H

/// @file DtpgEngine.h
/// @brief DtpgEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "DtpgStats.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgEngine DtpgEngine.h "DtpgEngine.h"
/// @brief DTPG の基本エンジン
//////////////////////////////////////////////////////////////////////
class DtpgEngine
{
public:

  /// @brief デストラクタ
  virtual
  ~DtpgEngine() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列をセットする．
  virtual
  void
  set_option(const string& option_str) = 0;

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats() = 0;

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const = 0;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fault_list 対象の故障リスト
  /// @param[out] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      FaultMgr& fmgr,
      Fsim& fsim,
      const vector<const TpgFault*>& fault_list,
      DtpgStats& stats) = 0;

};


/// @brief Single エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief FFR エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatF(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief Hierachical エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatH(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief Single2 エンジンを作る．
/// @param[in] th_val しきい値
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS2(ymuint th_val,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief Multi エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief Multi2 エンジンを作る．
/// @param[in] th_val しきい値
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM2(ymuint th_val,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief SmtSingle エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SmtEngineSingle(const string& sat_type,
		    const string& sat_option,
		    ostream* sat_outp,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop);

END_NAMESPACE_YM_SATPG

#endif // DTPGENGINE_H
