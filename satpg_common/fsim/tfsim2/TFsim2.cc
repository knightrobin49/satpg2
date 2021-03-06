﻿
/// @file TFsim2.cc
/// @brief TFsim2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TFsim2.h"
#include "FsimOp.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "NodeValList.h"
#include "DetectOp.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "ym/HashSet.h"
#include "ym/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

Fsim*
new_TFsim2()
{
  return new nsTFsim2::TFsim2();
}

END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_TFSIM2

BEGIN_NONAMESPACE

void
clear_lobs(SimNode* node)
{
  while ( node->check_lobs() ) {
    node->clear_lobs();
    node = node->fanout(0);
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// TFsim2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TFsim2::TFsim2()
{
}

// @brief デストラクタ
TFsim2::~TFsim2()
{
  clear();
}

// @brief ネットワークをセットする関数
// @param[in] network ネットワーク
void
TFsim2::set_network(const TpgNetwork& network)
{
  clear();

  mNetwork = &network;

  ymuint nn = mNetwork->node_num();
  ymuint ni = mNetwork->input_num2();
  ymuint no = mNetwork->output_num2();

  // SimNode の生成
  // 対応付けを行うマップの初期化
  mSimMap.resize(nn);
  mInputArray.resize(ni);
  mOutputArray.resize(no);

  ymuint nf = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* tpgnode = mNetwork->node(i);
    nf += tpgnode->fault_num();

    SimNode* node = nullptr;

    if ( tpgnode->is_input() ) {
      // 外部入力に対応する SimNode の生成
      node = make_input();
      mInputArray[tpgnode->input_id()] = node;
      node->set_name(tpgnode->name());
    }
    else if ( tpgnode->is_output() ) {
      // 外部出力に対応する SimNode の生成
      SimNode* inode = find_simnode(tpgnode->fanin(0));
      // 実際にはバッファタイプのノードに出力の印をつけるだけ．
      node = make_node(kGateBUFF, vector<SimNode*>(1, inode));
      node->set_output();
      mOutputArray[tpgnode->output_id()] = node;
      node->set_name(tpgnode->name());
    }
    else if ( tpgnode->is_logic() ) {
      // 論理ノードに対する SimNode の作成
      ymuint ni = tpgnode->fanin_num();

      // ファンインに対応する SimNode を探す．
      vector<SimNode*> inputs(ni);
      for (ymuint i = 0; i < ni; ++ i) {
	const TpgNode* itpgnode = tpgnode->fanin(i);
	SimNode* inode = find_simnode(itpgnode);
	ASSERT_COND(inode );
	inputs[i] = inode;
      }

      // 出力の論理を表す SimNode を作る．
      GateType type = tpgnode->gate_type();
      node = make_node(type, inputs);
      node->set_name(tpgnode->name());
    }
    // 対応表に登録しておく．
    mSimMap[tpgnode->id()] = node;
  }

  {
    for (ymuint i = 0; i < ni; ++ i) {
      SimNode* node = mInputArray[i];
      ASSERT_COND( node != nullptr );
    }
    for (ymuint i = 0; i < no; ++ i) {
      SimNode* node = mOutputArray[i];
      ASSERT_COND( node != nullptr );
    }
  }

  // 各ノードのファンアウトリストの設定
  ymuint node_num = mNodeArray.size();
  {
    vector<vector<SimNode*> > fanout_lists(node_num);
    vector<ymuint> ipos(node_num);
    for (vector<SimNode*>::iterator p = mNodeArray.begin();
	 p != mNodeArray.end(); ++ p) {
      SimNode* node = *p;
      ymuint ni = node->nfi();
      for (ymuint i = 0; i < ni; ++ i) {
	SimNode* inode = node->fanin(i);
	fanout_lists[inode->id()].push_back(node);
	ipos[inode->id()] = i;
      }
    }
    for (ymuint i = 0; i < node_num; ++ i) {
      SimNode* node = mNodeArray[i];
      node->set_fanout_list(fanout_lists[i], ipos[i]);
    }
  }

  // FFR の設定
  ymuint ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      ++ ffr_num;
    }
  }
  mFFRArray.resize(ffr_num);
  ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      SimFFR* ffr = &mFFRArray[ffr_num];
      node->set_ffr(ffr);
      ffr->set_root(node);
      ++ ffr_num;
    }
    else {
      SimFFR* ffr = node->fanout(0)->ffr();
      node->set_ffr(ffr);
    }
  }

  // 消去用の配列の大きさはノード数を越えない．
  mClearArray.reserve(mNodeArray.size());

  // 最大レベルを求め，イベントキューを初期化する．
  ymuint max_level = 0;
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);


  //////////////////////////////////////////////////////////////////////
  // 故障リストの設定
  //////////////////////////////////////////////////////////////////////
  mSimFaults.resize(nf);
  mFaultArray.resize(network.max_fault_id());
  ymuint fid = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* tpgnode = network.node(i);
    ymuint nf1 = tpgnode->fault_num();
    for (ymuint j = 0; j < nf1; ++ j) {
      const TpgFault* fault = tpgnode->fault(j);
      const TpgNode* tpgnode = fault->tpg_onode();
      SimNode* simnode = find_simnode(tpgnode);
      SimNode* isimnode = nullptr;
      ymuint ipos = 0;
      if ( fault->is_branch_fault() ) {
	ipos = fault->tpg_pos();
	const TpgNode* inode = tpgnode->fanin(ipos);
	isimnode = find_simnode(inode);
      }
      else {
	isimnode = simnode;
      }
      mSimFaults[fid].set(fault, simnode, ipos, isimnode);
      SimFault* ff = &mSimFaults[fid];
      mFaultArray[fault->id()] = ff;
      ++ fid;
    }
  }
}

