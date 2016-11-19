#ifndef DOP2DROP_H
#define DOP2DROP_H

/// @file Dop2Drop.h
/// @brief Dop2Drop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp2.h"
#include "Fop2Drop.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dop2Drop Dop2Drop.h "Dop2Drop.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class Dop2Drop :
  public DetectOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  Dop2Drop(FaultMgr& fmgr,
	   Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~Dop2Drop();


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

  // 故障マネージャ
  FaultMgr& mMgr;

  // 故障シミュレータ
  Fsim& mFsim;

  // Fsim 用のファンクタ
  Fop2Drop mOp;

};

END_NAMESPACE_YM_SATPG

#endif // DOP2DROP_H
