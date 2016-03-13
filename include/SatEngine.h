#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "Val3.h"
#include "TpgNode.h"
#include "ym/ym_logic.h"
#include "ym/SatLiteral.h"
#include "ym/SatBool3.h"
#include "ym/SatSolver.h"
#include "ym/SatStats.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief 論理ゲートのネットワークに関する SAT 問題を解くためのクラス
///
/// AND/OR/XORなどの論理ゲートの入出力の関係を表す CNF 式を作る関数
/// を用意している．
/// ゲートの入出力の値を表すリテラルは LitMap クラスを用いて指定する．
/// また作業用にリテラル配列を持っているので動的にリテラルを追加して
/// 節を作りたい時に利用できる (tmp_lits_XXX 関数)
//////////////////////////////////////////////////////////////////////
class SatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  SatEngine(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp);

  /// @brief デストラクタ
  virtual
  ~SatEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SATソルバを返す．
  SatSolver&
  solver();

  /// @brief 複数故障検出回路のCNFを作る．
  /// @param[in] mval_cnf 故障回路用のデータ構造
  /// @param[in] fault_list 故障リスト
  /// @param[in] fnode_list 故障を持つノードのリスト
  /// @param[in] node_set 故障に関係するノード集合
  void
  make_mval_cnf(MvalCnf& mval_cnf,
		const vector<const TpgFault*>& fault_list,
		const vector<const TpgNode*>& fnode_list,
		const NodeSet& node_set);

  /// @brief SAT 問題を解く．
  /// @param[in] assumptions 仮定を表すリテラルのリスト
  /// @param[out] model 結果の割当を格納するベクタ
  /// @param[out] sat_stats 統計情報
  /// @param[out] time 処理時間
  SatBool3
  solve(const vector<SatLiteral>& assumptions,
	vector<SatBool3>& model,
	SatStats& sat_stats,
	USTime& time);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] vid_map 変数番号のマップ
  void
  make_node_cnf(const TpgNode* node,
		const VidMap& vid_map);

  /// @brief 故障回路のノードの入出力の関係を表す CNF を作る．
  /// @param[in] node 対象のノード
  void
  make_fnode_cnf(const TpgNode* node,
		 const MvalCnf& mval_cnf);

  /// @brief 故障伝搬条件を表すCNFを作る．
  /// @param[in] node 対象のノード
  /// @param[in] dst_node 伝搬条件の終点のノード
  /// @param[in] gvar_map 正常値の変数マップ
  /// @param[in] fvar_map 故障値の変数マップ
  /// @param[in] dvar_map 故障伝搬条件の変数マップ
  void
  make_dchain_cnf(const TpgNode* node,
		  const TpgNode* dst_node,
		  const VidMap& gvar_map,
		  const VidMap& fvar_map,
		  const VidMap& dvar_map);

  /// @brief 多入力 AND ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_and_cnf(const LitMap& litmap,
	       bool inv);

  /// @brief 多入力 OR ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_or_cnf(const LitMap& litmap,
	      bool inv);

  /// @brief 多入力 XOR ゲートの入出力の関係を表す CNF 式を生成する．
  /// @param[in] litmap 入出力のリテラルを返すファンクタオブジェクト
  /// @param[in] inv 出力が反転している時 true にするフラグ
  void
  make_xor_cnf(const LitMap& litmap,
	       bool inv);

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

  /// @brief 節の作成用の作業領域の使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  void
  tmp_lits_begin(ymuint exp_size = 0);

  /// @brief 作業領域にリテラルを追加する．
  /// @param[in] lit 追加するリテラル
  void
  tmp_lits_add(SatLiteral lit);

  /// @brief 作業領域の節を SAT ソルバに加える．
  void
  tmp_lits_end();

  /// @brief 変数を追加する．
  /// @return 新しい変数番号を返す．
  /// @note 変数番号は 0 から始まる．
  SatVarId
  new_var();

  /// @brief 節を追加する．
  /// @param[in] lits リテラルのベクタ
  void
  add_clause(const vector<SatLiteral>& lits);

  /// @brief 1項の節(リテラル)を追加する．
  void
  add_clause(SatLiteral lit1);

  /// @brief 2項の節を追加する．
  void
  add_clause(SatLiteral lit1,
	     SatLiteral lit2);

  /// @brief 3項の節を追加する．
  void
  add_clause(SatLiteral lit1,
	     SatLiteral lit2,
	     SatLiteral lit3);

  /// @brief 4項の節を追加する．
  void
  add_clause(SatLiteral lit1,
	     SatLiteral lit2,
	     SatLiteral lit3,
	     SatLiteral lit4);

  /// @brief 5項の節を追加する．
  void
  add_clause(SatLiteral lit1,
	     SatLiteral lit2,
	     SatLiteral lit3,
	     SatLiteral lit4,
	     SatLiteral lit5);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT ソルバ
  SatSolver mSolver;

  // 作業用のリテラルのリスト
  vector<SatLiteral> mTmpLits;

  // 時間計測用のタイマー
  StopWatch mTimer;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバを返す．
