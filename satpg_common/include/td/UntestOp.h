#ifndef TD_UNTESTOP_H
#define TD_UNTESTOP_H

/// @file td/UntestOp.h
/// @brief UntestOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "td/td_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_TD

//////////////////////////////////////////////////////////////////////
/// @class UntestOp UntestOp.h "td/UntestOp.h"
/// @brief テスト不能故障と判定された時の処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class UntestOp
{
public:

  /// @brief デストラクタ
  virtual
  ~UntestOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(const TpgFault* f) = 0;

};

/// @brief 'base' タイプを生成する．
// @param[in] fmgr 故障マネージャ
UntestOp*
new_UopBase(TpgFaultMgr& fmgr);

/// @brief 'skip' タイプを生成する．
/// @param[in] threshold しきい値
/// @param[in] max_fault_id 故障番号の最大値
UntestOp*
new_UopSkip(ymuint threshold,
	    ymuint max_fault_id);

/// @brief 'dummy' タイプを生成する．
UntestOp*
new_UopDummy();

END_NAMESPACE_YM_SATPG_TD

#endif // UNTESTOP_H
