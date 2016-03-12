#ifndef GVALCNF_H
#define GVALCNF_H

/// @file GvalCnf.h
/// @brief GvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "GenVidMap.h"
#include "TpgNode.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GvalCnf GvalCnf.h "GvalCnf.h"
/// @brief 正常回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class GvalCnf
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SAT ソルバ
  /// @param[in] max_node_id ノード番号の最大値
  GvalCnf(SatSolver& solver,
	  ymuint max_node_id);

  /// @brief デストラクタ
  ~GvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値
  void
  init(ymuint max_node_id);

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief 変数マップを得る．
  const VidMap&
  var_map() const;

  /// @brief 変数番号を得る．
  /// @param[in] node ノード
  SatVarId
  var(const TpgNode* node) const;

  /// @brief ノードのマークを調べる．
  /// @param[in] node ノード
  bool
  mark(const TpgNode* node) const;

  /// @brief ノードにマークをつける．
  /// @param[in] node ノード
  void
  set_mark(const TpgNode* node);

  /// @brief ノードに正常値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] var 変数番号
  void
  set_var(const TpgNode* node,
	  SatVarId var);

  /// @brief 割当リストに従って値を固定する．
  /// @param[in] assignment 割当リスト
  void
  add_assignments(const NodeValList& assignment);

  /// @brief 割当リストの否定の節を加える．
  /// @param[in] assignment 割当リスト
  void
  add_negation(const NodeValList& assignment);

  /// @brief 割当リストに対応する仮定を追加する．
  /// @param[in] assign_list 割当リスト
  /// @param[out] assumptions 仮定を表すリテラルのリスト
  void
  add_assumption(const NodeValList& assign_list,
		 vector<SatLiteral>& assumptions);

  /// @brief NodeSet に含まれるノードの CNF を作る．
  /// @param[in] node_set ノード集合
  void
  make_cnf(const NodeSet& node_set);

  /// @brief node の TFI の CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_cnf(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT ソルバ
  SatSolver& mSolver;

  // ノード番号の最大値
  ymuint mMaxId;

  // 処理済みのノードの印
  vector<bool> mMark;

  // 変数マップ
  GenVidMap mVarMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値を返す．
inline
ymuint
GvalCnf::max_node_id() const
{
  return mMaxId;
}

// @brief 変数マップを得る．
inline
const VidMap&
GvalCnf::var_map() const
{
  return mVarMap;
}

// @brief 変数番号を得る．
// @param[in] node ノード
inline
SatVarId
GvalCnf::var(const TpgNode* node) const
{
  return mVarMap(node);
}

// @brief ノードのマークを調べる．
// @param[in] node ノード
inline
bool
GvalCnf::mark(const TpgNode* node) const
{
  return mMark[node->id()];
}

// @brief ノードにマークをつける．
// @param[in] node ノード
inline
void
GvalCnf::set_mark(const TpgNode* node)
{
  mMark[node->id()] = true;
}

// @brief ノードに正常値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] var 変数番号
inline
void
GvalCnf::set_var(const TpgNode* node,
		 SatVarId var)
{
  mVarMap.set_vid(node, var);
}

END_NAMESPACE_YM_SATPG

#endif // GVALCNF_H
