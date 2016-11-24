#ifndef DOP2LIST_H
#define DOP2LIST_H

/// @file Dop2List.h
/// @brief Dop2List のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dop2List Dop2List.h "Dop2List.h"
/// @brief DetectOp2 のリストを保持するクラス
///
/// 自身も DetectOp2 の継承クラスである．
//////////////////////////////////////////////////////////////////////
class Dop2List :
  public DetectOp2
{
public:

  /// @brief コンストラクタ
  Dop2List();

  /// @brief デストラクタ
  virtual
  ~Dop2List();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dop を追加する．
  void
  add(DetectOp2* dop);

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

  // dop のリスト
  vector<DetectOp2*> mDopList;

};

END_NAMESPACE_YM_SATPG

#endif // DOP2LIST_H
