#ifndef DOPTVLIST_H
#define DOPTVLIST_H

/// @file DopTvList.h
/// @brief DopTvList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DopTvList DopTvList.h "DopTvList.h"
/// @brief DetectOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class DopTvList :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] tvlist テストベクタのリスト
  DopTvList(TvMgr& tvmgr,
	    vector<const TestVector*>& tvlist);

  /// @brief デストラクタ
  virtual
  ~DopTvList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] assign_list 値割当のリスト
  virtual
  void
  operator()(const TpgFault* f,
	     const NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタのマネージャ
  TvMgr& mTvMgr;

  // テストベクタのリスト
  vector<const TestVector*>& mTvList;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DOPTVLIST_H
