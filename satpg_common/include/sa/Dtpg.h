#ifndef SA_DTPG_H
#define SA_DTPG_H

/// @file sa/Dtpg.h
/// @brief Dtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sa/sa_nsdef.h"
#include "sa/DtpgStats.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class Dtpg Dtpg.h "sa/Dtpg.h"
/// @brief DTPG の基本エンジン
//////////////////////////////////////////////////////////////////////
class Dtpg
{
public:

  /// @brief デストラクタ
  virtual
  ~Dtpg() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列をセットする．
  virtual
  void
  set_option(const string& option_str) = 0;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] fault_list 対象の故障リスト
  /// @param[in] fmgr 故障の状態を管理するオブジェクト
  /// @param[out] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      const vector<const TpgFault*>& fault_list,
      FaultMgr& fmgr,
      DtpgStats& stats) = 0;

};


/// @brief Original Single エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
Dtpg*
new_DtpgSatS0(const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief Single エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
Dtpg*
new_DtpgSatS(const string& sat_type,
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
Dtpg*
new_DtpgSatH(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

END_NAMESPACE_YM_SATPG_SA

#endif // SA_DTPG_H
