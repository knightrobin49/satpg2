
/// @file DtpgSatS.cc
/// @brief DtpgSatS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatS.h"
#include "DtpgStats.h"
#include "StructSat.h"
#include "FoCone.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "FaultMgr.h"
#include "Fsim.h"
#include "LitMap.h"
#include "VidLitMap.h"
#include "VectLitMap.h"
#include "ym/SatSolverR.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

class GvarVidMap :
  public VidMap
{
public:

  /// @brief ノードに関連した変数番号を返す．
  /// @param[in] node 対象のノード
  SatVarId
  operator()(const TpgNode* node) const;

};

SatVarId
GvarVidMap::operator()(const TpgNode* node) const
{
  return node->mGvar;
}

class FvarVidMap :
  public VidMap
{
public:

  /// @brief ノードに関連した変数番号を返す．
  /// @param[in] node 対象のノード
  SatVarId
  operator()(const TpgNode* node) const;

};

SatVarId
FvarVidMap::operator()(const TpgNode* node) const
{
  return node->mFvar;
}


class GvarLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] node 対象のノード
  GvarLitMap(const TpgNode* node);

  /// @brief デストラクタ
  ~GvarLitMap();

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const;

  /// @brief 入力のリテラルを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
  virtual
  SatLiteral
  input(ymuint pos) const;

  /// @brief 出力のリテラルを返す．
  virtual
  SatLiteral
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

};

// @brief コンストラクタ
// @param[in] node 対象のノード
GvarLitMap::GvarLitMap(const TpgNode* node) :
  mNode(node)
{
}

// @brief デストラクタ
GvarLitMap::~GvarLitMap()
{
}

// @brief 入力数を返す．
ymuint
GvarLitMap::input_size() const
{
  return mNode->fanin_num();
}

// @brief 入力のリテラルを返す．
// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
SatLiteral
GvarLitMap::input(ymuint pos) const
{
  return SatLiteral(mNode->fanin(pos)->mGvar);
}

// @brief 出力のリテラルを返す．
SatLiteral
GvarLitMap::output() const
{
  return SatLiteral(mNode->mGvar);
}


class FvarLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] node 対象のノード
  FvarLitMap(const TpgNode* node);

  ~FvarLitMap();

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const;

  /// @brief 入力のリテラルを返す．
  /// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
  virtual
  SatLiteral
  input(ymuint pos) const;

  /// @brief 出力のリテラルを返す．
  virtual
  SatLiteral
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

};

// @brief コンストラクタ
// @param[in] node 対象のノード
FvarLitMap::FvarLitMap(const TpgNode* node) :
  mNode(node)
{
}

FvarLitMap::~FvarLitMap()
{
}

// @brief 入力数を返す．
ymuint
FvarLitMap::input_size() const
{
  return mNode->fanin_num();
}

// @brief 入力のリテラルを返す．
// @param[in] pos 入力位置 ( 0 <= pos < input_size() )
SatLiteral
FvarLitMap::input(ymuint pos) const
{
  return SatLiteral(mNode->fanin(pos)->mFvar);
}

// @brief 出力のリテラルを返す．
SatLiteral
FvarLitMap::output() const
{
  return SatLiteral(mNode->mFvar);
}

#if USE_VIDMAP

void
make_dchain_cnf(SatSolver& solver,
		const TpgNode* node,
		const VidMap& gvar_map,
		const VidMap& fvar_map,
		const VidMap& dvar_map)
{
  SatLiteral glit(gvar_map(node));
  SatLiteral flit(fvar_map(node));
  SatLiteral dlit(dvar_map(node));

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  if ( node->is_output() ) {
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
  }
  else {
    // dlit -> ファンアウト先のノードの dlit の一つが 1
    ymuint nfo = node->fanout_num();
    if ( nfo == 1 ) {
      SatLiteral odlit(dvar_map(node->fanout(0)));
      solver.add_clause(~dlit, odlit);
    }
    else {
      vector<SatLiteral> tmp_lits(nfo + 1);
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* onode = node->fanout(i);
	tmp_lits[i] = SatLiteral(dvar_map(onode));
      }
      tmp_lits[nfo] = ~dlit;
      solver.add_clause(tmp_lits);
    }
    const TpgNode* imm_dom = node->imm_dom();
    if ( imm_dom != nullptr ) {
      SatLiteral odlit(dvar_map(imm_dom));
      solver.add_clause(~dlit, odlit);
    }
  }
}

