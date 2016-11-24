#ifndef FOPRTPGT_H
#define FOPRTPGT_H

/// @file FopRtpgT.h
/// @brief FopRtpgT のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopRtpgT FopRtpgT.h "FopRtpgT.h"
/// @brief Rtpg で用いる FsimOp
//////////////////////////////////////////////////////////////////////
class FopRtpgT :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  FopRtpgT(FsimT& fsim);

  /// @brief デストラクタ
  virtual
  ~FopRtpgT();


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

  /// @brief 検出された故障のリストを得る．
  const vector<const TpgFault*>&
  fault_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  FsimT& mFsim;

  // 検出回数
  ymuint32 mCount[kPvBitLen];

  // 検出された故障のリスト
  vector<const TpgFault*> mFaultList;

};

END_NAMESPACE_YM_SATPG

#endif // FOP1RTPGT_H
