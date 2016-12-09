
/// @file DtpgSatH.cc
/// @brief DtpgSatH の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatH.h"
#include "sa/DtpgStats.h"
#include "sa/StructSat.h"
#include "sa/FoCone.h"
#include "sa/MffcCone.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "FaultMgr.h"
#include "sa/Fsim.h"


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
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatH::~DtpgSatH()
{
}

BEGIN_NONAMESPACE

void
get_ffr_faults(const TpgNetwork& network,
	       const TpgNode* node,
	       const vector<bool>& fault_mark,
	       vector<const TpgFault*>& fault_list)
{
  // node 上のマークされている故障を fault_list に入れる．
  ymuint nf = network.node_fault_num(node->id());
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = network.node_fault(node->id(), i);
    if ( fault_mark[f->id()] ) {
      fault_list.push_back(f);
    }
  }

  // ファンインが同じ FFR のノードなら再帰する．
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    if ( inode->ffr_root() == node->ffr_root() ) {
      get_ffr_faults(network, inode, fault_mark, fault_list);
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
DtpgSatH::run(TpgNetwork& network,
	      FaultMgr& fmgr,
	      Fsim& fsim,
	      const vector<const TpgFault*>& fault_list,
	      DtpgStats& stats)
{
  clear_stats();

  StopWatch timer;
  timer.start();

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

  ymuint nn = network.node_num();
  ymuint max_id = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);
    if ( node->imm_dom() != nullptr ) {
      continue;
    }

    const TpgNode* mffc_root = node;
    ymuint ne = mffc_root->mffc_elem_num();
    if ( ne == 1 ) {
      // mffc_root を根とする FFR に含まれる故障を求める．
      vector<const TpgFault*> f_list;
      get_ffr_faults(network, mffc_root, fault_mark, f_list);
      if ( f_list.empty() ) {
	// 故障が残っていないのでパス
	continue;
      }

      timer.stop();
      cnf_begin();

      StructSat struct_sat(max_id, sat_type(), sat_option());
      const FoCone* focone = struct_sat.add_focone(mffc_root, kVal1);

      cnf_end();
      timer.start();

      ymuint nf = f_list.size();
      for (ymuint i = 0; i < nf; ++ i) {
	const TpgFault* fault = f_list[i];
	if ( fmgr.status(fault) != kFsUndetected ) {
	  continue;
	}

	// FFR 内の故障活性化&伝搬条件を求める．
	NodeValList assignment;
	struct_sat.add_ffr_condition(mffc_root, fault, assignment);

	vector<SatLiteral> assumption;
	struct_sat.conv_to_assumption(assignment, assumption);

#if 0
	cout << fault->str() << ":";
	for (ymuint i = 0; i < assumption.size(); ++ i) {
	  cout << " " << assumption[i];
	}
	cout << endl;
#endif

	timer.stop();

	// 故障に対するテスト生成を行なう．
	solve(struct_sat.solver(), assumption, fault, mffc_root, focone->output_list(),
	      focone->gvar_map(), focone->fvar_map());

	timer.start();
      }
    }
    else {
      vector<vector<const TpgFault*> > f_list(ne);
      ymuint nf = 0;
      for (ymuint j = 0; j < ne; ++ j) {
	const TpgNode* node1 = mffc_root->mffc_elem(j);
	// node1 を根とする FFR に含まれる故障を求める．
	get_ffr_faults(network, node1, fault_mark, f_list[j]);
	nf += f_list[j].size();
      }
      if ( nf == 0 ) {
	// 故障が残っていないのでパス
	continue;
      }

      timer.stop();

      cnf_begin();

      StructSat struct_sat(max_id, sat_type(), sat_option());
      const MffcCone* mffc_cone = struct_sat.add_mffccone(mffc_root);

      cnf_end();

      timer.start();

      for (ymuint j = 0; j < ne; ++ j) {
	const TpgNode* ffr_root = mffc_root->mffc_elem(j);
	const vector<const TpgFault*>& f_list1 = f_list[j];
	ymuint nf = f_list1.size();
	for (ymuint i = 0; i < nf; ++ i) {
	  const TpgFault* fault = f_list1[i];
	  if ( fmgr.status(fault) != kFsUndetected ) {
	    continue;
	  }

	  // FFR 内の故障活性化&伝搬条件を求める．
	  NodeValList assignment;
	  struct_sat.add_ffr_condition(ffr_root, fault, assignment);

	  vector<SatLiteral> assumption;
	  struct_sat.conv_to_assumption(assignment, assumption);

	  // ffr_root の出力に故障を挿入する．
	  mffc_cone->select_fault_node(j, assumption);

	  timer.stop();

	  // 故障に対するテスト生成を行なう．
	  solve(struct_sat.solver(), assumption, fault, mffc_root, mffc_cone->output_list(),
		mffc_cone->gvar_map(), mffc_cone->fvar_map());

	  timer.start();

	}
      }
    }
  }

#if 0
  USTime time = timer.time();
  cout << "MISC time = " << time << endl;
#endif

  get_stats(stats);
}

END_NAMESPACE_YM_SATPG_SA
