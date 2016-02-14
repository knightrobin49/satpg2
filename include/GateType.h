#ifndef GATETYPE_H
#define GATETYPE_H

/// @file GateType.h
/// @brief ゲートの種類を表す列挙型の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @brief 論理ゲートの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum GateType {
  /// @brief 定数0
  kGateConst0 = 0,

  /// @brief 定数1
  kGateConst1 = 1,

  /// @brief バッファ
  kGateBuff   = 2,

  /// @brief NOT
  kGateNot    = 3,

  /// @brief AND
  kGateAnd    = 4,

  /// @brief NAND
  kGateNand   = 5,

  /// @brief OR
  kGateOr     = 6,

  /// @brief NOR
  kGateNor    = 7,

  /// @brief XOR
  kGateXor    = 8,

  /// @brief XNOR
  kGateXnor   = 9,

  /// @brief 上記以外
  kGateCplx   = 10
};

/// @brief GateType のストリーム出力
ostream&
operator<<(ostream& s,
	   GateType gate_type);

END_NAMESPACE_YM_SATPG

#endif // GATETYPE_H
