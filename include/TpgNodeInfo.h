#ifndef TPGNODEINFO_H
#define TPGNODEINFO_H

/// @file TpgNodeInfo.h
/// @brief TpgNodeInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "Val3.h"
#include "ym/ym_logic.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgNodeInfo TpgNodeInfo.h "TpgNodeInfo.h"
/// @brief TpgNode の論理関数の情報を格納するクラス
///
/// - 追加ノード数
/// - 制御値
/// の情報を持つ．
//////////////////////////////////////////////////////////////////////
class TpgNodeInfo
{
public:

  /// @brief デストラクタ
  virtual
  ~TpgNodeInfo() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートタイプを返す．
  virtual
  GateType
  gate_type() const = 0;

  /// @brief 論理式を返す．
  virtual
  Expr
  expr() const = 0;

  /// @brief 追加ノード数を返す．
  virtual
  ymuint
  extra_node_num() const = 0;

  /// @brief 制御値を返す．
  /// @param[in] pos 入力位置
  /// @param[in] val 値
  virtual
  Val3
  cval(ymuint pos,
       Val3 val) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class TpgNodeInfoMgr TpgNodeInfoMgr.h "TpgNodeInfoMgr.h"
/// @brief TpgNodeInfo を管理するクラス
//////////////////////////////////////////////////////////////////////
class TpgNodeInfoMgr
{
public:

  /// @brief コンストラクタ
  TpgNodeInfoMgr();

  /// @brief デストラクタ
  ~TpgNodeInfoMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型のオブジェクトを返す．
  /// @param[in] gate_type ゲートタイプ
  const TpgNodeInfo*
  simple_type(GateType gate_type);

  /// @brief 複合型のオブジェクトを返す．
  /// @param[in] id ID番号
  /// @param[in] ni 入力数
  /// @param[in] expr 論理式
  const TpgNodeInfo*
  complex_type(ymuint id,
	       ymuint ni,
	       const Expr& expr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 組み込み型のオブジェクトの配列
  TpgNodeInfo* mSimpleType[10];

  // 複合型のオブジェクトのハッシュ表
  HashMap<ymuint, TpgNodeInfo*> mCplxTypeMap;

};

END_NAMESPACE_YM_SATPG

#endif // TPGNODEINFO_H
