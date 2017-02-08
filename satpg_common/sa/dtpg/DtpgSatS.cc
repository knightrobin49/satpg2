
/// @file DtpgSatS.cc
/// @brief DtpgSatS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS.h"
#include "FaultMgr.h"
#include "sa/DtpgS.h"
#include "sa/DetectOp.h"
#include "sa/UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG_SA

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
Dtpg*
new_DtpgSatS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatS(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS::DtpgSatS(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  mSatType(sat_type),
  mSatOption(sat_option),
  mSatOutP(sat_outp),
  mBackTracer(bt),
  mDetectOp(dop),
  mUntestOp(uop)
{
}

// @brief デストラクタ
DtpgSatS::~DtpgSatS()
{
}

// @brief オプションを設定する．
void
DtpgSatS::set_option(const string& option_str)
{
  for (string::size_type next = 0; ; ++ next) {
    string::size_type pos = option_str.find(':', next);
    if ( pos == next ) {
      continue;
    }
    string option = option_str.substr(next, pos - next);
    if ( pos == string::npos ) {
      break;
    }
    next = pos;
  }
}

// @breif 時間計測を制御する．
void
DtpgSatS::timer_enable(bool enable)
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fault_list 対象の故障リスト
// @param[in] fmgr 故障の状態を管理するオブジェクト
// @param[out] stats 結果を格納する構造体
void
DtpgSatS::run(TpgNetwork& network,
	      const vector<const TpgFault*>& fault_list,
	      FaultMgr& fmgr,
	      DtpgStats& stats)
{
  DtpgS dtpg_s(mSatType, mSatOption, mSatOutP, mBackTracer, network);

  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    if ( fmgr.status(fault) == kFsUndetected ) {
      NodeValList nodeval_list;
      SatBool3 ans = dtpg_s.dtpg(fault, nodeval_list, stats);
      if ( ans == kB3True ) {
	mDetectOp(fault, nodeval_list);
      }
      else if ( ans == kB3False ) {
	mUntestOp(fault);
      }
    }
  }
}

END_NAMESPACE_YM_SATPG_SA
