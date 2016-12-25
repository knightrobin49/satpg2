#ifndef FSIM_FVALNODE_H
#define FSIM_FVALNODE_H

/// @file FvalNode.h
/// @brief FvalNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"
#include "SimPrim.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class FvalNode FvalNode.h "FvalNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class FvalNode
{
  friend class EventQ;
public:

  /// @brief コンストラクタ
  FvalNode();

  /// @brief デストラクタ
  ~FvalNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief pos 番目のファンアウトを得る．
  FvalNode*
  fanout(ymuint pos) const;

  /// @brief 最初のファンアウト先の入力位置を得る．
  ymuint
  fanout_ipos() const;

  /// @brief FFR の根のノードの時 true を返す．
  bool
  is_ffr_root() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief 出力ノードの時 true を返す．
  bool
  is_output() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の設定用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力マークをつける．
  void
  set_output();

  /// @brief ファンアウトリストを作成する．
  void
  set_fanout_list(const vector<FvalNode*>& fo_list,
		  ymuint ipos);

  /// @brief FFR の根の印をつける．
  void
  set_ffr_root();

  /// @brief レベルを設定する．
  void
  set_level(ymuint level);


public:
  //////////////////////////////////////////////////////////////////////
  // 2値の故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値を得る．
  PackedVal
  gval() const;

  /// @brief 故障値を得る．
  PackedVal
  fval() const;

  /// @brief 故障値のセットを行う．
  /// @param[in] val 値
  void
  set_fval(PackedVal val);

  /// @brief 故障値のイベントをセットする．
  /// @param[in] mask 反転マスク
  void
  flip_fval(PackedVal mask);

  /// @brief 故障値を計算する．
  /// @param[in] mask マスク
  PackedVal
  calc_fval(PackedVal mask);

  /// @brief 故障値をクリアする．(2値版)
  void
  clear_fval();


private:
  //////////////////////////////////////////////////////////////////////
  // EventQ 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キューに積まれていたら true を返す．
  bool
  in_queue() const;

  /// @brief キューフラグをセットする．
  void
  set_queue();

  /// @brief キューフラグをクリアする．
  void
  clear_queue();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 正常値計算用のノード
  SimPrim* mGval;

  // 故障値計算用のノード
  SimPrim* mFval;

  // ファンアウトリストの要素数
  // その他以下の情報もパックして持つ．
  // - EventQ に入っているかどうかを示すマーク
  // - 最初のファンアウトの入力位置(FFR内のノードのみ意味を持つ)
  // - 出力のマーク
  // - lobs の計算マーク
  ymuint mFanoutNum;

  // ファンアウトリスト
  FvalNode** mFanouts;

  // レベル
  ymuint mLevel;

  // イベントキューの次の要素
  FvalNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// FvalNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 正常値を得る．
inline
PackedVal
FvalNode::gval() const
{
  return mGval->val();
}

// @brief 故障値を得る．
inline
PackedVal
FvalNode::fval() const
{
  return mFval->val();
}

// @brief 故障値のセットを行う．
// @param[in] val 値
inline
void
FvalNode::set_fval(PackedVal val)
{
  mFval->set_val(val);
}

// @brief 故障値のイベントをセットする．
// @param[in] mask 反転マスク
inline
void
FvalNode::flip_fval(PackedVal mask)
{
  set_fval(gval() ^ mask);
}

// @brief 故障値を計算する．
// @param[in] mask マスク
inline
PackedVal
FvalNode::calc_fval(PackedVal mask)
{
  PackedVal val = mFval->_calc_val();
  PackedVal dval = (val ^ mFval->val()) & mask;
  set_fval(fval() ^ dval);
  return dval;
}

// @brief 故障値をクリアする．(2値版)
inline
void
FvalNode::clear_fval()
{
  set_fval(gval());
}

// @brief ファンアウト数を得る．
inline
ymuint
FvalNode::fanout_num() const
{
  return mFanoutNum >> 16;
}

// @brief pos 番目のファンアウトを得る．
inline
FvalNode*
FvalNode::fanout(ymuint pos) const
{
  return mFanouts[pos];
}

// @brief 最初のファンアウト先の入力位置を得る．
inline
ymuint
FvalNode::fanout_ipos() const
{
  return (mFanoutNum >> 4) & 0x0FFFU;
}

// @brief FFR の根のノードの時 true を返す．
inline
bool
FvalNode::is_ffr_root() const
{
  return static_cast<bool>((mFanoutNum >> 1) & 1U);
}

// @brief レベルを得る．
inline
ymuint
FvalNode::level() const
{
  return mLevel;
}

// @brief 出力ノードの時 true を返す．
inline
bool
FvalNode::is_output() const
{
  return (mFanoutNum & 1U) == 1U;
}

// @brief 出力マークをつける．
inline
void
FvalNode::set_output()
{
  mFanoutNum |= 1U;
}

// @brief FFR の根の印をつける．
inline
void
FvalNode::set_ffr_root()
{
  mFanoutNum |= 2U;
}

// @brief キューに積まれていたら true を返す．
inline
bool
FvalNode::in_queue() const
{
  return static_cast<bool>((mFanoutNum >> 3) & 1U);
}

// @brief キューフラグをセットする．
inline
void
FvalNode::set_queue()
{
  mFanoutNum |= 1U << 3;
}

// @brief キューフラグをクリアする．
inline
void
FvalNode::clear_queue()
{
  mFanoutNum &= ~(1U << 3);
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_FVALNODE_H
