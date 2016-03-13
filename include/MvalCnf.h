#ifndef MVALCNF_H
#define MVALCNF_H

/// @file MvalCnf.h
/// @brief MvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnfBase.h"
#include "GvalCnf.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MvalCnf MvalCnf.h "MvalCnf.h"
/// @brief 複数の故障検出用CNFを作るクラス
//////////////////////////////////////////////////////////////////////
class MvalCnf :
  public FvalCnfBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] gval_cnf 正常回路のCNFを作るクラス
  MvalCnf(GvalCnf& gval_cnf);

  /// @brief デストラクタ
  ~MvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障に対応した変数を得る．
  /// @param[in] pos 位置
  SatVarId
  fault_var(ymuint pos) const;

  /// @brief ノードに検出対象の故障があるか調べる．
  bool
  has_faults(const TpgNode* node) const;

  /// @brief 出力の故障に対応した変数を得る．
  /// @param[in] node ノード
  /// @param[in] fval 故障値 (0/1)
  SatVarId
  ofvar(const TpgNode* node,
	int fval) const;

  /// @brief 入力の故障に対応した変数を得る．
  /// @param[in] node ノード
  /// @param[in] pos 入力番号
  /// @param[in] fval 故障値 (0/1)
  SatVarId
  ifvar(const TpgNode* node,
	ymuint pos,
	int fval) const;

  /// @brief 故障数をセットする．
  /// @param[in] num 故障数
  void
  set_fault_num(ymuint num);

  /// @brief 故障に対応した変数をセットする．
  void
  set_fault_var(ymuint pos,
		SatVarId var);

  /// @brief 出力の故障検出用の変数を割り当てる．
  /// @param[in] node ノード
  /// @param[in] fval 故障値 (0/1)
  /// @param[in] fdvar 変数番号
  void
  set_ofvar(const TpgNode* node,
	    int fval,
	    SatVarId fdvar);

  /// @brief 入力の故障検出用の変数を割り当てる．
  /// @param[in] node ノード
  /// @param[in] pos 入力番号
  /// @param[in] fval 故障値 (0/1)
  /// @param[in] fdvar 変数番号
  void
  set_ifvar(const TpgNode* node,
	    ymuint pos,
	    int fval,
	    SatVarId fdvar);

  /// @brief 複数故障検出回路のCNFを作る．
  /// @param[in] fault_list 故障リスト
  /// @param[in] fnode_list 故障を持つノードのリスト
  /// @param[in] node_set 故障に関係するノード集合
  void
  make_cnf(const vector<const TpgFault*>& fault_list,
	   const vector<const TpgNode*>& fnode_list,
	   const NodeSet& node_set);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] engine SAT エンジン
  /// @param[in] node 対象のノード
  void
  make_fnode_cnf(const TpgNode* node);

  /// @brief 0縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] cvar 故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// cvar = 1 の時は ovar = 0 となる．
  void
  make_flt0_cnf(SatVarId ivar,
		SatVarId cvar,
		SatVarId ovar);

  /// @brief 1縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] cvar 故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// cvar = 1 の時は ovar = 1 となる．
  void
  make_flt1_cnf(SatVarId ivar,
		SatVarId cvar,
		SatVarId ovar);

  /// @brief 0/1縮退故障挿入回路の CNF を作る．
  /// @param[in] ivar 入力の変数
  /// @param[in] c0var 0縮退故障制御変数
  /// @param[in] c1var 1縮退故障制御変数
  /// @param[in] ovar 出力の変数
  ///
  /// 通常は ovar <=> ivar だが
  /// c0var = 1 の時は ovar = 0 となる．
  /// c1var = 1 の時は ovar = 1 となる．
  void
  make_flt01_cnf(SatVarId ivar,
		 SatVarId c0var,
		 SatVarId c1var,
		 SatVarId ovar);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの故障検出用変数マップ
  struct FdMap
  {
    // コンストラクタ
    FdMap(ymuint ni) :
      mNi(ni),
      mIvar(ni * 2)
    {
    }

    // 入力数
    ymuint mNi;

    // 出力の故障に対応する変数番号
    SatVarId mOvar[2];

    // 入力の故障に対応する変数番号
    // サイズは mNi * 2
    vector<SatVarId> mIvar;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FdMap を初期化する．
  /// @param[in] node ノード
  FdMap*
  setup_fdmap(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障検出用の変数マップ配列
  // サイズは mMaxId
  vector<FdMap*> mFdMapArray;

  // 故障に対応した変数の配列
  vector<SatVarId> mFaultVarArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 故障に対応した変数を得る．
// @param[in] pos 位置
inline
SatVarId
MvalCnf::fault_var(ymuint pos) const
{
  ASSERT_COND( pos < mFaultVarArray.size() );
  return mFaultVarArray[pos];
}

// @brief 故障数をセットする．
// @param[in] num 故障数
inline
void
MvalCnf::set_fault_num(ymuint num)
{
  mFaultVarArray.clear();
  mFaultVarArray.resize(num);
}

// @brief 故障に対応した変数をセットする．
inline
void
MvalCnf::set_fault_var(ymuint pos,
		       SatVarId var)
{
  ASSERT_COND( pos < mFaultVarArray.size() );
  mFaultVarArray[pos] = var;
}

END_NAMESPACE_YM_SATPG

#endif // MVALCNF_H
