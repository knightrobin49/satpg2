#ifndef FSIM_SIMNODE_H
#define FSIM_SIMNODE_H

/// @file SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.

#define USE_VAL3 0

#include "fsim2_nsdef.h"
#include "TpgNode.h"
#include "PackedVal.h"
#include "PackedVal3.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
///
/// 出力値の計算はゲートの種類によって異なるので仮想関数にしている．<br>
/// 注意が必要なのがファンアウトの情報．最初のファンアウトだけ個別のポインタで
/// 持ち，２番目以降のファンアウトは配列で保持する．これは多くのノードが
/// 一つしかファンアウトを持たず，その場合に配列を使うとメモリ参照が余分に発生する
/// ため．
//////////////////////////////////////////////////////////////////////
class SimNode
{
  friend class EventQ;
protected:

  /// @brief コンストラクタ
  SimNode(ymuint id);


public:

  /// @brief デストラクタ
  virtual
  ~SimNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを生成するクラスメソッド
  static
  SimNode*
  new_input(ymuint id);

  /// @brief 論理ノードを生成するクラスメソッド
  static
  SimNode*
  new_gate(ymuint id,
	   GateType type,
	   const vector<SimNode*>& inputs);


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const = 0;

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const = 0;

  /// @brief pos 番めのファンインを得る．
  virtual
  SimNode*
  fanin(ymuint pos) const = 0;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトの先頭のノードを得る．
  SimNode*
  fanout_top() const;

  /// @brief pos 番目のファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() - 1 )
  ///
  /// ただし fanout_top() のノードは含まれない．
  SimNode*
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

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報の設定用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力マークをつける．
  void
  set_output();

  /// @brief ファンアウトリストを作成する．
  void
  set_fanout_list(const vector<SimNode*>& fo_list,
		  ymuint ipos);

  /// @brief FFR の根の印をつける．
  void
  set_ffr_root();


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

  /// @brief 正常値のセットを行う．
  /// @param[in] val 値
  void
  set_gval2(PackedVal val);

  /// @brief 正常値を計算する．
  void
  calc_gval2();

  /// @brief 故障値のイベントをセットする．
  /// @param[in] mask 反転マスク
  void
  flip_fval2(PackedVal mask);

  /// @brief 故障値を計算する．
  /// @param[in] mask マスク
  PackedVal
  calc_fval2(PackedVal mask);

  /// @brief 故障値をクリアする．(2値版)
  void
  clear_fval2();


public:
  //////////////////////////////////////////////////////////////////////
  // 3値の故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値を得る．(3値版)
  PackedVal3
  gval3() const;

  /// @brief 故障値を得る．(3値版)
  PackedVal3
  fval3() const;

  /// @brief 正常値のセットを行う．(3値版)
  /// @param[in] val 値
  void
  set_gval3(PackedVal3 val);

  /// @brief 正常値を計算する．
  void
  calc_gval3();

  /// @brief 故障値をセットする．(3値版)
  /// @param[in] mask 反転マスク
  void
  flip_fval3(PackedVal mask);

  /// @brief 故障値を計算する．
  /// @param[in] mask マスク
  PackedVal
  calc_fval3(PackedVal mask);

  /// @brief 故障値をクリアする．(3値版)
  void
  clear_fval3();


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障値の計算を行う．(2値版)
  /// @return 計算結果を返す．
  virtual
  PackedVal
  _calc_fval2() = 0;

  /// @brief ゲートの入力から出力までの可観測性を計算する．(2値版)
  virtual
  PackedVal
  _calc_gobs2(ymuint ipos) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 3値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障値の計算を行う．(3値版)
  /// @return 計算結果を返す．
  virtual
  PackedVal3
  _calc_fval3() = 0;

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  _calc_gobs3(ymuint ipos) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルを設定する．
  void
  set_level(ymuint level);


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

  // ID 番号
  ymuint mId;

  // ファンアウトリストの要素数
  // その他以下の情報もパックして持つ．
  // - 0      : 出力のマーク
  // - 1      : FFRの根のマーク
  // - 2      : EventQ に入っているかどうかを示すマーク
  // - 4 - 15 : 最初のファンアウトの入力位置(FFR内のノードのみ意味を持つ)
  // - 16 -   : ファンアウト数
  ymuint mFanoutNum;

  // ファンアウトの先頭のノード
  SimNode* mFanoutTop;

  // ファンアウトリスト
  SimNode** mFanouts;

  // レベル
  ymuint mLevel;

  // イベントキューの次の要素
  SimNode* mLink;

#if USE_VAL3
  // 正常値
  PackedVal3 mGval;

  // 故障値
  PackedVal3 mFval;
#else
  PackedVal mGval;
  PackedVal mFval;
#endif

};


//////////////////////////////////////////////////////////////////////
// SimNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief ID番号を返す．
inline
ymuint
SimNode::id() const
{
  return mId;
}

