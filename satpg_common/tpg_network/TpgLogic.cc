
/// @file TpgLogic.cc
/// @brief TpgLogic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgLogic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
TpgLogic::TpgLogic(ymuint id,
		   ymuint fanout_num,
		   TpgNode** fanout_list) :
  TpgNode(id, fanout_num, fanout_list)
{
}

// @brief デストラクタ
TpgLogic::~TpgLogic()
{
}

// @brief logic タイプの時 true を返す．
bool
TpgLogic::is_logic() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogic0
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
TpgLogic0::TpgLogic0(ymuint id,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list)
{
}

// @brief デストラクタ
TpgLogic0::~TpgLogic0()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic0::fanin_num() const
{
  return 0;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic0::fanin(ymuint pos) const
{
  ASSERT_NOT_REACHED;

  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogic1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin ファンイン
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
TpgLogic1::TpgLogic1(ymuint id,
		     TpgNode* fanin,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list),
  mFanin(fanin)
{
}

// @brief デストラクタ
TpgLogic1::~TpgLogic1()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic1::fanin_num() const
{
  return 1;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic1::fanin(ymuint pos) const
{
  ASSERT_COND( pos == 0 );

  return mFanin;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogic2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
//
// fanin_list.size() == 2 であることを仮定している．
TpgLogic2::TpgLogic2(ymuint id,
		     const vector<TpgNode*>& fanin_list,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list)
{
  ASSERT_COND( fanin_list.size() == 2 );

  mFanins[0] = fanin_list[0];
  mFanins[1] = fanin_list[1];
}

// @brief デストラクタ
TpgLogic2::~TpgLogic2()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic2::fanin_num() const
{
  return 2;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic2::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 2 );

  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogic3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
//
// fanin_list.size() == 3 であることを仮定している．
TpgLogic3::TpgLogic3(ymuint id,
		     const vector<TpgNode*>& fanin_list,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list)
{
  ASSERT_COND( fanin_list.size() == 3 );

  mFanins[0] = fanin_list[0];
  mFanins[1] = fanin_list[1];
  mFanins[2] = fanin_list[2];
}

// @brief デストラクタ
TpgLogic3::~TpgLogic3()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic3::fanin_num() const
{
  return 3;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic3::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 3 );

  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogic4
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
//
// fanin_list.size() == 4 であることを仮定している．
TpgLogic4::TpgLogic4(ymuint id,
		     const vector<TpgNode*>& fanin_list,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list)
{
  ASSERT_COND( fanin_list.size() == 4 );

  mFanins[0] = fanin_list[0];
  mFanins[1] = fanin_list[1];
  mFanins[2] = fanin_list[2];
  mFanins[3] = fanin_list[3];
}

// @brief デストラクタ
TpgLogic4::~TpgLogic4()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogic4::fanin_num() const
{
  return 4;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogic4::fanin(ymuint pos) const
{
  ASSERT_COND( pos < 4 );

  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス TpgLogicN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_num ファンイン数
// @param[in] fanin_list ファンインのリストを表す配列
// @param[in] fanout_num ファンアウト数
// @param[in] fanout_list ファンアウトのリストを格納する配列
TpgLogicN::TpgLogicN(ymuint id,
		     ymuint fanin_num,
		     TpgNode** fanin_list,
		     ymuint fanout_num,
		     TpgNode** fanout_list) :
  TpgLogic(id, fanout_num, fanout_list),
  mFaninNum(fanin_num),
  mFanins(fanin_list)
{
}

// @brief デストラクタ
TpgLogicN::~TpgLogicN()
{
}

// @brief ファンイン数を得る．
ymuint
TpgLogicN::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgLogicN::fanin(ymuint pos) const
{
  ASSERT_COND( pos < fanin_num() );

  return mFanins[pos];
}

END_NAMESPACE_YM_SATPG
