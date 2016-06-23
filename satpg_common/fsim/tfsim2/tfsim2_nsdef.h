#ifndef TFSIM2_NSDEF_H
#define TFSIM2_NSDEF_H

/// @file tfsim2_nsdef.h
/// @brief TFsim2 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "satpg.h"

#define BEGIN_NAMESPACE_YM_SATPG_TFSIM2 \
BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE(nsTFsim2)

#define END_NAMESPACE_YM_SATPG_TFSIM2 \
END_NAMESPACE(nsTFsim2) \
END_NAMESPACE_YM_SATPG

#endif // TFSIM2_NSDEF_H