// @brief 故障にスキップマークをつける．
void
TFsim2::set_skip(const TpgFault* f)
{
  mFaultArray[f->id()]->mSkip = true;
}

// @brief 故障リストを設定する．
// @param[in] fault_list 対象の故障リスト
//
// スキップマークは消される．
void
TFsim2::set_faults(const vector<const TpgFault*>& fault_list)
{
  HashSet<ymuint> fault_set;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    fault_set.add(fault_list[i]->id());
  }

  // 同時に各 SimFFR 内の fault_list() も再構築する．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    p->fault_list().clear();
  }
  ymuint nf = mSimFaults.size();
  for (ymuint i = 0; i < nf; ++ i) {
    SimFault* ff = &mSimFaults[i];
    if ( fault_set.check(ff->mOrigF->id()) ) {
      ff->mSkip = false;
      SimNode* simnode = ff->mNode;
      SimFFR* ffr = simnode->ffr();
      ffr->fault_list().push_back(ff);
    }
    else {
      ff->mSkip = true;
    }
  }
}

// @brief ひとつのパタンで故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] op 検出した時に起動されるファンクタオブジェクト
void
TFsim2::sppfp(TestVector* tv,
	      FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval1(val);
  }

  _sppfp(op);
}

// @brief ひとつのパタンで故障シミュレーションを行う．
// @param[in] assign_list 値の割当リスト
// @param[in] op 検出した時に起動されるファンクタオブジェクト
void
TFsim2::sppfp(const NodeValList& assign_list,
	      FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();

  // デフォルトで 0 にする．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval1(kPvAll0);
  }

  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    if ( nv.val() ) {
      SimNode* simnode = mInputArray[nv.node()->input_id()];
      simnode->set_gval1(kPvAll1);
    }
  }

  _sppfp(op);
}

// @brief SPPFP故障シミュレーションの本体
// @param[in] op 検出した時に起動されるファンクタオブジェクト
void
TFsim2::_sppfp(FsimOp& op)
{
  // 1時刻目の正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval1();
  }

  // 2時刻目のフリップフロップの値を設定する．
  ymuint npi1 = mNetwork->input_num();
  ymuint npo1 = mNetwork->output_num();
  ymuint nff = mNetwork->dff_num();
  for (ymuint i = 0; i < nff; ++ i) {
    SimNode* onode = mOutputArray[i + npo1];
    SimNode* inode = mInputArray[i + npi1];
    inode->set_gval(onode->gval());
  }

  // 2時刻目の正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  ymuint bitpos = 0;
  SimFFR* ffr_buff[kPvBitLen];
  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は SimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    SimNode* root = ffr->root();
    if ( root->is_output() ) {
      // 常に観測可能
      fault_sweep(ffr, op);
      continue;
    }

    // キューに積んでおく
    PackedVal bitmask = 1UL << bitpos;
    PackedVal pat = root->gval() ^ bitmask;
    root->set_fval(pat);
    root->set_fmask(~bitmask);

    mClearArray.push_back(root);
    ymuint no = root->nfo();
    for (ymuint i = 0; i < no; ++ i) {
      mEventQ.put(root->fanout(i));
    }
    ffr_buff[bitpos] = ffr;

    ++ bitpos;
    if ( bitpos == kPvBitLen ) {
      PackedVal obs = eventq_simulate();
      for (ymuint i = 0; i < kPvBitLen; ++ i, obs >>= 1) {
	if ( obs & 1UL ) {
	  fault_sweep(ffr_buff[i], op);
	}
      }
      bitpos = 0;
    }
  }
  if ( bitpos > 0 ) {
    PackedVal obs = eventq_simulate();
    for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
      if ( obs & 1UL ) {
	fault_sweep(ffr_buff[i], op);
      }
    }
  }
}

