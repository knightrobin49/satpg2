#ifndef FVALCNF_H
#define FVALCNF_H

/// @file FvalCnf.h
/// @brief FvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnfBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FvalCnf FvalCnf.h "FvalCnf.h"
/// @brief 故障回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class FvalCnf :
  public FvalCnfBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] gval_cnf 正常回路のCNFを作るクラス
  FvalCnf(GvalCnf& gval_cnf);

  /// @brief デストラクタ
  ~FvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 十分割当リストを求める．
  /// @param[in] sat_model SAT問題の解
  /// @param[in] fault 故障
  /// @param[in] node_set 故障に関連するノード集合
  /// @param[out] suf_list 十分割当リストを格納する変数
  void
  get_suf_list(const vector<SatBool3>& sat_model,
	       const TpgFault* fault,
	       const NodeSet& node_set,
	       NodeValList& suf_list);

  /// @brief 十分割当リストを求める．
  /// @param[in] sat_model SAT問題の解
  /// @param[in] fault 故障
  /// @param[in] node_set 故障に関連するノード集合
  /// @param[out] suf_list 十分割当リストを格納する変数
  /// @param[out] pi_suf_list 外部入力上の十分割当リストを格納する変数
  void
  get_pi_suf_list(const vector<SatBool3>& sat_model,
		  const TpgFault* fault,
		  const NodeSet& node_set,
		  NodeValList& suf_list,
		  NodeValList& pi_suf_list);

  /// @brief 故障検出用の変数番号を返す．
  SatVarId
  fd_var() const;

  /// @brief 故障検出用の変数番号を割り当てる．
  void
  set_fdvar(SatVarId fdvar);

  /// @brief 故障回路のCNFを作る．
  /// @param[in] src_node 故障位置のノード
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] detect 検出条件
  ///
  /// detect = kVal0: 検出しないCNFを作る．
  ///        = kVal1: 検出するCNFを作る．
  ///        = kValX: fd_var() で制御するCNFを作る．
  void
  make_cnf(const TpgNode* src_node,
	   const NodeSet& node_set,
	   Val3 detect);

  /// @brief 故障回路のCNFを作る．
  /// @param[in] fault 故障
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] detect 検出条件
  ///
  /// detect = kVal0: 検出しないCNFを作る．
  ///        = kVal1: 検出するCNFを作る．
  ///        = kValX: fd_var() で制御するCNFを作る．
  void
  make_cnf(const TpgFault* fault,
	   const NodeSet& node_set,
	   Val3 detect);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障検出用の変数番号
  SatVarId mFdVar;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 故障検出用の変数番号を返す．
inline
SatVarId
FvalCnf::fd_var() const
{
  return mFdVar;
}

// @brief 故障検出用の変数番号を割り当てる．
inline
void
FvalCnf::set_fdvar(SatVarId fdvar)
{
  mFdVar = fdvar;
}

END_NAMESPACE_YM_SATPG

#endif // FVALCNF_H
