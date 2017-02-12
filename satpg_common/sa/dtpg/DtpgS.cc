
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
  DtpgBase(sat_type, sat_option, sat_outp, bt, network, ffr_root)
{
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

  gen_cnf_base();

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

  if ( fault->ffr_root() != root_node() ) {
    cerr << "error: DtpgS::dtpg(fault). fault is not within mFfrRoot's FFR" << endl;
    return kB3False;
  }

  NodeValList assign_list;

  make_ffr_condition(fault, assign_list);

  SatBool3 ans = solve(vector<SatLiteral>(), assign_list, fault, nodeval_list, stats);

  return ans;
}

END_NAMESPACE_YM_SATPG_SA
