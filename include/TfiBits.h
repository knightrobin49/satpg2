#ifndef TFIBITS_H
#define TFIBITS_H

/// @file TfiBits.h
/// @brief TfiBits のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TfiBits TfiBits.h "TfiBits.h"
/// @brief 各ノードがどの PO の TFI に含まれているかの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class TfiBits
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_id ノード番号の最大値
  /// @param[in] output_num 出力数
  TfiBits(ymuint max_id,
	  ymuint output_num);

  /// @brief デストラクタ
  ~TfiBits();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief node の opos ビットをセットする．
  /// @param[in] node ノード
  /// @param[in] opos 出力番号
  void
  set_bit(const TpgNode* node,
	  ymuint opos);

  /// @brief node1 のビットベクタに node2 のビットベクタの OR を加える．
  /// @param[in] node1 設定対象のノード
  /// @param[in] node2 参照元のノード
  void
  or_bits(const TpgNode* node1,
	  const TpgNode* node2);

  /// @brief node が opos の TFI に含まれているか調べる．
  /// @param[in] node ノード
  /// @param[in] opos 出力番号
  bool
  check(const TpgNode* node,
	ymuint opos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ブロックサイズ
  ymuint mBlockSize;

  // ビットベクタ
  // サイズは max_id * mBlockSize
  ymuint64* mBitVect;

};

END_NAMESPACE_YM_SATPG

#endif // TFIBITS_H
