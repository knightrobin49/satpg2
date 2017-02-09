
/// @file DtpgS.cc
/// @brief DtpgS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DtpgS.h"
#include "sa/DtpgStats.h"
#include "TpgFault.h"
#include "TpgNetwork.h"

#include "VidLitMap.h"


BEGIN_NAMESPACE_YM_SATPG_SA

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] network 対象のネットワーク
// @param[in] ffr_root 対象の故障のあるFFRの根のノード
DtpgS::DtpgS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     const TpgNetwork& network,
	     const TpgNode* ffr_root) :
  DtpgBase(bt),
  mSolver(sat_type, sat_option, sat_outp),
  mMaxNodeId(network.node_num()),
  mMarkArray(mMaxNodeId, 0U),
  mFfrRoot(ffr_root),
  mGvarMap(mMaxNodeId),
  mFvarMap(mMaxNodeId),
  mDvarMap(mMaxNodeId)
{
  mNodeList.reserve(mMaxNodeId);
  mOutputList.reserve(network.ppo_num());

  mHasCnf = false;
}

// @brief デストラクタ
DtpgS::~DtpgS()
{
}

// @brief 回路の構造を表すCNF式を作る．
// @param[out] stats DTPGの統計情報
//
// このオブジェクトに対しては1回行えばよい．
// というか1回しか行えない．
void
DtpgS::gen_cnf(DtpgStats& stats)
{
  if ( mHasCnf ) {
    cerr << "error: DtpgS::gen_cnf() called more than once." << endl;
    return;
  }

  cnf_begin();

  // ffr_root の TFO を mNodeList に入れる．
  set_tfo_mark(mFfrRoot);
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint nfo = node->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* onode = node->fanout(i);
      set_tfo_mark(onode);
    }
  }
  ymuint tfo_num = mNodeList.size();

  // TFO の TFI を mNodeList に入れる．
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      set_tfi_mark(inode);
    }
  }
  ymuint tfi_num = mNodeList.size();

  // TFO の部分に変数を割り当てる．
  for (ymuint rpos = 0; rpos < tfo_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    SatVarId gvar = mSolver.new_var();
    SatVarId fvar = mSolver.new_var();
    SatVarId dvar = mSolver.new_var();

    mGvarMap.set_vid(node, gvar);
    mFvarMap.set_vid(node, fvar);
    mDvarMap.set_vid(node, dvar);
  }

  // TFI の部分に変数を割り当てる．
  for (ymuint rpos = tfo_num; rpos < tfi_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    SatVarId gvar = mSolver.new_var();

    mGvarMap.set_vid(node, gvar);
    mFvarMap.set_vid(node, gvar);
  }


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfi_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    node->make_cnf(mSolver, VidLitMap(node, mGvarMap));
  }


  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < tfo_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    if ( node != mFfrRoot ) {
      node->make_cnf(mSolver, VidLitMap(node, mFvarMap));
    }
    make_dchain_cnf(node);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint no = mOutputList.size();
  vector<SatLiteral> odiff(no);
  for (ymuint i = 0; i < no; ++ i) {
    const TpgNode* node = mOutputList[i];
    SatLiteral dlit(mDvarMap(node));
    odiff[i] = dlit;
  }
  mSolver.add_clause(odiff);

  mHasCnf = true;

  cnf_end(stats);
}

// @brief テスト生成を行なう．
// @param[in] fault 対象の故障
// @param[out] nodeval_list テストパタンの値割り当てを格納するリスト
// @param[inout] stats DTPGの統計情報
// @return 結果を返す．
SatBool3
DtpgS::dtpg(const TpgFault* fault,
	    NodeValList& nodeval_list,
	    DtpgStats& stats)
{
  if ( !mHasCnf ) {
    cerr << "error: DtpgS::gen_cnf() has not been called before DtpgS::dtpg()" << endl;
    return kB3False;
  }

  if ( fault->ffr_root() != mFfrRoot ) {
    cerr << "error: DtpgS::dtpg(fault). fault is not within mFfrRoot's FFR" << endl;
    return kB3False;
  }

  vector<SatLiteral> assumptions;

  // 故障の活性化条件を作る．
  const TpgNode* inode = fault->tpg_inode();
  // 0 縮退故障の時に 1 にする．
  bool inv = (fault->val() == 1);
  assumptions.push_back(SatLiteral(mGvarMap(inode), inv));

  // ブランチの故障の場合，ゲートの出力までの伝搬条件を作る．
  if ( fault->is_branch_fault() ) {
    const TpgNode* onode = fault->tpg_onode();
    Val3 nval = onode->nval();
    if ( nval != kValX ) {
      ymuint ni = onode->fanin_num();
      bool inv = (nval == kVal0);
      for (ymuint i = 0; i < ni; ++ i) {
	const TpgNode* inode1 = onode->fanin(i);
	if ( inode1 != inode ) {
	  assumptions.push_back(SatLiteral(mGvarMap(inode1), inv));
	}
      }
    }
  }

  // FFR の根までの伝搬条件を作る．
  for (const TpgNode* node = fault->tpg_onode(); node != mFfrRoot;
       node = node->fanout(0)) {
    ASSERT_COND( node->fanout_num() == 1 );

    const TpgNode* fonode = node->fanout(0);
    ymuint ni = fonode->fanin_num();
    if ( ni == 1 ) {
      continue;
    }
    Val3 nval = fonode->nval();
    if ( nval == kValX ) {
      continue;
    }
    bool inv = (nval == kVal0);
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode1 = fonode->fanin(i);
      if ( inode1 != node ) {
	assumptions.push_back(SatLiteral(mGvarMap(inode1), inv));
      }
    }
  }

  // mFfrRoot において故障の影響が伝搬していなければならない．
  assumptions.push_back(SatLiteral(mDvarMap(mFfrRoot)));

  SatBool3 ans = solve(mSolver, assumptions, fault, mFfrRoot,
		       mOutputList, mGvarMap, mFvarMap,
		       nodeval_list, stats);

  return ans;
}

// @brief 故障伝搬条件を表すCNF式を生成する．
// @param[in] node 対象のノード
void
DtpgS::make_dchain_cnf(const TpgNode* node)
{
  SatLiteral glit(mGvarMap(node));
  SatLiteral flit(mFvarMap(node));
  SatLiteral dlit(mDvarMap(node));

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
  mSolver.add_clause(~glit, ~flit, ~dlit);
  mSolver.add_clause( glit,  flit, ~dlit);

  if ( node->is_ppo() ) {
    mSolver.add_clause(~glit,  flit,  dlit);
    mSolver.add_clause( glit, ~flit,  dlit);
  }
  else {
    // dlit -> ファンアウト先のノードの dlit の一つが 1
    ymuint nfo = node->fanout_num();
    if ( nfo == 1 ) {
      SatLiteral odlit(mDvarMap(node->fanout(0)));
      mSolver.add_clause(~dlit, odlit);
    }
    else {
      vector<SatLiteral> tmp_lits(nfo + 1);
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* onode = node->fanout(i);
	tmp_lits[i] = SatLiteral(mDvarMap(onode));
      }
      tmp_lits[nfo] = ~dlit;
      mSolver.add_clause(tmp_lits);

      const TpgNode* imm_dom = node->imm_dom();
      if ( imm_dom != nullptr ) {
	SatLiteral odlit(mDvarMap(imm_dom));
	mSolver.add_clause(~dlit, odlit);
      }
    }
  }
}

END_NAMESPACE_YM_SATPG_SA
