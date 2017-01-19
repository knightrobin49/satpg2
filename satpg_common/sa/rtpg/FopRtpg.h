#ifndef FOPRTPG_H
#define FOPRTPG_H

/// @file FopRtpg.h
/// @brief FopRtpg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "sa/FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG_SA

//////////////////////////////////////////////////////////////////////
/// @class FopRtpg FopRtpg.h "FopRtpg.h"
/// @brief Rtpg で用いる FsimOp
//////////////////////////////////////////////////////////////////////
class FopRtpg :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fmgr 故障マネージャ
  FopRtpg(Fsim& fsim,
	  FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~FopRtpg();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(const TpgFault* f,
	     PackedVal dpat);

  /// @brief 初期化する．
  void
  init();

  /// @brief 検出回数をクリアする．
  void
  clear_count();

  /// @brief 検出回数を得る．
  /// @param[in] bitpos ビット位置
  ymuint
  count(ymuint bitpos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 故障マネージャ
  FaultMgr& mFaultMgr;

  // 検出回数
  ymuint32 mCount[kPvBitLen];

};

END_NAMESPACE_YM_SATPG_SA

#endif // FOP1RTPG_H
