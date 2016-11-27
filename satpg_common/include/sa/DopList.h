#ifndef DOPLIST_H
#define DOPLIST_H

/// @file DopList.h
/// @brief DopList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "sa/DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class DopList DopList.h "sa/DopList.h"
/// @brief DetectOp のリストを保持するクラス
///
/// 自身も DetectOp の継承クラスである．
//////////////////////////////////////////////////////////////////////
class DopList :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  DopList();

  /// @brief デストラクタ
  virtual
  ~DopList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dop を追加する．
  void
  add(DetectOp* dop);

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

  // dop のリスト
  vector<DetectOp*> mDopList;

};

END_NAMESPACE_YM_SATPG_SA

#endif // DOPLIST_H
