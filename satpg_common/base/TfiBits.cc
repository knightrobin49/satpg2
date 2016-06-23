
/// @file TfiBits.cc
/// @brief TfiBits の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TfiBits.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス TfiBits
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_id ノード番号の最大値
// @param[in] output_num 出力数
TfiBits::TfiBits(ymuint max_id,
		 ymuint output_num)
{
  mBlockSize = (output_num + 63) / 64;
  mBitVect = new ymuint64[mBlockSize * max_id];
}

// @brief デストラクタ
TfiBits::~TfiBits()
{
  delete [] mBitVect;
}

// @brief node の opos ビットをセットする．
// @param[in] node ノード
// @param[in] opos 出力番号
void
TfiBits::set_bit(const TpgNode* node,
		 ymuint opos)
{
  ymuint blk = opos / 64;
  ymuint sft = opos % 64;
  mBitVect[(node->id() * mBlockSize) + blk] |= (1ULL << sft);
}

// @brief node1 のビットベクタに node2 のビットベクタの OR を加える．
// @param[in] node1 設定対象のノード
// @param[in] node2 参照元のノード
void
TfiBits::or_bits(const TpgNode* node1,
		 const TpgNode* node2)
{
  ymuint base1 = node1->id() * mBlockSize;
  ymuint base2 = node2->id() * mBlockSize;
  for (ymuint i = 0; i < mBlockSize; ++ i) {
    mBitVect[base1 + i] |= mBitVect[base2 + i];
  }
}

// @brief node が opos の TFI に含まれているか調べる．
// @param[in] node ノード
// @param[in] opos 出力番号
bool
TfiBits::check(const TpgNode* node,
	       ymuint opos) const
{
  ymuint blk = opos / 64;
  ymuint sft = opos % 64;
  return static_cast<bool>((mBitVect[(node->id() * mBlockSize) + blk] >> sft) & 1UL);
}

END_NAMESPACE_YM_SATPG
