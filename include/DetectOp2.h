#ifndef DETECTOP2_H
#define DETECTOP2_H

/// @file DetectOp2.h
/// @brief DetectOp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DetectOp2 DetectOp2.h "DetectOp2.h"
/// @brief テストパタンが見つかったときの処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class DetectOp2
{
public:

  /// @brief デストラクタ
  virtual
  ~DetectOp2() { }


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
	     const NodeVal2List& assign_list) = 0;

};

/// @brief 'base' タイプを生成する．
/// @param[in] fmgr 故障マネージャ
DetectOp2*
new_Dop2Base(FaultMgr& fmgr);

/// @brief 'drop' タイプを生成する．
/// @param[in] fmgr 故障マネージャ
/// @param[in] fsim 故障シミュレータ
DetectOp2*
new_Dop2Drop(FaultMgr& fmgr,
	     Fsim& fsim);

/// @brief 'tvlist' タイプを生成する．
/// @param[in] tvmgr テストベクタのマネージャ
/// @param[in] tvlist テストベクタのリスト
DetectOp2*
new_Dop2TvList(TvMgr& tvmgr,
	       vector<TestVector*>& tvlist);

/// @brief 'verify' タイプを生成する．
/// @param[in] fsim 故障シミュレータ
DetectOp2*
new_Dop2Verify(Fsim& fsim);

/// @brief 'dummy' タイプを生成する．
DetectOp2*
new_Dop2Dummy();

END_NAMESPACE_YM_SATPG

#endif // DETECTOP2_H