// @brief ファンアウト数を得る．
inline
ymuint
SimNode::fanout_num() const
{
  return mFanoutNum >> 16;
}

// @brief ファンアウトの先頭のノードを得る．
inline
SimNode*
SimNode::fanout_top() const
{
  return mFanoutTop;
}

// @brief pos 番目のファンアウトを得る．
inline
SimNode*
SimNode::fanout(ymuint pos) const
{
  return mFanouts[pos];
}

// @brief 最初のファンアウト先の入力位置を得る．
inline
ymuint
SimNode::fanout_ipos() const
{
  return (mFanoutNum >> 4) & 0x0FFFU;
}

// @brief FFR の根のノードの時 true を返す．
inline
bool
SimNode::is_ffr_root() const
{
  return static_cast<bool>((mFanoutNum >> 1) & 1U);
}

// @brief レベルを得る．
inline
ymuint
SimNode::level() const
{
  return mLevel;
}

// @brief 出力ノードの時 true を返す．
inline
bool
SimNode::is_output() const
{
  return static_cast<bool>((mFanoutNum >> 0) & 1U);
}

// @brief 出力マークをつける．
inline
void
SimNode::set_output()
{
  mFanoutNum |= 1U;
}

// @brief FFR の根の印をつける．
inline
void
SimNode::set_ffr_root()
{
  mFanoutNum |= 2U;
}

// @brief 正常値を得る．
inline
PackedVal
SimNode::gval() const
{
#if USE_VAL3
  return mGval.val1();
#else
  return mGval;
#endif
}

// @brief 故障値を得る．
inline
PackedVal
SimNode::fval() const
{
#if USE_VAL3
  return mFval.val1();
#else
  return mFval;
#endif
}

// @brief 正常値のセットを行う．
// @param[in] val 値
inline
void
SimNode::set_gval2(PackedVal val)
{
  mGval = mFval = val;
}

// @brief 正常値を計算する．
inline
void
SimNode::calc_gval2()
{
  set_gval2(_calc_fval2());
}

// @brief 故障値のイベントをセットする．
// @param[in] mask 反転マスク
inline
void
SimNode::flip_fval2(PackedVal mask)
{
  mFval = (gval() ^ mask);
}

// @brief 故障値を計算する．
inline
PackedVal
SimNode::calc_fval2(PackedVal mask)
{
  PackedVal val = _calc_fval2();
#if USE_VAL3
  mFval.set_with_mask(val, mask);
#else
  mFval &= ~mask;
  mFval |= (val & mask);
#endif
  return gval() ^ fval();
}

// @brief 正常値を得る．(3値版)
inline
PackedVal3
SimNode::gval3() const
{
#if USE_VAL3
  return mGval;
#else
  return PackedVal3(mGval);
#endif
}

// @brief 故障値を得る．(3値版)
inline
PackedVal3
SimNode::fval3() const
{
#if USE_VAL3
  return mFval;
#else
  return PackedVal3(mFval);
#endif
}

// @brief 正常値のセットを行う．(3値版)
// @param[in] val 値
//
// 通常は外部入力に対して行われる．
// 故障値も同様にセットされる．
inline
void
SimNode::set_gval3(PackedVal3 val)
{
#if USE_VAL3
  mGval = mFval = val;
#endif
}

// @brief 正常値を計算する．
inline
void
SimNode::calc_gval3()
{
#if USE_VAL3
  set_gval3(_calc_fval3());
#endif
}

// @brief 故障値をセットする．(3値版)
// @param[in] mask 反転マスク
inline
void
SimNode::flip_fval3(PackedVal mask)
{
#if USE_VAL3
  mFval.set_with_mask(~mGval, mask);
#endif
}

// @brief 故障値を計算する．
// @param[in] mask マスク
inline
PackedVal
SimNode::calc_fval3(PackedVal mask)
{
#if USE_VAL3
  PackedVal3 val = _calc_fval3();
  mFval.set_with_mask(val, mask);
  return diff(mGval, mFval);
#else
  return kPvAll0;
#endif
}

// @brief 故障値をクリアする．
inline
void
SimNode::clear_fval2()
{
  mFval = mGval;
}

// @brief 故障値をクリアする．
inline
void
SimNode::clear_fval3()
{
  mFval = mGval;
}

// @brief キューに積まれていたら true を返す．
inline
bool
SimNode::in_queue() const
{
  return static_cast<bool>((mFanoutNum >> 3) & 1U);
}

// @brief キューフラグをセットする．
inline
void
SimNode::set_queue()
{
  mFanoutNum |= 1U << 3;
}

// @brief キューフラグをクリアする．
inline
void
SimNode::clear_queue()
{
  mFanoutNum &= ~(1U << 3);
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_SIMNODE_H
