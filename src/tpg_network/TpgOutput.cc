
/// @file TpgOutput.cc
/// @brief TpgOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOutput.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TpgOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] output_id 出力番号
// @param[in] fanin ファンインのノード
TpgOutput::TpgOutput(ymuint id,
		     ymuint output_id,
		     TpgNode* fanin) :
  TpgNode(id),
  mOutputId(output_id),
  mFanin(fanin)
{
}

// @brief デストラクタ
TpgOutput::~TpgOutput()
{
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
bool
TpgOutput::is_output() const
{
  return true;
}

// @brief 外部出力タイプの時に出力番号を返す．
//
// node = TpgNetwork::output(node->output_id())
// の関係を満たす．
// is_output() が false の場合の返り値は不定
ymuint
TpgOutput::output_id() const
{
  return mOutputId;
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
ymuint
TpgOutput::output_id2() const
{
  return mOutputId2;
}

// @brief ファンイン数を得る．
ymuint
TpgOutput::fanin_num() const
{
  return 1;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
TpgNode*
TpgOutput::fanin(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mFanin;
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgOutput::input_fault(int val,
		       ymuint pos) const
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( pos == 0 );
  return mFaults[val];
}

// @brief このノードに関係する故障数を返す．
ymuint
TpgOutput::fault_num() const
{
  return 2;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgOutput::fault(ymuint pos) const
{
  ASSERT_COND( pos < fault_num() );
  return mFaults[pos];
}

END_NAMESPACE_YM_SATPG
