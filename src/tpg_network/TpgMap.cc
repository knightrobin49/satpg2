
/// @file TpgMap.cc
/// @brief TpgMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] inode_array 入力のノードの配列
// @param[in] ipos_array 入力のファンイン番号の配列
TpgMap::TpgMap(TpgNode** inode_array,
	       ymuint* ipos_array) :
  mInputNodeArray(inode_array),
  mInputPosArray(ipos_array)
{
}

// @brief デストラクタ
TpgMap::~TpgMap()
{
}

// @brief 入力に対応するノードを得る．
// @param[in] ipos 入力の位置番号
TpgNode*
TpgMap::input(ymuint ipos) const
{
  return mInputNodeArray[ipos];
}

// @brief 入力に対応するノードのファンイン番号を得る．
// @param[in] ipos 入力の位置番号
ymuint
TpgMap::input_pos(ymuint ipos) const
{
  return mInputPosArray[ipos];
}

END_NAMESPACE_YM_SATPG
