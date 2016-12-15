#ifndef FSIM_SIMNODE_H
#define FSIM_SIMNODE_H

/// @file SimNode.h
/// @brief SimNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"
#include "TpgNode.h"
#include "EqElem.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

class SimFFR;

//////////////////////////////////////////////////////////////////////
/// @class SimNode SimNode.h "SimNode.h"
/// @brief 故障シミュレーション用のノード
//////////////////////////////////////////////////////////////////////
class SimNode :
  public EqElem
{
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

  /// @brief pos 番目のファンアウトを得る．
  SimNode*
  fanout(ymuint pos) const;

  /// @brief 最初のファンアウト先の入力位置を得る．
  ymuint
  fanout_ipos() const;

  /// @brief FFR を得る．
  SimFFR*
  ffr() const;

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

  /// @brief FFR を設定する．
  void
  set_ffr(SimFFR* ffr);


public:
  //////////////////////////////////////////////////////////////////////
  // 2値の故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値のセットを行う．
  /// @param[in] pat 値
  ///
  /// - 通常は外部入力に対して行われる．
  /// - 故障値も同様にセットされる．
  void
  set_gval(PackedVal pat);

  /// @brief 正常値を得る．
  PackedVal
  gval() const;

  /// @brief 故障値をセットする．
  void
  set_fval(PackedVal pat);

  /// @brief 故障値を得る．
  PackedVal
  fval() const;

  /// @brief 正常値の計算を行う．(2値版)
  /// @note 結果は mGval にセットされる．
  void
  calc_gval2();

  /// @brief 故障値の計算を行う．(2値版)
  /// @param[in] mask マスク
  /// @return 故障差を返す．
  ///
  /// - mask 中で1の立っているビットのみ計算する．
  /// - 結果は mFval にセットされる．
  PackedVal
  calc_fval2(PackedVal mask);

  /// @brief 故障値をクリアする．(2値版)
  void
  clear_fval2();

  /// @brief FFRの根までの obs の計算を行う．(2値版)
  PackedVal
  calc_lobs2();


public:
  //////////////////////////////////////////////////////////////////////
  // 3値の故障シミュレーションに関する情報の取得/設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値のセットを行う．(3値版)
  /// @param[in] val_0, val_1 値
  ///
  /// 通常は外部入力に対して行われる．
  /// 故障値も同様にセットされる．
  void
  set_gval(PackedVal val_0,
	   PackedVal val_1);

  /// @brief 正常値の 0 パタンを得る．
  PackedVal
  gval_0() const;

  /// @brief 正常値の 1 パタンを得る．
  PackedVal
  gval_1() const;

  /// @brief 故障値をセットする．(3値版)
  /// @param[in] val_0, val_1 値
  /// @param[in] mask ビットマスク
  ///
  /// mask が0のビットはセットしない．
  void
  set_fval(PackedVal val_0,
	   PackedVal val_1,
	   PackedVal mask = kPvAll1);

  /// @brief 故障値の 0 パタンを得る．
  PackedVal
  fval_0() const;

  /// @brief 故障値の 1 パタンを得る．
  PackedVal
  fval_1() const;

  /// @brief 正常値の計算を行う．(3値版)
  /// @return 結果が X でなければ true を返す．
  ///
  /// 結果は mGval にセットされる．
  bool
  calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @return 故障差を返す．
  ///
  /// 結果は mFval にセットされる．
  PackedVal
  calc_fval3(PackedVal mask);

  /// @brief 故障値をクリアする．(3値版)
  void
  clear_fval3();

  /// @brief ローカルな obs の計算を行う．(3値版)
  PackedVal
  calc_lobs3();


public:
  //////////////////////////////////////////////////////////////////////
  // lobs の計算用の作業領域にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief lobs が計算済みかチェックする．
  bool
  check_lobs() const;

  /// @brief lobs が計算済みの印をつける．
  void
  set_lobs();

  /// @brief lobs の計算済みの印を消す．
  void
  clear_lobs();


public:
  //////////////////////////////////////////////////////////////////////
  // 2値版の故障シミュレーション用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常値の計算を行う．(2値版)
  virtual
  PackedVal
  _calc_gval2() = 0;

  /// @brief 故障値の計算を行う．(2値版)
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

  /// @brief 正常値の計算を行う．(3値版)
  ///
  /// 結果は mGval[0], mGval[1] に格納される．
  virtual
  void
  _calc_gval3() = 0;

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  ///
  /// 結果は mFval[0], mFval[1] に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask) = 0;

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint mId;

  // ファンアウトリストの要素数
  // その他以下の情報もパックして持つ．
  // - 最初のファンアウトの入力位置(FFR内のノードのみ意味を持つ)
  // - 出力のマーク
  // - lobs の計算マーク
  ymuint mFanoutNum;

  // ファンアウトリスト
  SimNode** mFanouts;

  // FFR
  SimFFR* mFFR;

  // レベル
  ymuint mLevel;

  // 正常値
  PackedVal mGval[2];

  // 故障値
  PackedVal mFval[2];

  // FFR 内のローカルな obs
  PackedVal mLobs;

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
  return (mFanoutNum >> 2) & 0x3FFFU;
}

