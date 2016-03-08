#ifndef TPGINPUTFAULT_H
#define TPGINPUTFAULT_H

/// @file TpgInputFault.h
/// @brief TpgInputFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2014 Yusuke Matsunaga
/// All rights reserved.

#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgInputFault TpgInputFault.h "TpgInputFault.h"
/// @brief 入力の故障を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgInputFault :
  public TpgFault
{
  friend class TpgNetwork;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node_name ノード名
  /// @param[in] tpgnode 故障位置の TpgNode
  /// @param[in] pos 故障の入力位置
  /// @param[in] i_tpgnode 入力側の TpgNode
  /// @param[in] tpg_pos i_tpgnode 上の故障位置
  /// @param[in] val 故障値
  /// @param[in] rep_fault 代表故障
  TpgInputFault(ymuint id,
		const char* node_name,
		const TpgNode* tpgnode,
		ymuint pos,
		const TpgNode* i_tpgnode,
		ymuint tpg_pos,
		int val,
		const TpgFault* rep_fault);

  /// @brief デストラクタ
  virtual
  ~TpgInputFault();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief node() に対応する TpgNode を返す．
  virtual
  const TpgNode*
  tpg_node() const;

  /// @brief 故障の入力側の TpgNode を返す．
  ///
  /// 出力の故障の場合には tpg_node() と同じ値を返す．
  virtual
  const TpgNode*
  tpg_inode() const;

  /// @brief 出力の故障の時 true を返す．
  virtual
  bool
  is_output_fault() const;

  /// @brief 故障位置を返す．
  ///
  /// is_input_fault() == true の時のみ意味を持つ．
  virtual
  ymuint
  pos() const;

  /// @brief tpg_inode 上の故障位置を返す．
  ///
  /// is_input_fault() == true の時のみ意味を持つ．
  virtual
  ymuint
  tpg_pos() const;

  /// @brief 故障の内容を表す文字列を返す．
  virtual
  string
  str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード名
  const char* mNodeName;

  // 対象の TpgNode
  const TpgNode* mTpgNode;

  // 故障の入力位置
  ymuint mPos;

  // 入力側の TpgNode
  const TpgNode* mI_TpgNode;

  // mI_TpgNode 上の入力位置
  ymuint mTpgPos;

};

END_NAMESPACE_YM_SATPG

#endif // TPGINPUTFAULT_H
