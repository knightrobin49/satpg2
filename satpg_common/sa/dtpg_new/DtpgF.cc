
/// @file DtpgF.cc
/// @brief DtpgF の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgF.h"
#include "sa/DtpgStats.h"
#include "sa/StructSat.h"
#include "sa/FoCone.h"
#include "TpgFault.h"
#include "TpgNetwork.h"


BEGIN_NAMESPACE_YM_SATPG_SA

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] network 対象のネットワーク
// @param[in] mffc_root MFFC の根のノード
DtpgF::DtpgF(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     const TpgNetwork& network,
	     const TpgNode* mffc_root) :
  DtpgBase(sat_type, sat_option, sat_outp, bt),
  mNetwork(network),
  mFfrRoot(mffc_root),
  mStructSat(mNetwork.node_num(), sat_type, sat_option),
  mFoCone(nullptr)

{
  ASSERT_COND( mFfrRoot->mffc_elem_num() == 1 );

  // mFfrRoot を根とする FFR に含まれる故障を求める．
  get_ffr_faults(mFfrRoot);
}

// @brief デストラクタ
DtpgF::~DtpgF()
{
  // mFoCone は StructSat のデストラクタで解放される．
}

// @brief 対象の故障数を返す．
ymuint
DtpgF::fault_num() const
{
  return mFaultList.size();
}

// @brief 対象の故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
DtpgF::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );

  return mFaultList[pos];
}

// @brief CNF 式を作る．
// @param[inout] stats DTPGの統計情報
void
DtpgF::cnf_gen(DtpgStats& stats)
{
  cnf_begin();

  mFoCone = mStructSat.add_focone(mFfrRoot, kVal1);

  cnf_end(stats);
}

// @briefテスト生成を行う．
// @param[in] fault 対象の故障
// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
// @param[inout] stats DTPGの統計情報
// @return 結果を返す．
SatBool3
DtpgF::dtpg(const TpgFault* fault,
	    NodeValList& nodeval_list,
	    DtpgStats& stats)
{
  ASSERT_COND( mFoCone != nullptr );

  //timer.start();

  // FFR 内の故障活性化&伝搬条件を求める．
  NodeValList assignment;
  mStructSat.add_ffr_condition(mFfrRoot, fault, assignment);

  vector<SatLiteral> assumption;
  mStructSat.conv_to_assumption(assignment, assumption);

  //timer.stop();

  // 故障に対するテスト生成を行なう．
  SatBool3 ans = solve(mStructSat.solver(), assumption, fault, mFfrRoot,
		       mFoCone->output_list(),
		       mFoCone->gvar_map(), mFoCone->fvar_map(),
		       nodeval_list, stats);

  return ans;
}

// @brief node と同じ FFR 内の故障を mFaultList に入れる．
// @param[in] node 対象のノード
void
DtpgF::get_ffr_faults(const TpgNode* node)
{
  // node 上の故障を fault_list に入れる．
  ymuint nf = mNetwork.node_fault_num(node->id());
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = mNetwork.node_fault(node->id(), i);
    mFaultList.push_back(f);
  }

  // ファンインが同じ FFR のノードなら再帰する．
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    if ( inode->ffr_root() == node->ffr_root() ) {
      get_ffr_faults(inode);
    }
  }
}

END_NAMESPACE_YM_SATPG_SA
