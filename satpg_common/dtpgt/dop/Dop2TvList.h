#ifndef DOP2TVLIST_H
#define DOP2TVLIST_H

/// @file Dop2TvList.h
/// @brief Dop2TvList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dop2TvList Dop2TvList.h "Dop2TvList.h"
/// @brief DetectOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class Dop2TvList :
  public DetectOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] tvlist テストベクタのリスト
  Dop2TvList(TvMgr& tvmgr,
	     vector<TestVector*>& tvlist);

  /// @brief デストラクタ
  virtual
  ~Dop2TvList();


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
	     const NodeVal2List& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタのマネージャ
  TvMgr& mTvMgr;

  // テストベクタのリスト
  vector<TestVector*>& mTvList;

};

END_NAMESPACE_YM_SATPG

#endif // DOP2TVLIST_H