// @brief 複数のパタンで故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
void
TFsim2::ppsfp(const vector<TestVector*>& tv_array,
	      FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();
  ymuint nb = tv_array.size();

  // tv_array を入力ごとに固めてセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    PackedVal val = kPvAll0;
    PackedVal bit = 1UL;
    for (ymuint j = 0; j < nb; ++ j, bit <<= 1) {
      if ( tv_array[j]->val3(i) == kVal1 ) {
	val |= bit;
      }
    }
    // 残ったビットには 0 番めのパタンを詰めておく．
    if ( tv_array[0]->val3(i) == kVal1 ) {
      for (ymuint j = nb; j < kPvBitLen; ++ j, bit <<= 1) {
	val |= bit;
      }
    }
    SimNode* simnode = mInputArray[i];
    simnode->set_gval1(val);
  }

  // 1時刻目の正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval1();
  }

  // 2時刻目のフリップフロップの値を設定する．
  {
    ymuint npi1 = mNetwork->input_num();
    ymuint npo1 = mNetwork->output_num();
    ymuint nff = mNetwork->dff_num();
    for (ymuint i = 0; i < nff; ++ i) {
      SimNode* onode = mOutputArray[i + npo1];
      SimNode* inode = mInputArray[i + npi1];
      inode->set_gval(onode->gval());
    }
  }

  // 2時刻目の正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は SimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    // FFR の出力の故障伝搬を行う．
    SimNode* root = ffr->root();
    PackedVal obs = kPvAll0;
    if ( root->is_output() ) {
      obs = kPvAll1;
    }
    else {
      PackedVal pat = root->gval() ^ ffr_req;
      root->set_fval(pat);
      mClearArray.clear();
      mClearArray.push_back(root);
      ymuint no = root->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(root->fanout(i));
      }
      obs = eventq_simulate();
    }

    // obs と各々の故障の mObsMask との AND が 0 でなければ故障検出
    // できたということ．対応するテストベクタを記録する．
    vector<SimFault*>& flist = ffr->fault_list();
    for (vector<SimFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      SimFault* ff = *p;
      if ( ff->mSkip ) {
	continue;
      }
      PackedVal dpat = obs & ff->mObsMask;
      if ( dpat ) {
	const TpgFault* f = ff->mOrigF;
	op(f, dpat);
      }
    }
  }
}

// @brief SPSFP故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
// @retval true 故障の検出が行えた．
// @retval false 故障の検出が行えなかった．
bool
TFsim2::spsfp(TestVector* tv,
	      const TpgFault* f)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval(val);
  }

  return _spsfp(f);
}

// @brief SPSFP故障シミュレーションを行う．
// @param[in] assign_list 値の割当リスト
// @param[in] f 対象の故障
// @retval true 故障の検出が行えた．
// @retval false 故障の検出が行えなかった．
bool
TFsim2::spsfp(const NodeValList& assign_list,
	      const TpgFault* f)
{
  ymuint npi = mNetwork->input_num2();

  // assign_list にないノードの値は 0 にしておく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(kPvAll0);
  }

  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    if ( nv.val() ) {
      SimNode* simnode = mInputArray[nv.node()->input_id()];
      simnode->set_gval(kPvAll1);
    }
  }

  return _spsfp(f);
}

// @brief SPSFP故障シミュレーションの本体
// @param[in] f 対象の故障
// @retval true 故障の検出が行えた．
// @retval false 故障の検出が行えなかった．
bool
TFsim2::_spsfp(const TpgFault* f)
{
  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  // FFR 内の故障伝搬を行う．
  PackedVal lobs;
  if ( f->is_branch_fault() ) {
    SimNode* simnode = find_simnode(f->tpg_onode());
    ymuint ipos = f->tpg_pos();
    lobs = simnode->calc_lobs() & simnode->calc_gobs2(ipos);
    clear_lobs(simnode);
  }
  else {
    SimNode* simnode = find_simnode(f->tpg_inode());
    lobs = simnode->calc_lobs();
    clear_lobs(simnode);
  }

  SimNode* isimnode = find_simnode(f->tpg_inode());
  PackedVal valdiff = isimnode->gval();
  if ( f->val() == 1 ) {
    valdiff = ~valdiff;
  }
  lobs &= valdiff;

  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs == kPvAll0 ) {
    return false;
  }

  SimNode* root = isimnode->ffr()->root();
  if ( root->is_output() ) {
    return (lobs != kPvAll0);
  }

  root->set_fval(~root->gval());

  mClearArray.push_back(root);
  ymuint no = root->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(root->fanout(i));
  }

  PackedVal obs = eventq_simulate() & lobs;
  return (obs != kPvAll0);
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
TFsim2::clear()
{
  mSimMap.clear();

  // mNodeArray が全てのノードを持っている
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    delete *p;
  }
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();

  mFFRArray.clear();

  mClearArray.clear();

  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }

  mSimFaults.clear();
  mFaultArray.clear();

  // 念のため
  mNetwork = nullptr;
}

