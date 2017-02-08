
/// @file DtpgSatH.cc
/// @brief DtpgSatH の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatH.h"

#include "TpgNetwork.h"
#include "TpgFault.h"
#include "FaultMgr.h"

#include "sa/DtpgM.h"
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
new_DtpgSatH(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatH(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatH::DtpgSatH(const string& sat_type,
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
DtpgSatH::~DtpgSatH()
{
}

// @brief オプション文字列をセットする．
void
DtpgSatH::set_option(const string& option_str)
{
}

// @breif 時間計測を制御する．
void
DtpgSatH::timer_enable(bool enable)
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 対象の故障リスト
// @param[in] fmgr 故障の状態を管理するオブジェクト
// @param[out] stats 結果を格納する構造体
void
DtpgSatH::run(TpgNetwork& network,
	      const vector<const TpgFault*>& fault_list,
	      FaultMgr& fmgr,
	      DtpgStats& stats)
{
  // fault_list に含まれる故障に印をつける．
  ymuint max_fault_id = network.max_fault_id();
  vector<bool> fault_mark(max_fault_id, false);
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    ymuint fid = fault->id();
    fault_mark[fid] = true;
  }

  ymuint nn = network.node_num();
  ymuint max_id = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);
    if ( node->imm_dom() != nullptr ) {
      continue;
    }

    DtpgM dtpg_m(mSatType, mSatOption, mSatOutP, mBackTracer, network, node);

    // node を根とする MFFC に含まれる故障のうち fault_list に入っていたものを求める．
    vector<const TpgFault*> f_list;
    ymuint nf = dtpg_m.fault_num();
    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* f = dtpg_m.fault(j);
      if ( fault_mark[f->id()] ) {
	f_list.push_back(f);
      }
    }
    if ( f_list.empty() ) {
      // 故障が残っていないのでパス
      continue;
    }

    dtpg_m.cnf_gen(stats);

    for (ymuint j = 0; j < nf; ++ j) {
      const TpgFault* fault = f_list[j];
      if ( fmgr.status(fault) == kFsUndetected ) {
	// 故障に対するテスト生成を行なう．
	NodeValList nodeval_list;
	SatBool3 ans = dtpg_m.dtpg(fault, nodeval_list, stats);
	if ( ans == kB3True ) {
	  mDetectOp(fault, nodeval_list);
	}
	else if ( ans == kB3False ) {
	  mUntestOp(fault);
	}
      }
    }
  }
}

END_NAMESPACE_YM_SATPG_SA
