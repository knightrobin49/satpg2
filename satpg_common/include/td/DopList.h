#ifndef TD_DOPLIST_H
#define TD_DOPLIST_H

/// @file td/DopList.h
/// @brief DopList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "td/DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG_TD

//////////////////////////////////////////////////////////////////////
/// @class DopList DopList.h "td/DopList.h"
/// @brief DetectOp2 のリストを保持するクラス
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

END_NAMESPACE_YM_SATPG_TD

#endif // TD_DOPLIST_H
