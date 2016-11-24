#ifndef MODELVALMAP2_H
#define MODELVALMAP2_H

/// @file ModelValMap2.h
/// @brief ModelValMap2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ValMap2.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class ModelValMap2 ModelValMap2.h "ModelValMap2.h"
/// @brief SAT ソルバの model 配列から値を読みだす ValMap
//////////////////////////////////////////////////////////////////////
class ModelValMap2 :
  public ValMap2
{
public:

  /// @brief コンストラクタ
  /// @param[in] hvar_map 1時刻前の正常値の変数マップ
  /// @param[in] gvar_map 正常値の変数マップ
  /// @param[in] fvar_map 故障値の変数マップ
  /// @param[in] model SATソルバの作ったモデル
  ModelValMap2(const VidMap& hvar_map,
	       const VidMap& gvar_map,
	       const VidMap& fvar_map,
	       const vector<SatBool3>& model);

  /// @brief デストラクタ
  virtual
  ~ModelValMap2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの1時刻前の正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  hval(const TpgNode* node) const;

  /// @brief ノードの正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  gval(const TpgNode* node) const;

  /// @brief ノードの故障値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  fval(const TpgNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1時刻前の正常値の変数マップ
  const VidMap& mHvarMap;

  // 正常値の変数マップ
  const VidMap& mGvarMap;

  // 故障値の変数マップ
  const VidMap& mFvarMap;

  // モデル
  const vector<SatBool3>& mModel;

};

END_NAMESPACE_YM_SATPG

#endif // GVALMODELMAP2_H
