#ifndef DOP2BASE_H
#define DOP2BASE_H

/// @file Dop2Base.h
/// @brief Dop2Base のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dop2Base Dop2Base.h "Dop2Base.h"
/// @brief DetectOp2 の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class Dop2Base :
  public DetectOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr FaultMgr
  Dop2Base(FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~Dop2Base();


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

  // FaultMgr
  FaultMgr& mMgr;

};

END_NAMESPACE_YM_SATPG

#endif // DOP2BASE_H
