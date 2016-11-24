
/// @file Dop2TvList.cc
/// @brief Dop2TvList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2TvList.h"
#include "Tv2Mgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
DetectOp2*
new_Dop2TvList(Tv2Mgr& tvmgr,
	       vector<TestVector2*>& tvlist)
{
  return new Dop2TvList(tvmgr, tvlist);
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2TvList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
Dop2TvList::Dop2TvList(Tv2Mgr& tvmgr,
		       vector<TestVector2*>& tvlist) :
  mTvMgr(tvmgr),
  mTvList(tvlist)
{
}

// @brief デストラクタ
Dop2TvList::~Dop2TvList()
{
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値割当のリスト
void
Dop2TvList::operator()(const TpgFault* f,
		       const NodeVal2List& assign_list)
{
  TestVector2* tv = mTvMgr.new_vector();
  tv->set_from_assign_list(assign_list);
  mTvList.push_back(tv);
}

END_NAMESPACE_YM_SATPG
