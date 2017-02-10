
/// @file DtpgM.cc
/// @brief DtpgM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DtpgM.h"
#include "sa/DtpgStats.h"
#include "sa/StructSat.h"
#include "sa/FoCone.h"
#include "sa/MffcCone.h"
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
DtpgM::DtpgM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     const TpgNetwork& network,
	     const TpgNode* mffc_root) :
  DtpgBase(bt),
  mNetwork(network),
  mMffcRoot(mffc_root),
  mFaultInfoArray(mNetwork.max_fault_id()),
  mStructSat(mNetwork.node_num(), sat_type, sat_option, sat_outp),
  mMffcCone(nullptr)
{
  ymuint ne = mMffcRoot->mffc_elem_num();
  for (ymuint i = 0; i < ne; ++ i) {
    const TpgNode* node1 = mMffcRoot->mffc_elem(i);
    // node1 を根とする FFR に含まれる故障を求める．
    get_ffr_faults(node1, i);
  }
}

// @brief デストラクタ
DtpgM::~DtpgM()
{
  // mMffcCone は StructSat のデストラクタで解放される．
}

// @brief 対象の故障数を返す．
ymuint
DtpgM::fault_num() const
{
  return mFaultList.size();
}

// @brief 対象の故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
DtpgM::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );

  return mFaultList[pos];
}

// @brief CNF 式を作る．
void
DtpgM::cnf_gen(DtpgStats& stats)
{
  cnf_begin();

  mMffcCone = mStructSat.add_mffccone(mMffcRoot);

  cnf_end(stats);
}

// @briefテスト生成を行う．
// @param[in] fault 対象の故障
// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
// @param[inout] stats DTPGの統計情報
// @return 結果を返す．
SatBool3
DtpgM::dtpg(const TpgFault* fault,
	    NodeValList& nodeval_list,
	    DtpgStats& stats)
{
  vector<SatLiteral> assumption;

  ASSERT_COND( mMffcCone != nullptr );

  //timer.start();

  const FaultInfo& fi = mFaultInfoArray[fault->id()];

  // FFR 内の故障活性化&伝搬条件を求める．
  NodeValList assignment;
  mStructSat.add_ffr_condition(fi.mFfrRoot, fault, assignment);
  mStructSat.conv_to_assumption(assignment, assumption);

  // FFR の根の出力に故障を挿入する．
  mMffcCone->select_fault_node(fi.mElemPos, assumption);

  //timer.stop();

  // 故障に対するテスト生成を行なう．
  SatBool3 ans = solve(mStructSat.solver(), assumption, fault, mMffcRoot,
		       mMffcCone->output_list(),
		       mMffcCone->gvar_map(), mMffcCone->fvar_map(),
		       nodeval_list, stats);

  return ans;
}

// @brief node と同じ FFR 内の故障を mFaultList に入れる．
// @param[in] node 対象のノード
// @param[in] elem_pos MFFC 内の要素番号
void
DtpgM::get_ffr_faults(const TpgNode* node,
		      ymuint elem_pos)
{
  // node 上の故障を mFaultList に入れる．
  ymuint nf = mNetwork.node_fault_num(node->id());
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = mNetwork.node_fault(node->id(), i);
    mFaultList.push_back(f);
    // と同時に mFaultInfoArray に必要な情報をセットする．
    FaultInfo& fi = mFaultInfoArray[f->id()];
    fi.mFfrRoot = node->ffr_root();
    fi.mElemPos = elem_pos;
  }

  // ファンインが同じ FFR のノードなら再帰する．
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    if ( inode->ffr_root() == node->ffr_root() ) {
      get_ffr_faults(inode, elem_pos);
    }
  }
}

END_NAMESPACE_YM_SATPG_SA
