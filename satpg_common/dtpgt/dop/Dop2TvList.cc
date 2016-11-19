
/// @file Dop2TvList.cc
/// @brief Dop2TvList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2TvList.h"
#include "TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'base' タイプを生成する．
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
DetectOp2*
new_Dop2TvList(TvMgr& tvmgr,
	       vector<TestVector*>& tvlist)
{
  return new Dop2TvList(tvmgr, tvlist);
}


//////////////////////////////////////////////////////////////////////
// クラス Dop2TvList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr テストベクタのマネージャ
// @param[in] tvlist テストベクタのリスト
Dop2TvList::Dop2TvList(TvMgr& tvmgr,
		       vector<TestVector*>& tvlist) :
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
  TestVector* tv = mTvMgr.new_vector();
  tv->set_from_assign_list(assign_list, mTvMgr.input_num());
  mTvList.push_back(tv);
}

END_NAMESPACE_YM_SATPG
