#ifndef TPGMAP_H
#define TPGMAP_H

/// @file TpgMap.h
/// @brief TpgMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgMap TpgMap.h "TpgMap.h"
/// @brief BnNode と TpgNode の対応付けを表すクラス
///
/// 基本的には一つのゲートに一つの TpgNode が対応するが，
/// ゲートのタイプが cplx_logic だった場合には複数の TpgNode で構成
/// されることもある．
/// その場合には，個々の入力ごとに対応する TpgNode が異なることになる．
//////////////////////////////////////////////////////////////////////
class TpgMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] inode_array 入力のノードの配列
  /// @param[in] ipos_array 入力のファンイン番号の配列
  TpgMap(TpgNode** inode_array,
	 ymuint* ipos_array);

  /// @brief デストラクタ
  ~TpgMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に対応するノードを得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  TpgNode*
  input(ymuint ipos) const;

  /// @brief 入力に対応するノードのファンイン番号を得る．
  /// @param[in] ipos 入力の位置番号
  virtual
  ymuint
  input_pos(ymuint ipos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のノードの配列
  TpgNode** mInputNodeArray;

  // 入力のファンイン番号の配列
  ymuint* mInputPosArray;

};

END_NAMESPACE_YM_SATPG

#endif // TPGMAP_H
