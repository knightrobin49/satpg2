
/// @file DtpgSatF.cc
/// @brief DtpgSatF の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatF.h"
#include "DtpgStats.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "FoCone.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "FaultMgr.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatF(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatF(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatF::DtpgSatF(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatF::~DtpgSatF()
{
}

BEGIN_NONAMESPACE

void
get_ffr_faults(const TpgNode* node,
	       const vector<bool>& fault_mark,
	       vector<const TpgFault*>& fault_list)
{
  // node 上のマークされている故障を fault_list に入れる．
  ymuint nf = node->fault_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = node->fault(i);
    if ( fault_mark[f->id()] ) {
      fault_list.push_back(f);
    }
  }

  // ファンインが同じ FFR のノードなら再帰する．
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    if ( inode->ffr_root() == node->ffr_root() ) {
      get_ffr_faults(inode, fault_mark, fault_list);
    }
  }
}

END_NONAMESPACE

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 対象の故障リスト
// @param[out] stats 結果を格納する構造体
void
DtpgSatF::run(TpgNetwork& network,
	      FaultMgr& fmgr,
	      Fsim& fsim,
	      const vector<const TpgFault*>& fault_list,
	      DtpgStats& stats)
{
  clear_stats();

  // 故障シミュレータに故障リストをセットする．
  fsim.set_faults(fault_list);

  ymuint max_fault_id = network.max_fault_id();

  // fault_list に含まれる故障に印をつける．
  vector<bool> fault_mark(max_fault_id, false);
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    ymuint fid = fault->id();
    fault_mark[fid] = true;
  }

  ymuint nn = network.active_node_num();
  ymuint max_id = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.active_node(i);
    if ( node->ffr_root() != node ) {
      continue;
    }

    // node を根とする FFR に含まれる故障を求める．
    vector<const TpgFault*> f_list;
    get_ffr_faults(node, fault_mark, f_list);
    if ( f_list.empty() ) {
      // 故障が残っていないのでパス
      continue;
    }

    cnf_begin();

    GvalCnf gval_cnf(max_id, sat_type(), sat_option(), sat_outp());
    FvalCnf fval_cnf(gval_cnf);
    FoCone focone(max_id, node);
    fval_cnf.make_cnf(node, focone, kVal1);

    cnf_end();

    ymuint nf = f_list.size();
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* fault = f_list[i];
      if ( fmgr.status(fault) != kFsUndetected ) {
	continue;
      }

      // FFR 内の故障活性化&伝搬条件を求める．
      NodeValList assignment;
      gval_cnf.add_ffr_condition(node, fault, assignment);

      vector<SatLiteral> assumption;
      gval_cnf.conv_to_assumption(assignment, assumption);

      // 故障に対するテスト生成を行なう．
      solve(gval_cnf.solver(), assumption, fault, focone.output_list(), fval_cnf.gvar_map(), fval_cnf.fvar_map());
    }
  }

  get_stats(stats);
}

END_NAMESPACE_YM_SATPG