inline
SatSolver&
SatEngine::solver()
{
  return mSolver;
}

// @brief 節の作成用の作業領域の使用を開始する．
// @param[in] exp_size 予想されるサイズ
inline
void
SatEngine::tmp_lits_begin(ymuint exp_size)
{
  mTmpLits.clear();
  if ( exp_size > 0 ) {
    mTmpLits.reserve(exp_size);
  }
}

// @brief 作業領域にリテラルを追加する．
inline
void
SatEngine::tmp_lits_add(SatLiteral lit)
{
  mTmpLits.push_back(lit);
}

// @brief 作業領域の冊を SAT ソルバに加える．
inline
void
SatEngine::tmp_lits_end()
{
  mSolver.add_clause(mTmpLits);
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
inline
SatVarId
SatEngine::new_var()
{
  return mSolver.new_var();
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
inline
void
SatEngine::add_clause(const vector<SatLiteral>& lits)
{
  mSolver.add_clause(lits);
}

// @brief 1項の節(リテラル)を追加する．
inline
void
SatEngine::add_clause(SatLiteral lit1)
{
  mSolver.add_clause(lit1);
}

// @brief 2項の節を追加する．
inline
void
SatEngine::add_clause(SatLiteral lit1,
		      SatLiteral lit2)
{
  mSolver.add_clause(lit1, lit2);
}

// @brief 3項の節を追加する．
inline
void
SatEngine::add_clause(SatLiteral lit1,
		      SatLiteral lit2,
		      SatLiteral lit3)
{
  mSolver.add_clause(lit1, lit2, lit3);
}

// @brief 4項の節を追加する．
inline
void
SatEngine::add_clause(SatLiteral lit1,
		      SatLiteral lit2,
		      SatLiteral lit3,
		      SatLiteral lit4)
{
  mSolver.add_clause(lit1, lit2, lit3, lit4);
}

/// @brief 5項の節を追加する．
inline
void
SatEngine::add_clause(SatLiteral lit1,
		      SatLiteral lit2,
		      SatLiteral lit3,
		      SatLiteral lit4,
		      SatLiteral lit5)
{
  mSolver.add_clause(lit1, lit2, lit3, lit4, lit5);
}

// @brief SAT 問題を解く．
// @param[in] assumptions 仮定を表すリテラルのリスト
// @param[out] model 結果の割当を格納するベクタ
// @param[out] sat_stats 統計情報
// @param[out] time 処理時間
inline
SatBool3
SatEngine::solve(const vector<SatLiteral>& assumptions,
		 vector<SatBool3>& model,
		 SatStats& sat_stats,
		 USTime& time)
{
  SatStats prev_stats;
  mSolver.get_stats(prev_stats);

  mTimer.reset();
  mTimer.start();

  SatBool3 ans = mSolver.solve(assumptions, model);

  mTimer.stop();
  time = mTimer.time();

  mSolver.get_stats(sat_stats);

  sat_stats -= prev_stats;

  return ans;
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINE_H