// @brief FFR を得る．
inline
SimFFR*
SimNode::ffr() const
{
  return mFFR;
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
  return (mFanoutNum & 1U) == 1U;
}

// @brief 出力マークをつける．
inline
void
SimNode::set_output()
{
  mFanoutNum |= 1U;
}

// @brief FFR を設定する．
inline
void
SimNode::set_ffr(SimFFR* ffr)
{
  mFFR = ffr;
}

// @brief lobs が計算済みかチェックする．
inline
bool
SimNode::check_lobs() const
{
  return ((mFanoutNum >> 1) & 1U) == 1U;
}

// @brief lobs が計算済みの印をつける．
inline
void
SimNode::set_lobs()
{
  mFanoutNum |= (1U << 1);
}

// @brief lobs の計算済みの印を消す．
inline
void
SimNode::clear_lobs()
{
  mFanoutNum &= ~(1U << 1);
}

// @brief 正常値のセットを行う．
// @param[in] val 値
// @note 通常は外部入力に対して行われる．
// @note 故障値も同様にセットされる．
inline
void
SimNode::set_gval(PackedVal val)
{
  mGval[0] = val;
  mFval[0] = val;
}

// @brief 正常値を得る．
inline
PackedVal
SimNode::gval() const
{
  return mGval[0];
}

// @brief 故障値をセットする．
inline
void
SimNode::set_fval(PackedVal pat)
{
  mFval[0] = pat;
}

// @brief 故障値を得る．
inline
PackedVal
SimNode::fval() const
{
  return mFval[0];
}

// @brief 正常値のセットを行う．(3値版)
// @param[in] val_0, val_1 値
//
// 通常は外部入力に対して行われる．
// 故障値も同様にセットされる．
inline
void
SimNode::set_gval(PackedVal val_0,
		  PackedVal val_1)
{
  mGval[0] = mFval[0] = val_0;
  mGval[1] = mFval[1] = val_1;
}

// @brief 正常値の 0 パタンを得る．
inline
PackedVal
SimNode::gval_0() const
{
  return mGval[0];
}

// @brief 正常値の 1 パタンを得る．
inline
PackedVal
SimNode::gval_1() const
{
  return mGval[1];
}

// @brief 故障値をセットする．(3値版)
// @param[in] val_0, val_1 値
// @param[in] mask ビットマスク
//
// mask が0のビットはセットしない．
inline
void
SimNode::set_fval(PackedVal val_0,
		  PackedVal val_1,
		  PackedVal mask)
{
  mFval[0] &= ~mask;
  mFval[0] |= val_0 & mask;
  mFval[1] &= ~mask;
  mFval[1] |= val_1 & mask;
}

// @brief 故障値の 0 パタンを得る．
inline
PackedVal
SimNode::fval_0() const
{
  return mFval[0];
}

// @brief 故障値の 1 パタンを得る．
inline
PackedVal
SimNode::fval_1() const
{
  return mFval[1];
}

// @brief 正常値の計算を行う．
// @note 結果は mGval にセットされる．
inline
void
SimNode::calc_gval2()
{
  set_gval(_calc_gval2());
}

// @brief 故障値の計算を行う．
// @return 故障差を返す．
// @note 結果は mFval にセットされる．
inline
PackedVal
SimNode::calc_fval2(PackedVal mask)
{
  PackedVal val = _calc_fval2();
  PackedVal diff = (mGval[0] ^ val) & mask;
  mFval[0] ^= diff;
  return diff;
}

// @brief 故障値をクリアする．
inline
void
SimNode::clear_fval2()
{
  mFval[0] = mGval[0];
}

// @brief 正常値の計算を行う．
// @return 結果が X でなければ true を返す．
//
// 結果は mGval にセットされる．
inline
bool
SimNode::calc_gval3()
{
  _calc_gval3();
  mFval[0] = mGval[0];
  mFval[1] = mGval[1];
  return (mGval[0] | mGval[1]) != kPvAll0;
}

// @brief 故障値の計算を行う．
// @return 故障差を返す．
//
// 結果は mFval にセットされる．
inline
PackedVal
SimNode::calc_fval3(PackedVal mask)
{
  _calc_fval3(mask);
  return (mGval[0] ^ mFval[0]) | (mGval[1] ^ mFval[1]);
}

// @brief 故障値をクリアする．
inline
void
SimNode::clear_fval3()
{
  mFval[0] = mGval[0];
  mFval[1] = mGval[1];
}

END_NAMESPACE_YM_SATPG_FSIM

#endif // FSIM_SIMNODE_H
