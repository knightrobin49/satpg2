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

class FoCone;

//////////////////////////////////////////////////////////////////////
/// @class GvalCnf GvalCnf.h "GvalCnf.h"
/// @brief 正常回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class GvalCnf
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  GvalCnf(ymuint max_node_id,
	  const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp);

  /// @brief デストラクタ
  ~GvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SATソルバを返す．
  SatSolver&
  solver();

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

  /// @brief ノードに正常値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] var 変数番号
  void
  set_var(const TpgNode* node,
	  SatVarId var);

  /// @brief 故障の検出条件を割当リストに追加する．
  /// @param[in] fault 故障
  /// @param[out] assignment 割当リスト
  void
  add_fault_condition(const TpgFault* fault,
		      NodeValList& assignment);

  /// @brief FFR内の故障の伝搬条件を割当リストに追加する．
  /// @param[in] root_node FFRの根のノード
  /// @param[in] fault 故障
  /// @param[out] assignment 割当リスト
  void
  add_ffr_condition(const TpgNode* root_node,
		    const TpgFault* fault,
		    NodeValList& assignment);

  /// @brief 割当リストに従って値を固定する．
  /// @param[in] assignment 割当リスト
  void
  add_assignments(const NodeValList& assignment);

  /// @brief 割当リストの否定の節を加える．
  /// @param[in] assignment 割当リスト
  void
  add_negation(const NodeValList& assignment);

  /// @brief 割当リストを仮定のリテラルに変換する．
  /// @param[in] assign_list 割当リスト
  /// @param[out] assumptions 仮定を表すリテラルのリスト
  ///
  /// 必要に応じて使われているリテラルに関するCNFを追加する．
  void
  conv_to_assumption(const NodeValList& assign_list,
		     vector<SatLiteral>& assumptions);

  /// @brief NodeSet に含まれるノードの CNF を作る．
  /// @param[in] node_set ノード集合
  void
  make_cnf(const NodeSet& node_set);

  /// @brief FoCone に含まれるノードの CNF を作る．
  /// @param[in] focone ノード集合
  void
  make_cnf(const FoCone& focone);

  /// @brief node の TFI の CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_cnf(const TpgNode* node);

  /// @brief チェックを行う．
  /// @param[out] sat_model SATの場合の解
  SatBool3
  check_sat(vector<SatBool3>& sat_model);

  /// @brief チェックを行う．
  ///
  /// こちらは結果のみを返す．
  SatBool3
  check_sat();

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] assign_list 割当リスト
  /// @param[out] sat_model SATの場合の解
  SatBool3
  check_sat(const NodeValList& assign_list,
	    vector<SatBool3>& sat_model);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] assign_list 割当リスト
  ///
  /// こちらは結果のみを返す．
  SatBool3
  check_sat(const NodeValList& assign_list);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] assign_list1, assign_list2 割当リスト
  /// @param[out] sat_model SATの場合の解
  SatBool3
  check_sat(const NodeValList& assign_list1,
	    const NodeValList& assign_list2,
	    vector<SatBool3>& sat_model);

  /// @brief 割当リストのもとでチェックを行う．
  /// @param[in] assign_list1, assign_list2 割当リスト
  /// @param[in] assign_list1, assign_list2 割当リスト
  ///
  /// こちらは結果のみを返す．
  SatBool3
  check_sat(const NodeValList& assign_list1,
	    const NodeValList& assign_list2);

  /// @brief デバッグ用のフラグをセットする．
  void
  set_debug(ymuint bits);

  /// @brief デバッグ用のフラグを得る．
  ymuint
  debug() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのマークを調べる．
  /// @param[in] node ノード
  bool
  mark(const TpgNode* node) const;

  /// @brief ノードにマークをつける．
  /// @param[in] node ノード
  void
  set_mark(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT ソルバ
  SatSolver mSolver;

  // ノード番号の最大値
  ymuint mMaxId;

  // 処理済みのノードの印
  vector<bool> mMark;

  // 変数マップ
  GenVidMap mVarMap;

  // デバッグ用のフラグ
  ymuint mDebugFlag;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバを返す．
inline
SatSolver&
GvalCnf::solver()
{
  return mSolver;
}

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

// @brief チェックを行う．
//
// こちらは結果のみを返す．
inline
SatBool3
GvalCnf::check_sat()
{
  vector<SatBool3> model;
  return check_sat(model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] assign_list 割当リスト
//
// こちらは結果のみを返す．
inline
SatBool3
GvalCnf::check_sat(const NodeValList& assign_list)
{
  vector<SatBool3> model;
  return check_sat(assign_list, model);
}

// @brief 割当リストのもとでチェックを行う．
// @param[in] gval_cnf 正常回路用のデータ構造
// @param[in] assign_list1, assign_list2 割当リスト
//
// こちらは結果のみを返す．
inline
SatBool3
GvalCnf::check_sat(const NodeValList& assign_list1,
		   const NodeValList& assign_list2)
{
  vector<SatBool3> model;
  return check_sat(assign_list1, assign_list2, model);
}

// @brief デバッグ用のフラグをセットする．
inline
void
GvalCnf::set_debug(ymuint bits)
{
  mDebugFlag = bits;
}

// @brief デバッグ用のフラグを得る．
inline
ymuint
GvalCnf::debug() const
{
  return mDebugFlag;
}

END_NAMESPACE_YM_SATPG

#endif // GVALCNF_H
