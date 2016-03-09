
/// @file TpgFault.cc
/// @brief TpgFault の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgFault.h"
#include "TpgOutputFault.h"
#include "TpgInputFault.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgFault
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] val 故障値
// @param[in] rep_fault 代表故障
TpgFault::TpgFault(ymuint id,
		   int val,
		   const TpgFault* rep_fault) :
  mId(id),
  mVal(val),
  mRepFault(rep_fault)
{
}

// @brief デストラクタ
TpgFault::~TpgFault()
{
}

// @brief ストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const TpgFault* f)
{
  return s << f->str();
}


//////////////////////////////////////////////////////////////////////
// クラス TpgOutputFault
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 故障位置のノード名
// @param[in] val 故障値
// @param[in] node 故障位置のノード
// @param[in] rep_fault 代表故障
TpgOutputFault::TpgOutputFault(ymuint id,
			       const char* name,
			       int val,
			       const TpgNode* node,
			       const TpgFault* rep_fault) :
  TpgFault(id, val, rep_fault),
  mNodeName(name),
  mTpgNode(node)
{
  ASSERT_COND( tpg_node() != nullptr );
}

// @brief デストラクタ
TpgOutputFault::~TpgOutputFault()
{
}

// @brief node() に対応する TpgNode を返す．
inline
const TpgNode*
TpgOutputFault::tpg_node() const
{
  return mTpgNode;
}

// @brief 故障の入力側の TpgNode を返す．
//
// 出力の故障の場合には tpg_node() と同じ値を返す．
const TpgNode*
TpgOutputFault::tpg_inode() const
{
  return tpg_node();
}

// @brief 出力の故障の時 true を返す．
bool
TpgOutputFault::is_output_fault() const
{
  return true;
}

// @brief 故障位置を返す．
ymuint
TpgOutputFault::fault_pos() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief tpg_inode 上の故障位置を返す．
//
// is_input_fault() == true の時のみ意味を持つ．
ymuint
TpgOutputFault::tpg_pos() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 故障の内容を表す文字列を返す．
string
TpgOutputFault::str() const
{
  ostringstream ans;
  ans << mNodeName << ":O:";
  if ( val() ) {
    ans <<"SA1";
  }
  else {
    ans <<"SA0";
  }
  return ans.str();
}


//////////////////////////////////////////////////////////////////////
// クラス TpgInputFault
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name ノード名
// @param[in] val 故障値
// @param[in] pos 故障の入力位置
// @param[in] node 故障位置の TpgNode
// @param[in] inode 入力側の TpgNode
// @param[in] tpg_pos node 上の故障位置
// @param[in] rep_fault 代表故障
TpgInputFault::TpgInputFault(ymuint id,
			     const char* name,
			     int val,
			     ymuint pos,
			     const TpgNode* node,
			     const TpgNode* inode,
			     ymuint tpg_pos,
			     const TpgFault* rep_fault) :
  TpgFault(id, val, rep_fault),
  mNodeName(name),
  mTpgNode(node),
  mPos(pos),
  mI_TpgNode(inode),
  mTpgPos(tpg_pos)
{
  ASSERT_COND( tpg_node() != nullptr );
  ASSERT_COND( tpg_inode() != nullptr );
}

// @brief デストラクタ
TpgInputFault::~TpgInputFault()
{
}

// @brief node() に対応する TpgNode を返す．
const TpgNode*
TpgInputFault::tpg_node() const
{
  return mTpgNode;
}

// @brief 故障の入力側の TpgNode を返す．
//
// 出力の故障の場合には tpg_node() と同じ値を返す．
const TpgNode*
TpgInputFault::tpg_inode() const
{
  return mI_TpgNode;
}

// @brief 出力の故障の時 true を返す．
bool
TpgInputFault::is_output_fault() const
{
  return false;
}

// @brief 故障位置を返す．
//
// is_input_fault() == true の時のみ意味を持つ．
ymuint
TpgInputFault::fault_pos() const
{
  return mPos;
}

// @brief tpg_inode 上の故障位置を返す．
//
// is_input_fault() == true の時のみ意味を持つ．
ymuint
TpgInputFault::tpg_pos() const
{
  return mTpgPos;
}

// @brief 故障の内容を表す文字列を返す．
string
TpgInputFault::str() const
{
  ostringstream ans;
  ans << mNodeName << ":I" << fault_pos() << ":";
  if ( val() ) {
    ans <<"SA1";
  }
  else {
    ans <<"SA0";
  }
  return ans.str();
}

END_NAMESPACE_YM_SATPG