// @brief FFR 内の故障シミュレーションを行う．
PackedVal
TFsim2::ffr_simulate(SimFFR* ffr)
{
  PackedVal ffr_req = kPvAll0;
  const vector<SimFault*>& flist = ffr->fault_list();
  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip ) {
      continue;
    }

    // ff の故障伝搬を行う．
    SimNode* simnode = ff->mNode;
    PackedVal lobs = simnode->calc_lobs();
    PackedVal val1 = ff->mInode->gval1();
    PackedVal val2 = ff->mInode->gval();
    const TpgFault* f = ff->mOrigF;
    if ( f->is_branch_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->calc_gobs2(ipos);
    }
    PackedVal valdiff;
    if ( f->val() == 1 ) {
      // 1 -> 0 への遷移
      valdiff = ~val2 & val1;
    }
    else {
      // 0 -> 1 への遷移
      valdiff = val2 & ~val1;
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
  }

  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( !ff->mSkip ) {
      SimNode* node = ff->mNode;
      clear_lobs(node);
    }
  }

  return ffr_req;
}

// @brief イベントキューを用いてシミュレーションを行う．
PackedVal
TFsim2::eventq_simulate()
{
  // どこかの外部出力で検出されたことを表すビット
  PackedVal obs = kPvAll0;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == nullptr ) break;
    // すでに検出済みのビットはマスクしておく
    // これは無駄なイベントの発生を抑える．
    PackedVal diff = node->calc_fval2(~obs);
    if ( diff != kPvAll0 ) {
      mClearArray.push_back(node);
      if ( node->is_output() ) {
	obs |= diff;
      }
      else {
	ymuint no = node->nfo();
	for (ymuint i = 0; i < no; ++ i) {
	  mEventQ.put(node->fanout(i));
	}
      }
    }
  }
  // 今の故障シミュレーションで値の変わったノードを元にもどしておく
  for (vector<SimNode*>::iterator p = mClearArray.begin();
       p != mClearArray.end(); ++ p) {
    (*p)->clear_fval();
  }
  mClearArray.clear();
  return obs;
}

// @brief ffr 内の故障が検出可能か調べる．
void
TFsim2::fault_sweep(SimFFR* ffr,
		    FsimOp& op)
{
  vector<SimFault*>& flist = ffr->fault_list();
  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( !ff->mSkip && ff->mObsMask != kPvAll0 ) {
      const TpgFault* f = ff->mOrigF;
      op(f, kPvAll1);
    }
  }
}

// @brief 外部入力ノードを作る．
SimNode*
TFsim2::make_input()
{
  ymuint id = mNodeArray.size();
  SimNode* node = SimNode::new_input(id);
  mNodeArray.push_back(node);
  return node;
}

// @brief 単純な logic ノードを作る．
SimNode*
TFsim2::make_node(GateType type,
		 const vector<SimNode*>& inputs)
{
  ymuint id = mNodeArray.size();
  SimNode* node = SimNode::new_node(id, type, inputs);
  mNodeArray.push_back(node);
  mLogicArray.push_back(node);
  return node;
}

// @brief node に対応する SimNode* を得る．
SimNode*
TFsim2::find_simnode(const TpgNode* node) const
{
  return mSimMap[node->id()];
}

// @brief WSA を計算する．
// @param[in] tv テストベクタ
ymuint
TFsim2::calc_wsa(TestVector* tv)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval1(val);
  }

  // 1時刻目の正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval1();
  }

  // 2時刻目のフリップフロップの値を設定する．
  ymuint npi1 = mNetwork->input_num();
  ymuint npo1 = mNetwork->output_num();
  ymuint nff = mNetwork->dff_num();
  for (ymuint i = 0; i < nff; ++ i) {
    SimNode* onode = mOutputArray[i + npo1];
    SimNode* inode = mInputArray[i + npi1];
    inode->set_gval(onode->gval());
  }

  // 2時刻目の正常値の計算を行う．
  // と同時に WSA を計算する．
  ymuint wsa = 0;
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();

    // wsa を計算する．
    if ( node->gval1() != node->gval() ) {
      wsa += node->nfo();
    }
  }

  return wsa;
}

END_NAMESPACE_YM_SATPG_TFSIM2
