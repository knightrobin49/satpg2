#ifndef TPGFAULT_H
#define TPGFAULT_H

/// @file TpgFault.h
/// @brief TpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2014 Yusuke Matsunaga
/// All rights reserved.

#include "satpg.h"
#include "Val3.h"

#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgFault TpgFault.h "TpgFault.h"
/// @brief 単一縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgFault
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] val 故障値
  /// @param[in] rep_fault 代表故障
  TpgFault(ymuint id,
	   int val,
	   const TpgFault* rep_fault);

  /// @brief デストラクタ
  virtual
  ~TpgFault();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 故障位置に対応する TpgNode を返す．
  virtual
  const TpgNode*
  tpg_node() const = 0;

  /// @brief 故障の入力側の TpgNode を返す．
  ///
  /// 出力の故障の場合には tpg_node() と同じ値を返す．
  virtual
  const TpgNode*
  tpg_inode() const = 0;

  /// @brief 入力の故障の時 true を返す．
  bool
  is_input_fault() const;

  /// @brief 出力の故障の時 true を返す．
  virtual
  bool
  is_output_fault() const = 0;

  /// @brief 故障位置を返す．
  ///
  /// is_input_fault() == true の時のみ意味を持つ．
  virtual
  ymuint
  pos() const = 0;

  /// @brief tpg_inode 上の故障位置を返す．
  ///
  /// is_input_fault() == true の時のみ意味を持つ．
  virtual
  ymuint
  tpg_pos() const = 0;

  /// @brief 故障値を返す．
  /// @note 返す値は 0 か 1
  int
  val() const;

  /// @brief 故障値を3値型で返す．
  Val3
  val3() const;

  /// @brief 故障の内容を表す文字列を返す．
  virtual
  string
  str() const = 0;

  /// @brief 代表故障の時 true を返す．
  bool
  is_rep() const;

  /// @brief 代表故障を返す．
  /// @note 代表故障の時は自分自身を返す．
  const TpgFault*
  rep_fault() const;

  /// @brief 支配故障のリストを返す．
  const vector<const TpgFault*>&
  dom_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 微妙な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障の支配関係を設定する．
  /// @param[in] dom_f 支配する故障
  void
  set_dominance(const TpgFault* dom_f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 故障値
  ymuint8 mVal;

  // 代表故障
  const TpgFault* mRepFault;

  // 支配故障のリスト
  vector<const TpgFault*> mDomList;

};

/// @relates TpgFault
/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const TpgFault* f);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を返す．
inline
ymuint
TpgFault::id() const
{
  return mId;
}

// @brief 入力の故障の時 true を返す．
inline
bool
TpgFault::is_input_fault() const
{
  return !is_output_fault();
}

// @brief 故障値を返す．
// @note 返す値は 0 か 1
inline
int
TpgFault::val() const
{
  return mVal;
}

// @brief 故障値を3値型で返す．
inline
Val3
TpgFault::val3() const
{
  if ( val() ) {
    return kVal1;
  }
  else {
    return kVal0;
  }
}

// @brief 代表故障の時 true を返す．
inline
bool
TpgFault::is_rep() const
{
  return mRepFault == this;
}

// @brief 代表故障を返す．
// @note 代表故障の時は自分自身を返す．
inline
const TpgFault*
TpgFault::rep_fault() const
{
  return mRepFault;
}

// @brief 支配故障のリストを返す．
inline
const vector<const TpgFault*>&
TpgFault::dom_list() const
{
  return mDomList;
}

// @brief 故障の支配関係を設定する．
// @param[in] dom_f 支配する故障
inline
void
TpgFault::set_dominance(const TpgFault* dom_f)
{
  mDomList.push_back(dom_f);
}

END_NAMESPACE_YM_SATPG

BEGIN_NAMESPACE_YM
// TpgFault へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<nsSatpg::TpgFault*>
{
  ymuint
  operator()(nsSatpg::TpgFault* fault) const
  {
    return reinterpret_cast<ympuint>(fault)/sizeof(void*);
  }
};
END_NAMESPACE_YM

#endif // TPGFAULT_H
