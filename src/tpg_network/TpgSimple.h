#ifndef TPGSIMPLE_H
#define TPGSIMPLE_H

/// @file TpgSimple.h
/// @brief TpgSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgSimple TpgSimple.h "TpgSimple.h"
/// @brief プリミティブゲートタイプの TpgLogic
//////////////////////////////////////////////////////////////////////
class TpgSimple :
  public TpgLogic
{
public:

  /// @brief コンストラクタ
  TpgSimple();

  /// @brief デストラクタ
  ~TpgSimple();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  virtual
  bool
  is_root() const;

  /// @brief もとのゲートのファンインに対応するノードを返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  virtual
  TpgNode*
  input_map(ymuint pos) const;

  /// @brief BnNode のファンインに対応するノードのファンイン番号を返す．
  /// @param[in] pos もとの BnNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  virtual
  ymuint
  ipos_map(ymuint pos) const;

  /// @brief 内部ノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  virtual
  bool
  is_internal() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_SATPG

#endif // TPGSIMPLE_H
