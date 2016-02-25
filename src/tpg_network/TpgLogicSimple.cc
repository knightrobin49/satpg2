
/// @file TpgLogicSimple.cc
/// @brief TpgLogicSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogicSimple.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 根のノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
bool
TpgLogicSimple::is_root() const
{
  return true;
}

// @brief 内部ノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
bool
TpgLogicSimple::is_internal() const
{
  return false;
}

// @brief もとのゲートのファンインに対応するノードを返す．
// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
const TpgNode*
TpgLogicSimple::input_map(ymuint pos) const
{
  return this;
}

// @brief BnNode のファンインに対応するノードのファンイン番号を返す．
// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
ymuint
TpgLogicSimple::ipos_map(ymuint pos) const
{
  return pos;
}

END_NAMESPACE_YM_SATPG
