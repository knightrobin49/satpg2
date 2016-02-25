
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "TpgMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
TpgNode::TpgNode(ymuint id) :
  mId(id)
{
  mName = nullptr;
  mMap = nullptr;
  mMarks = 0U;
}

// @brief デストラクタ
TpgNode::~TpgNode()
{
}

// @brief ID番号を得る．
ymuint
TpgNode::id() const
{
  return mId;
}

// @brief 名前を得る．
const char*
TpgNode::name() const
{
  return mName;
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
bool
TpgNode::is_input() const
{
  return false;
}

// @brief 外部入力タイプの時に入力番号を返す．
ymuint
TpgNode::input_id() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
bool
TpgNode::is_output() const
{
  return false;
}

// @brief 外部出力タイプの時に出力番号を返す．
ymuint
TpgNode::output_id() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
ymuint
TpgNode::output_id2() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief logic タイプの時 true を返す．
bool
TpgNode::is_logic() const
{
  return false;
}

// @brief ゲートタイプを得る．
TpgNode::GateType
TpgNode::gate_type() const
{
  ASSERT_NOT_REACHED;
  return kGateConst0;
}

// @brief controling value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::coval() const
{
  return kValX;;
}

// @brief noncontroling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::noval() const
{
  return kValX;
}

// @brief 値のノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
bool
TpgNode::is_root() const
{
  return mMap != nullptr;
}

// @brief 内部ノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
bool
TpgNode::is_internal() const
{
  return !is_root();
}

// @brief ファンイン数を得る．
ymuint
TpgNode::fanin_num() const
{
  return 0;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgNode::fanin(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief TgNode のファンインに対応するノードを返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
const TpgNode*
TpgNode::input_map(ymuint pos) const
{
  ASSERT_COND( is_root() );
  return mMap->input(pos);
}

// @brief TgNode のファンインに対応するノードのファンイン番号を返す．
// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
//
// is_root() が true の時のみ意味を持つ．
ymuint
TpgNode::ipos_map(ymuint pos) const
{
  ASSERT_COND( is_root() );
  return mMap->input_pos(pos);
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgNode::make_cnf(SatSolver& solver,
		  const LitMap& lit_map)
{
  ASSERT_NOT_REACHED;
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgNode::output_fault(int val) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgNode::input_fault(int val,
		     ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgNode::fault_num() const
{
  return 0;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgNode::fault(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

END_NAMESPACE_YM_SATPG