#else

void
make_dchain_cnf(SatSolver& solver,
		const TpgNode* node,
		const VidMap& fvar_map,
		const VidMap& dvar_map)
{
  SatLiteral glit(node->mGvar);
  //SatLiteral flit(node->mFvar);
  //SatLiteral dlit(node->mDvar);
  SatLiteral flit(fvar_map(node));
  SatLiteral dlit(dvar_map(node));

  // dlit -> XOR(glit, flit) を追加する．
  // 要するに正常回路と故障回路で異なっているとき dlit が 1 となる．
  solver.add_clause(~glit, ~flit, ~dlit);
  solver.add_clause( glit,  flit, ~dlit);

  if ( node->is_output() ) {
    solver.add_clause(~glit,  flit,  dlit);
    solver.add_clause( glit, ~flit,  dlit);
  }
  else {
    // dlit -> ファンアウト先のノードの dlit の一つが 1
    ymuint nfo = node->fanout_num();
    if ( nfo == 1 ) {
      //SatLiteral odlit(node->fanout(0)->mDvar);
      SatLiteral odlit(dvar_map(node->fanout(0)));
      solver.add_clause(~dlit, odlit);
    }
    else {
      vector<SatLiteral> tmp_lits(nfo + 1);
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* onode = node->fanout(i);
	//tmp_lits[i] = SatLiteral(onode->mDvar);
	tmp_lits[i] = SatLiteral(dvar_map(onode));
      }
      tmp_lits[nfo] = ~dlit;
      solver.add_clause(tmp_lits);
    }
    const TpgNode* imm_dom = node->imm_dom();
    if ( imm_dom != nullptr ) {
      //SatLiteral odlit(imm_dom->mDvar);
      SatLiteral odlit(dvar_map(imm_dom));
      solver.add_clause(~dlit, odlit);
    }
  }
}

#endif

END_NONAMESPACE

// @brief Single エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatS(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatS::DtpgSatS(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatS::~DtpgSatS()
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fmgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 対象の故障リスト
// @param[out] stats 結果を格納する構造体
void
DtpgSatS::run(TpgNetwork& network,
	      FaultMgr& fmgr,
	      Fsim& fsim,
	      const vector<const TpgFault*>& fault_list,
	      DtpgStats& stats)
{
  clear_stats();

  // 故障シミュレータに故障リストをセットする．
  fsim.set_faults(fault_list);

  mMaxNodeId = network.node_num();

  ymuint max_fault_id = network.max_fault_id();

  // fault_list に含まれる故障に印をつける．
  vector<bool> fault_mark(max_fault_id, false);
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    const TpgFault* fault = fault_list[i];
    ymuint fid = fault->id();
    fault_mark[fid] = true;
  }

  ymuint nn = network.node_num();
  ymuint max_id = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);

    ymuint nf = network.node_fault_num(node->id());
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* fault = network.node_fault(node->id(), i);
      if ( !fault_mark[fault->id()] || fmgr.status(fault) != kFsUndetected ) {
	continue;
      }

      // 故障に対するテスト生成を行なう．
#if 0
      cnf_begin();

      StructSat struct_sat(max_id);
      const FoCone* focone = struct_sat.add_focone(fault, kVal1);

      cnf_end();

      solve(struct_sat.solver(), vector<SatLiteral>(), fault, node, focone->output_list(),
	    focone->gvar_map(), focone->fvar_map());
#else
      run_single(fault);
