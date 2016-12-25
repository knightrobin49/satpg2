#ifndef FSIM_NSDEF_H
#define FSIM_NSDEF_H

/// @file fsim_nsdef.h
/// @brief Fsim 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "sa/sa_nsdef.h"

#define BEGIN_NAMESPACE_YM_SATPG_FSIM \
BEGIN_NAMESPACE_YM_SATPG_SA \
BEGIN_NAMESPACE(nsFsim2New)

#define END_NAMESPACE_YM_SATPG_FSIM \
END_NAMESPACE(nsFsim2New) \
END_NAMESPACE_YM_SATPG_SA

#endif // FSIM_NSDEF_H
