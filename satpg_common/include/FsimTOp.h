#ifndef FSIMTOP_H
#define FSIMTOP_H

/// @file FsimTop.h
/// @brief FsimTop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimTop FsimTop.h "FsimTop.h"
/// @brief Fsim::ppsfp() で故障を検出した時の処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class FsimTop
{
public:

  /// @brief デストラクタ
  virtual
  ~FsimTop() { }


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
	     PackedVal dpat) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // FSIMTOP2_H
