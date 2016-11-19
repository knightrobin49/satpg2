
/// @file ModelValMap2.cc
/// @brief ModelValMap2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ModelValMap2.h"
#include "VidMap.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief SatBool3 から Val3 への変換
inline
Val3
bool3_to_val3(SatBool3 bval)
{
  switch ( bval ) {
  case kB3True:  return kVal1;
  case kB3False: return kVal0;
  case kB3X:     return kValX;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return kValX;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ModelValMap2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] hvar_map 1時刻前の正常値の変数マップ
// @param[in] gvar_map 正常値の変数マップ
// @param[in] fvar_map 故障値の変数マップ
// @param[in] model SATソルバの作ったモデル
ModelValMap2::ModelValMap2(const VidMap& hvar_map,
			   const VidMap& gvar_map,
			   const VidMap& fvar_map,
			   const vector<SatBool3>& model) :
  mHvarMap(hvar_map),
  mGvarMap(gvar_map),
  mFvarMap(fvar_map),
  mModel(model)
{
}

// @brief デストラクタ
ModelValMap2::~ModelValMap2()
{
}

// @brief ノードの1時刻前の正常値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap2::hval(const TpgNode* node) const
{
  return bool3_to_val3(mModel[mHvarMap(node).val()]);
}

// @brief ノードの正常値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap2::gval(const TpgNode* node) const
{
  return bool3_to_val3(mModel[mGvarMap(node).val()]);
}

// @brief ノードの故障値を返す．
// @param[in] node 対象のノード
Val3
ModelValMap2::fval(const TpgNode* node) const
{
  return bool3_to_val3(mModel[mFvarMap(node).val()]);
}

END_NAMESPACE_YM_SATPG
