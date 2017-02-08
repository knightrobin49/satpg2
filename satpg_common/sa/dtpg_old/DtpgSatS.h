#ifndef DTPGSATS_H
#define DTPGSATS_H

/// @file DtpgSatS.h
/// @brief DtpgSatS のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatS DtpgSatS.h "DtpgSatS.h"
/// @brief 1つの故障を対象とした CNF を生成する DtpgSat
//////////////////////////////////////////////////////////////////////
class DtpgSatS :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatS(const string& sat_type,
	   const string& sat_option,
	   ostream* sat_outp,
	   BackTracer& bt,
	   DetectOp& dop,
	   UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatS();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列をセットする．
  virtual
  void
  set_option(const string& option_str);

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] fault_list 対象の故障リスト
  /// @param[in] fmgr 故障の状態を管理するオブジェクト
  /// @param[out] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
      const vector<const TpgFault*>& fault_list,
      FaultMgr& fmgr,
      DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


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

  // 検出時に呼ばれるファンクタ
  DetectOp& mDetectOp;

  // 検出不能時に呼ばれるファンクタ
  UntestOp& mUntestOp;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DTPGSATS_H