#endif
    }
  }

  get_stats(stats);
}

// @brief テスト生成を行なう．
// @param[in] fault 対象の故障
void
DtpgSatS::run_single(const TpgFault* fault)
{
  cnf_begin();

  mNodeList.clear();
  mMarkArray.clear();
  mMarkArray.resize(mMaxNodeId, 0);

  const TpgNode* fnode = fault->tpg_onode();

  vector<const TpgNode*> output_list;

  // fnode の TFO を mNodeList に入れる．
  set_tfo_mark(fnode);
  if ( fnode->is_output() ) {
    output_list.push_back(fnode);
  }
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    //cout << "node#" << node->id() << ": ";
    ymuint nfo = node->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      const TpgNode* onode = node->fanout(i);
      //cout << " " << onode->id();
      set_tfo_mark(onode);
      if ( onode->is_output() ) {
	output_list.push_back(onode);
      }
    }
    //cout << endl;
  }
  ymuint tfo_num = mNodeList.size();

  // TFO の TFI を mNodeList に入れる．
  for (ymuint rpos = 0; rpos < mNodeList.size(); ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
      set_tfi_mark(inode);
    }
  }
  ymuint tfi_num = mNodeList.size();

  SatSolver solver(sat_type(), sat_option());

  //cout << fault->str() << endl;

  // TFO の部分に変数を割り当てる．
#if USE_VIDMAP
  //GenVidMap gvar_map(mMaxNodeId);
  GvarVidMap gvar_map;
  GenVidMap fvar_map(mMaxNodeId);
  GenVidMap dvar_map(mMaxNodeId);
#else
  GenVidMap fvar_map(mMaxNodeId);
  GenVidMap dvar_map(mMaxNodeId);
#endif
  for (ymuint rpos = 0; rpos < tfo_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    //cout << "Node#" << node->id() << endl;
    SatVarId gvar = solver.new_var();
    SatVarId fvar = solver.new_var();
    SatVarId dvar = solver.new_var();

#if USE_VIDMAP
    //gvar_map.set_vid(node, gvar);
    node->mGvar = gvar;
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
#else
    node->mGvar = gvar;
    //node->mFvar = fvar;
    //node->mDvar = dvar;
    fvar_map.set_vid(node, fvar);
    dvar_map.set_vid(node, dvar);
#endif
  }

  // TFI の部分に変数を割り当てる．
  for (ymuint rpos = tfo_num; rpos < tfi_num; ++ rpos) {
    const TpgNode* node = mNodeList[rpos];
    //cout << "Node#" << node->id() << endl;
    SatVarId gvar = solver.new_var();

#if USE_VIDMAP
    //gvar_map.set_vid(node, gvar);
    node->mGvar = gvar;
    fvar_map.set_vid(node, gvar);
#else
    node->mGvar = gvar;
    //node->mFvar = gvar;
    fvar_map.set_vid(node, gvar);
#endif
  }


  //////////////////////////////////////////////////////////////////////
  // 正常回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  for (ymuint i = 0; i < tfi_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    //cout << "Node#" << node->id() << ": ";
#if USE_VIDMAP
    node->make_cnf(solver, VidLitMap(node, gvar_map));
#else
    node->make_cnf(solver, GvarLitMap(node));
