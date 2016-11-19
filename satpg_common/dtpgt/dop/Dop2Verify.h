#ifndef DOP2VERIFY_H
#define DOP2VERIFY_H

/// @file Dop2Verify.h
/// @brief Dop2Verify のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dop2Verify Dop2Verify.h "Dop2Verify.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class Dop2Verify :
  public DetectOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  Dop2Verify(Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~Dop2Verify();


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

  // 故障シミュレータ
  Fsim& mFsim;

};

END_NAMESPACE_YM_SATPG

#endif // DOP2VERIFY_H