#endif
  }


  //////////////////////////////////////////////////////////////////////
  // 故障回路の CNF を生成
  //////////////////////////////////////////////////////////////////////

  //cout << "Node#" << fnode->id() << ": ";
  if ( fault->is_stem_fault() ) {
#if USE_VIDMAP
    SatLiteral flit(fvar_map(fnode));
#else
    //SatLiteral flit(fnode->mFvar);
    SatLiteral flit(fvar_map(fnode));
#endif
    if ( fault->val() == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
    }
  }
  else {
    ymuint ni = fnode->fanin_num();
    vector<SatVarId> ivars(ni);
    SatVarId fvar = solver.new_var();
    for (ymuint i = 0; i < ni; ++ i) {
      if ( i == fault->tpg_pos() ) {
	ivars[i] = fvar;
      }
      else {
	const TpgNode* inode = fnode->fanin(i);
#if USE_VIDMAP
	ivars[i] = fvar_map(inode);
#else
	//ivars[i] = inode->mFvar;
	ivars[i] = fvar_map(inode);
#endif
      }
    }
    SatLiteral flit(fvar);
    if ( fault->val() == 0 ) {
      solver.add_clause(~flit);
    }
    else {
      solver.add_clause(flit);
    }
#if USE_VIDMAP
    fnode->make_cnf(solver, VectLitMap(ivars, fvar_map(fnode)));
#else
    //fnode->make_cnf(solver, VectLitMap(ivars, fnode->mFvar));
    fnode->make_cnf(solver, VectLitMap(ivars, fvar_map(fnode)));
#endif
  }
#if USE_VIDMAP
  make_dchain_cnf(solver, fnode, gvar_map, fvar_map, dvar_map);
#else
  make_dchain_cnf(solver, fnode, fvar_map, dvar_map);
#endif

  for (ymuint i = 1; i < tfo_num; ++ i) {
    const TpgNode* node = mNodeList[i];
    //cout << "Node#" << node->id() << ": ";
#if USE_VIDMAP
    node->make_cnf(solver, VidLitMap(node, fvar_map));
#else
    //node->make_cnf(solver, FvarLitMap(node));
    node->make_cnf(solver, VidLitMap(node, fvar_map));
#endif

#if USE_VIDMAP
    make_dchain_cnf(solver, node, gvar_map, fvar_map, dvar_map);
#else
    make_dchain_cnf(solver, node, fvar_map, dvar_map);
#endif

    ymuint ni = node->fanin_num();
    vector<SatLiteral> tmp_lits;
    tmp_lits.reserve(ni + 1);
#if USE_VIDMAP
    SatLiteral dlit(dvar_map(node));
#else
    //SatLiteral dlit(node->mDvar);
    SatLiteral dlit(dvar_map(node));
#endif
    tmp_lits.push_back(~dlit);
    for (ymuint i = 0; i < ni; ++ i) {
      const TpgNode* inode = node->fanin(i);
#if USE_VIDMAP
      if ( gvar_map(inode) != fvar_map(inode) ) {
	SatLiteral dlit(dvar_map(inode));
	tmp_lits.push_back(dlit);
      }
#else
      if ( inode->mGvar != fvar_map(inode) ) {
	//SatLiteral dlit(inode->mDvar);
	SatLiteral dlit(dvar_map(inode));
	tmp_lits.push_back(dlit);
      }
#endif
    }
    solver.add_clause(tmp_lits);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障の検出条件
  //////////////////////////////////////////////////////////////////////
  ymuint no = output_list.size();
  vector<SatLiteral> odiff(no);
  for (ymuint i = 0; i < no; ++ i) {
    const TpgNode* node = output_list[i];
#if USE_VIDMAP
    SatLiteral dlit(dvar_map(node));
#else
    //SatLiteral dlit(node->mDvar);
    SatLiteral dlit(dvar_map(node));
#endif
    odiff[i] = dlit;
  }
  solver.add_clause(odiff);

  cnf_end();

  vector<SatLiteral> assumptions;
  for (const TpgNode* node = fnode; node != nullptr; node = node->imm_dom()) {
#if USE_VIDMAP
    SatLiteral dlit(dvar_map(node));
#else
    //SatLiteral dlit(node->mDvar);
    SatLiteral dlit(dvar_map(node));
#endif
    assumptions.push_back(dlit);
  }

#if USE_VIDMAP
  solve(solver, assumptions, fault, fnode, output_list,
	gvar_map, fvar_map);
#else
  solve(solver, assumptions, fault, fnode, output_list,
	GvarVidMap(), fvar_map);
#endif

}

END_NAMESPACE_YM_SATPG
