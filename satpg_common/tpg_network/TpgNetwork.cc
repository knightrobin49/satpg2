
/// @File TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgNodeInfo.h"
#include "TpgNodeMap.h"
#include "TpgFault.h"
#include "TpgStemFault.h"
#include "TpgBranchFault.h"
#include "AuxNodeInfo.h"

#include "ym/BnBlifReader.h"
#include "ym/BnIscas89Reader.h"
#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnDff.h"
#include "ym/BnNode.h"
#include "ym/Expr.h"

#define USE_MYALLOC 1

BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// BnFuncType を GateType に変換する．
inline
GateType
conv_to_gate_type(BnLogicType type)
{
  switch ( type ) {
  case kBnLt_C0:   return kGateCONST0;
  case kBnLt_C1:   return kGateCONST1;
  case kBnLt_BUFF: return kGateBUFF;
  case kBnLt_NOT:  return kGateNOT;
  case kBnLt_AND:  return kGateAND;
  case kBnLt_NAND: return kGateNAND;
  case kBnLt_OR:   return kGateOR;
  case kBnLt_NOR:  return kGateNOR;
  case kBnLt_XOR:  return kGateXOR;
  case kBnLt_XNOR: return kGateXNOR;
  case kBnLt_EXPR: return kGateCPLX;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return kGateCPLX;
}

// immediate dominator リストをマージする．
const TpgNode*
merge(const TpgNode* node1,
      const TpgNode* node2)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == nullptr || node2 == nullptr ) {
      return nullptr;
    }
    ymuint id1 = node1->id();
    ymuint id2 = node2->id();
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
    }
  }
}

// pair の最初の要素で比較を行なうファンクター
// こういうのは lambda 表記があると簡潔にすむ．
struct Lt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }

};


void
check_network_connection(const TpgNetwork& network)
{
  // fanin/fanout の sanity check
  bool error = false;

  ymuint nn = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.node(i);
    ymuint nfi = node->fanin_num();
    for (ymuint j = 0; j < nfi; ++ j) {
      const TpgNode* inode = node->fanin(j);
      ymuint nfo = inode->fanout_num();
      bool found = false;
      for (ymuint k = 0; k < nfo; ++ k) {
	if ( inode->fanout(k) == node ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	error = true;
	cout << "Error: inode(" << inode->id() << ") is a fanin of "
	     << "node(" << node->id() << "), but "
	     << "node(" << node->id() << ") is not a fanout of "
	     << "inode(" << inode->id() << ")" << endl;
      }
    }
    ymuint nfo = node->fanout_num();
    for (ymuint j = 0; j < nfo; ++ j) {
      const TpgNode* onode = node->fanout(j);
      ymuint nfi = onode->fanin_num();
      bool found = false;
      for (ymuint k = 0; k < nfi; ++ k) {
	if ( onode->fanin(k) == node ) {
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	error = true;
	cout << "Error: onode(" << onode->id() << ") is a fanout of "
	     << "node(" << node->id() << "), but "
	     << "node(" << node->id() << ") is not a fanin of "
	     << "onode(" << onode->id() << ")" << endl;
      }
    }
  }
  if ( error ) {
    cout << "network connectivity check failed" << endl;
    abort();
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TpgNetwork::TpgNetwork() :
  mAlloc(4096)
{
  mInputNum = 0;
  mOutputNum = 0;
  mDffNum = 0;
  mDffArray = nullptr;
  mNodeNum = 0;
  mNodeArray = nullptr;
  mAuxInfoArray = nullptr;
  mPPIArray = nullptr;
  mPPOArray = nullptr;
  mPPOArray2 = nullptr;
  mFaultNum = 0;
  mMffcNum = 0;
  mFfrNum = 0;
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  clear();
}

// @brief blif ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @return 読み込みが成功したら true を返す．
bool
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  BnNetwork network;
  bool stat = BnBlifReader::read(network, filename, cell_library);
  if ( stat ) {
    clear();
    set(network);
  }

  return stat;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @return 読み込みが成功したら true を返す．
bool
TpgNetwork::read_iscas89(const string& filename)
{
  BnNetwork network;
  bool stat = BnIscas89Reader::read(network, filename);
  if ( stat ) {
    clear();
    set(network);
  }

  return stat;
}

// @brief ノード名を返す．
// @param[in] id ノードID ( 0 <= id < node_num() )
const char*
TpgNetwork::node_name(ymuint id) const
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].name();
}

// @brief 出力の故障を得る．
// @param[in] id ノードID ( 0 <= id < node_num() )
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgNetwork::node_output_fault(ymuint id,
			      int val) const
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].output_fault(val);
}

// @brief 入力の故障を得る．
// @param[in] id ノードID ( 0 <= id < node_num() )
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
const TpgFault*
TpgNetwork::node_input_fault(ymuint id,
			     int val,
			     ymuint pos) const
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].input_fault(pos, val);
}

// @brief 出力の故障を得る．
// @param[in] id ノードID ( 0 <= id < node_num() )
// @param[in] val 故障値 ( 0 / 1 )
TpgFault*
TpgNetwork::_node_output_fault(ymuint id,
			       int val)
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].output_fault(val);
}

// @brief 入力の故障を得る．
// @param[in] id ノードID ( 0 <= id < node_num() )
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
TpgFault*
TpgNetwork::_node_input_fault(ymuint id,
			      int val,
			      ymuint pos)
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].input_fault(pos, val);
}

// @brief このノードに関係する代表故障数を返す．
// @param[in] id ノードID ( 0 <= id < node_num() )
ymuint
TpgNetwork::node_fault_num(ymuint id) const
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].fault_num();
}

// @brief このノードに関係する代表故障を返す．
// @param[in] id ノードID ( 0 <= id < node_num() )
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
const TpgFault*
TpgNetwork::node_fault(ymuint id,
		       ymuint pos) const
{
  ASSERT_COND( id < mNodeNum );
  return mAuxInfoArray[id].fault(pos);
}

BEGIN_NONAMESPACE

// @brief ノードの TFI にマークをつける．
ymuint
tfimark(TpgNode* node,
	vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return 0;
  }
  mark[node->id()] = true;

  ymuint ni = node->fanin_num();
  ymuint n = 1;
  for (ymuint i = 0; i < ni; ++ i) {
    n += tfimark(node->fanin(i), mark);
  }
  return n;
}

END_NONAMESPACE

// @brief 内容をクリアする．
void
TpgNetwork::clear()
{
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    delete node;
  }

  delete [] mDffArray;
  delete [] mNodeArray;
  delete [] mAuxInfoArray;
  delete [] mPPIArray;
  delete [] mPPOArray;
  delete [] mPPOArray2;

  mAlloc.destroy();

  mDffArray = nullptr;
  mNodeArray = nullptr;
  mAuxInfoArray = nullptr;
  mPPIArray = nullptr;
  mPPOArray = nullptr;
  mPPOArray2 = nullptr;
}

// @brief 内容を設定する．
// @param[in] network 設定元のネットワーク
void
TpgNetwork::set(const BnNetwork& network)
{
  //////////////////////////////////////////////////////////////////////
  // NodeInfoMgr にノードの論理関数を登録する．
  //////////////////////////////////////////////////////////////////////
  TpgNodeInfoMgr node_info_mgr;
  ymuint nexpr = network.expr_num();
  vector<const TpgNodeInfo*> node_info_list(nexpr);
  ymuint extra_node_num = 0;
  for (ymuint i = 0; i < nexpr; ++ i) {
    Expr expr = network.expr(i);
    ymuint ni = expr.input_size();
    const TpgNodeInfo* node_info = node_info_mgr.complex_type(ni, expr);
    node_info_list[i] = node_info;
  }

  //////////////////////////////////////////////////////////////////////
  // 追加で生成されるノード数を数える．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = network.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* src_node = network.logic(i);
    BnLogicType logic_type = src_node->logic_type();
    if ( logic_type == kBnLt_EXPR ) {
      const TpgNodeInfo* node_info = node_info_list[src_node->func_id()];
      extra_node_num += node_info->extra_node_num();
    }
    else if ( logic_type == kBnLt_XOR || logic_type == kBnLt_XNOR ) {
      ymuint ni = src_node->fanin_num();
      extra_node_num += (ni - 2);
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  // BnPort は複数ビットの場合があり，さらに入出力が一緒なのでめんどくさい
  vector<ymuint> input_map;
  vector<ymuint> output_map;
  ymuint np = network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BnPort* port = network.port(i);
    ymuint nb = port->bit_width();
    for (ymuint j = 0; j < nb; ++ j) {
      ymuint id = port->bit(j);
      const BnNode* node = network.node(id);
      if ( node->is_input() ) {
	input_map.push_back(id);
      }
      else if ( node->is_output() ) {
	output_map.push_back(id);
      }
      else {
	ASSERT_NOT_REACHED;
      }
    }
  }
  mInputNum = input_map.size();
  mOutputNum = output_map.size();
  mDffNum = network.dff_num();

  ymuint dff_control_num = 0;
  for (ymuint i = 0; i < mDffNum; ++ i) {
    const BnDff* dff = network.dff(i);
    // まずクロックで一つ
    ++ dff_control_num;
    if ( dff->clear() != kBnNullId ) {
      // クリア端子で一つ
      ++ dff_control_num;
    }
    if ( dff->preset() != kBnNullId ) {
      // プリセット端子で一つ
      ++ dff_control_num;
    }
  }

  mDffArray = new TpgDff[mDffNum];
  for (ymuint i = 0; i < mDffNum; ++ i) {
    mDffArray[i].mId = i;
  }

  ymuint nn = mInputNum + mOutputNum + mDffNum * 2 + nl + extra_node_num + dff_control_num;

  mNodeArray = new TpgNode*[nn];

  mAuxInfoArray = new AuxNodeInfo[nn];

  mPPIArray = new TpgNode*[mInputNum + mDffNum];
  mPPOArray = new TpgNode*[mOutputNum + mDffNum];
  mPPOArray2 = new TpgNode*[mOutputNum + mDffNum];

  TpgNodeMap node_map;

  mNodeNum = 0;
  mFaultNum = 0;


  //////////////////////////////////////////////////////////////////////
  // 入力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mInputNum; ++ i) {
    ymuint id = input_map[i];
    const BnNode* src_node = network.node(id);
    ASSERT_COND( src_node->is_input() );
    ymuint nfo = src_node->fanout_num();
    TpgNode* node = make_input_node(i, src_node->name(), nfo);
    mPPIArray[i] = node;

    node_map.reg(id, node);
  }


  //////////////////////////////////////////////////////////////////////
  // DFFの出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mDffNum; ++ i) {
    const BnDff* src_dff = network.dff(i);
    const BnNode* src_node = network.node(src_dff->output());
    ASSERT_COND( src_node->is_input() );
    ymuint nfo = src_node->fanout_num();
    TpgDff* dff = &mDffArray[i];
    TpgNode* node = make_dff_output_node(i + mInputNum, dff, src_node->name(), nfo);
    mPPIArray[i + mInputNum] = node;
    dff->mOutput = node;

    node_map.reg(src_node->id(), node);
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* src_node = network.logic(i);
    const TpgNodeInfo* node_info = nullptr;
    BnLogicType logic_type = src_node->logic_type();
    if ( logic_type == kBnLt_EXPR ) {
      node_info = node_info_list[src_node->func_id()];
    }
    else {
      ASSERT_COND( logic_type != kBnLt_TV );
      GateType gate_type = conv_to_gate_type(logic_type);
      node_info = node_info_mgr.simple_type(gate_type);
    }

    // ファンインのノードを取ってくる．
    ymuint ni = src_node->fanin_num();
    vector<TpgNode*> fanin_array(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      fanin_array[j] = node_map.get(src_node->fanin(j));
    }
    ymuint nfo = src_node->fanout_num();
    TpgNode* node = make_logic_node(src_node->name(), node_info, fanin_array, nfo);

    // ノードを登録する．
    node_map.reg(src_node->id(), node);
  }


  //////////////////////////////////////////////////////////////////////
  // 出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mOutputNum; ++ i) {
    ymuint id = output_map[i];
    const BnNode* src_node = network.node(id);
    ASSERT_COND( src_node->is_output() );
    TpgNode* inode = node_map.get(src_node->fanin());
    string buf = "*";
    buf += src_node->name();
    TpgNode* node = make_output_node(i, buf, inode);
    mPPOArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // DFFの入力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mDffNum; ++ i) {
    const BnDff* src_dff = network.dff(i);
    const BnNode* src_node = network.node(src_dff->input());

    TpgNode* inode = node_map.get(src_node->fanin());
    string dff_name = src_dff->name();
    string input_name = dff_name + ".input";
    TpgDff* dff = &mDffArray[i];
    TpgNode* node = make_dff_input_node(i + mOutputNum, dff, input_name, inode);
    mPPOArray[i + mOutputNum] = node;
    dff->mInput = node;

    // クロック端子を作る．
    const BnNode* src_clock = network.node(src_dff->clock());
    TpgNode* clock_fanin = node_map.get(src_clock->fanin());
    string clock_name = dff_name + ".clock";
    TpgNode* clock = make_dff_clock_node(dff, clock_name, clock_fanin);
    dff->mClock = clock;

    // クリア端子を作る．
    if ( src_dff->clear() != kBnNullId ) {
      const BnNode* src_clear = network.node(src_dff->clear());
      TpgNode* clear_fanin = node_map.get(src_clear->fanin());
      string clear_name = dff_name + ".clear";
      TpgNode* clear = make_dff_clear_node(dff, clear_name, clear_fanin);
      dff->mClear = clear;
    }

    // プリセット端子を作る．
    if ( src_dff->preset() != kBnNullId ) {
      const BnNode* src_preset = network.node(src_dff->preset());
      TpgNode* preset_fanin = node_map.get(src_preset->fanin());
      string preset_name = dff_name + ".preset";
      TpgNode* preset = make_dff_preset_node(dff, preset_name, preset_fanin);
      dff->mPreset = preset;
    }
  }

  ASSERT_COND( mNodeNum == nn );


  //////////////////////////////////////////////////////////////////////
  // ファンアウトをセットする．
  //////////////////////////////////////////////////////////////////////
  vector<ymuint> nfo_array(mNodeNum, 0);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgNode* inode = node->fanin(j);
      ymuint& fo_pos = nfo_array[inode->id()];
      inode->set_fanout(fo_pos, node);
      ++ fo_pos;
    }
  }
  { // 検証
    ymuint error = 0;
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      TpgNode* node = mNodeArray[i];
      if ( nfo_array[node->id()] != node->fanout_num() ) {
	if ( error == 0 ) {
	  cerr << "Error in TpgNetwork()" << endl;
	}
	cerr << "nfo_array[Node#" << node->id() << "] = " << nfo_array[node->id()] << endl
	     << "node->fanout_num()    = " << node->fanout_num() << endl;
	++ error;
      }
    }
    if ( error ) {
      abort();
    }
    // 接続が正しいかチェックする．
    check_network_connection(*this);
  }


  //////////////////////////////////////////////////////////////////////
  // データ系のノードに印をつける．
  //////////////////////////////////////////////////////////////////////
  vector<bool> marks(mNodeNum, false);
  for (ymuint i = 0; i < ppo_num(); ++ i) {
    TpgNode* node = ppo(i);
    tfimark(node, marks);
  }


  //////////////////////////////////////////////////////////////////////
  // 代表故障を求める．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    // ノードごとに代表故障を設定する．
    // この処理は出力側から行う必要がある．
    TpgNode* node = mNodeArray[mNodeNum - i - 1];
    if ( marks[node->id()] ) {
      set_rep_faults(node);
    }
  }


  //////////////////////////////////////////////////////////////////////
  // TFI のサイズの昇順に並べた出力順を
  // mPPOArray2 に記録する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num();
  vector<pair<ymuint, ymuint> > tmp_list(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* onode = output(i);
    // onode の TFI のノード数を計算する．
    vector<bool> mark(nn, false);
    ymuint n = tfimark(onode, mark);
    tmp_list[i] = make_pair(n, i);
  }

  // TFI のサイズの昇順にソートする．
  sort(tmp_list.begin(), tmp_list.end(), Lt());
  // tmp_list の順に mPPOArray2 にセットする．
  for (ymuint i = 0; i < npo; ++ i) {
    ymuint opos = tmp_list[i].second;
    TpgNode* onode = mPPOArray[opos];
    mPPOArray2[i] = onode;
    onode->set_output_id2(i);
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[mNodeNum - i - 1];
    const TpgNode* imm_dom = nullptr;
    if ( !node->is_ppo() &&
	 !node->is_dff_clock() &&
	 !node->is_dff_clear() &&
	 !node->is_dff_preset() ) {
      ymuint nfo = node->fanout_num();
      ASSERT_COND( nfo > 0 );
      imm_dom = node->fanout(0);
      for (ymuint i = 1; imm_dom != nullptr && i < nfo; ++ i) {
	imm_dom = merge(imm_dom, node->fanout(i));
      }
    }
    node->set_imm_dom(imm_dom);
  }

  // MFFC 内の FFR の情報をセットしておく．
  ymuint total_mffc_size = 0;
  ymuint max_mffc_size = 0;
  ymuint total_mffc_inputs = 0;
  ymuint max_mffc_inputs = 0;

  mMffcNum = 0;
  mFfrNum = 0;
  vector<bool> mark(node_num(), false);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( node->imm_dom() != nullptr ) {
      continue;
    }

    // node を根とする MFFC の情報を得る．
    vector<TpgNode*> node_list;
    vector<TpgNode*> input_list;

    node_list.push_back(node);
    mark[node->id()] = true;
    for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
      TpgNode* node = node_list[rpos];
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	TpgNode* inode = node->fanin(i);
	if ( mark[inode->id()] ) {
	  continue;
	}
	mark[inode->id()] = true;
	if ( inode->imm_dom() == nullptr ) {
	  input_list.push_back(inode);
	}
	else {
	  if ( inode->is_ppi() ) {
	    // これは例外
	    input_list.push_back(inode);
	  }
	  node_list.push_back(inode);
	}
      }
    }
    // マークを消しておく．
    // ただし根と葉のノードだけで十分
    mark[node->id()] = false;
    for (ymuint rpos = 0; rpos < input_list.size(); ++ rpos) {
      TpgNode* node = input_list[rpos];
      mark[node->id()] = false;
    }
#if 0
    MffcInfo* mffc_info = new MffcInfo(node_list, input_list);
    mMffcInfoList.push_back(mffc_info);
#endif
    // node を根とする MFFC 内の FFR の根のノードのリストを作る．
    ymuint elem_num = 0;
    for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
      TpgNode* node = node_list[rpos];
      if ( node->ffr_root() == node ) {
	++ elem_num;
      }
    }
    void* p = mAlloc.get_memory(sizeof(TpgNode*) * elem_num);
    TpgNode** root_list = new (p) TpgNode*[elem_num];
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
      TpgNode* node = node_list[rpos];
      if ( node->ffr_root() == node ) {
	root_list[wpos] = node;
	++ wpos;
      }
    }
    node->set_mffc_info(elem_num, root_list);
    ++ mMffcNum;
    mFfrNum += elem_num;

    ymuint mffc_size = node_list.size();
    total_mffc_size += mffc_size;
    if ( max_mffc_size < mffc_size ) {
      max_mffc_size = mffc_size;
    }
    ymuint input_size = input_list.size();
    total_mffc_inputs += input_size;
    if ( max_mffc_inputs < input_size ) {
      max_mffc_inputs = input_size;
    }
  }
}

BEGIN_NONAMESPACE

class FoNodeLt
{
public:

  // コンストラクタ
  FoNodeLt(const vector<ymuint>& level_array) :
    mLevelArray(level_array)
  {
  }

  bool
  operator()(TpgNode* left,
	     TpgNode* right)
  {
    return mLevelArray[left->id()] < mLevelArray[right->id()];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const vector<ymuint>& mLevelArray;

};

END_NONAMESPACE

// @brief 入力ノードを生成する．
// @param[in] iid 入力の番号
// @param[in] name ノード名
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_input_node(ymuint iid,
			    const string& name,
			    ymuint fanout_num)
{
  TpgNode* node = TpgNode::make_input(mNodeNum, iid, fanout_num);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);

  // 出力位置の故障を生成
  const char* c_name = node_name(node->id());
  for (int val = 0; val < 2; ++ val) {
    new_ofault(c_name, val, node);
  }

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] oid 出力の番号
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_output_node(ymuint oid,
			     const string& name,
			     TpgNode* inode)
{
  TpgNode* node = TpgNode::make_output(mNodeNum, oid, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);
  mAuxInfoArray[node->id()].set_fanin_num(1, mAlloc);

  // 入力位置の故障を生成
  const char* c_name = node_name(node->id());
  ymuint ipos = 0;
  for (int val = 0; val < 2; ++ val) {
    new_ifault(c_name, ipos, val, InodeInfo(node, ipos), nullptr);
  }

  return node;
}

// @brief DFFの入力ノードを生成する．
// @param[in] oid 出力の番号
// @param[in] dff 接続しているDFF
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_dff_input_node(ymuint oid,
				TpgDff* dff,
				const string& name,
				TpgNode* inode)
{
  TpgNode* node = TpgNode::make_dff_input(mNodeNum, oid, dff, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);
  mAuxInfoArray[node->id()].set_fanin_num(1, mAlloc);

  // 入力位置の故障を生成
  const char* c_name = node_name(node->id());
  ymuint ipos = 0;
  for (int val = 0; val < 2; ++ val) {
    new_ifault(c_name, ipos, val, InodeInfo(node, ipos), nullptr);
  }

  return node;
}

// @brief 入力ノードを生成する．
// @param[in] iid 入力の番号
// @param[in] dff 接続しているDFF
// @param[in] name ノード名
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_dff_output_node(ymuint iid,
				 TpgDff* dff,
				 const string& name,
				 ymuint fanout_num)
{
  TpgNode* node = TpgNode::make_dff_output(mNodeNum, iid, dff, fanout_num);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);

  // 出力位置の故障を生成
  const char* c_name = node_name(node->id());
  for (int val = 0; val < 2; ++ val) {
    new_ofault(c_name, val, node);
  }

  return node;
}

// @brief DFFのクロック端子を生成する．
// @param[in] dff 接続しているDFF
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_dff_clock_node(TpgDff* dff,
				const string& name,
				TpgNode* inode)
{
  TpgNode* node = TpgNode::make_dff_clock(mNodeNum, dff, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);
  mAuxInfoArray[node->id()].set_fanin_num(1, mAlloc);

  // 入力位置の故障を生成
  const char* c_name = node_name(node->id());
  ymuint ipos = 0;
  for (int val = 0; val < 2; ++ val) {
    new_ifault(c_name, ipos, val, InodeInfo(node, ipos), nullptr);
  }

  return node;
}

// @brief DFFのクリア端子を生成する．
// @param[in] dff 接続しているDFF
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_dff_clear_node(TpgDff* dff,
				const string& name,
				TpgNode* inode)
{
  TpgNode* node = TpgNode::make_dff_clear(mNodeNum, dff, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);
  mAuxInfoArray[node->id()].set_fanin_num(1, mAlloc);

  // 入力位置の故障を生成
  const char* c_name = node_name(node->id());
  ymuint ipos = 0;
  for (int val = 0; val < 2; ++ val) {
    new_ifault(c_name, ipos, val, InodeInfo(node, ipos), nullptr);
  }

  return node;
}

// @brief DFFのプリセット端子を生成する．
// @param[in] dff 接続しているDFF
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_dff_preset_node(TpgDff* dff,
				 const string& name,
				 TpgNode* inode)
{
  TpgNode* node = TpgNode::make_dff_preset(mNodeNum, dff, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  mAuxInfoArray[node->id()].set_name(name, mAlloc);
  mAuxInfoArray[node->id()].set_fanin_num(1, mAlloc);

  // 入力位置の故障を生成
  const char* c_name = node_name(node->id());
  ymuint ipos = 0;
  for (int val = 0; val < 2; ++ val) {
    new_ifault(c_name, ipos, val, InodeInfo(node, ipos), nullptr);
  }

  return node;
}

// @brief 論理ノードを生成する．
// @param[in] src_name ノード名
// @param[in] node_info 論理関数の情報
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_logic_node(const string& src_name,
			    const TpgNodeInfo* node_info,
			    const vector<TpgNode*>& fanin_list,
			    ymuint fanout_num)
{
  ymuint ni = fanin_list.size();

  // 複合型の場合の入力ノードを納める配列
  vector<InodeInfo> inode_array(ni);

  GateType gate_type = node_info->gate_type();
  TpgNode* node = nullptr;
  if ( gate_type != kGateCPLX ) {
    // 組み込み型の場合．
    // 2入力以上の XOR/XNOR ゲートを2入力に分解する．
    if ( gate_type == kGateXOR && ni > 2 ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = fanin_list[0];
      tmp_list[1] = fanin_list[1];
      TpgNode* tmp_node = make_prim_node(string(), kGateXOR, tmp_list, 1);
      inode_array[0].set(tmp_node, 0);
      inode_array[1].set(tmp_node, 1);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = fanin_list[i];
	if ( i < ni - 1 ) {
	  tmp_node = make_prim_node(string(), kGateXOR, tmp_list, 1);
	}
	else {
	  tmp_node = make_prim_node(src_name, kGateXOR, tmp_list, fanout_num);
	}
	inode_array[i].set(tmp_node, 1);
      }
      node = tmp_node;
    }
    else if ( gate_type == kGateXNOR && ni > 2 ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = fanin_list[0];
      tmp_list[1] = fanin_list[1];
      TpgNode* tmp_node = make_prim_node(string(), kGateXOR, tmp_list, 1);
      inode_array[0].set(tmp_node, 0);
      inode_array[1].set(tmp_node, 1);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = fanin_list[i];
	if ( i < ni - 1 ) {
	  tmp_node = make_prim_node(string(), kGateXOR, tmp_list, 1);
	}
	else {
	  tmp_node = make_prim_node(src_name, kGateXNOR, tmp_list, fanout_num);
	}
	inode_array[i].set(tmp_node, i);
      }
      node = tmp_node;
    }
    else {
      node = make_prim_node(src_name, gate_type, fanin_list, fanout_num);
      for (ymuint i = 0; i < ni; ++ i) {
	inode_array[i].set(node, i);
      }
    }
  }
  else {
    Expr expr = node_info->expr();

    // 論理式の葉(リテラル)に対応するノードを入れる配列．
    // pos * 2 + 0: 肯定のリテラル
    // pos * 2 + 1: 否定のリテラルに対応する．
    vector<TpgNode*> leaf_nodes(ni * 2, nullptr);
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint p_num = expr.litnum(VarId(i), false);
      ymuint n_num = expr.litnum(VarId(i), true);
      TpgNode* inode = fanin_list[i];
      if ( n_num == 0 ) {
	if ( p_num == 1 ) {
	  // 肯定のリテラルが1回だけ現れている場合
	  // 本当のファンインを直接つなぐ
	  leaf_nodes[i * 2 + 0] = inode;
	}
	else {
	  // 肯定のリテラルが2回以上現れている場合
	  // ブランチの故障に対応するためにダミーのバッファをつくる．
	  TpgNode* dummy_buff = make_prim_node(string(), kGateBUFF, vector<TpgNode*>(1, inode), p_num);
	  leaf_nodes[i * 2 + 0] = dummy_buff;
	  // このバッファの入力が故障位置となる．
	  inode_array[i].set(dummy_buff, 0);
	}
      }
      else {
	if ( p_num > 0 ) {
	  // 肯定と否定のリテラルがともに現れる場合
	  // ブランチの故障に対応するためにダミーのバッファを作る．
	  TpgNode* dummy_buff = make_prim_node(string(), kGateBUFF, vector<TpgNode*>(1, inode), p_num + 1);
	  inode = dummy_buff;
	  leaf_nodes[i * 2 + 0] = dummy_buff;
	}

	// 否定のリテラルに対応するNOTゲートを作る．
	TpgNode* not_gate = make_prim_node(string(), kGateNOT, vector<TpgNode*>(1, inode), n_num);
	leaf_nodes[i * 2 + 1] = not_gate;

	if ( p_num > 0 ) {
	  inode_array[i].set(inode, 0);
	}
	else {
	  inode_array[i].set(not_gate, 0);
	}
      }
    }

    // expr の内容を表す TpgNode の木を作る．
    node = make_cplx_node(src_name, expr, leaf_nodes, inode_array, fanout_num);
  }

  // 出力位置の故障を生成
  const char* c_name = node_name(node->id());
  for (int val = 0; val < 2; ++ val) {
    new_ofault(c_name, val, node);
  }

  // 入力位置の故障を生成
  mAuxInfoArray[node->id()].set_fanin_num(ni, mAlloc);
  for (ymuint i = 0; i < ni; ++ i) {
    Val3 oval0 = node_info->cval(i, kVal0);
    Val3 oval1 = node_info->cval(i, kVal1);

    const TpgFault* rep0 = nullptr;
    if ( oval0 == kVal0 ) {
      rep0 = node_output_fault(node->id(), 0);
    }
    else if ( oval0 == kVal1 ) {
      rep0 = node_output_fault(node->id(), 1);
    }

    const TpgFault* rep1 = nullptr;
    if ( oval1 == kVal0 ) {
      rep1 = node_output_fault(node->id(), 0);
    }
    else if ( oval1 == kVal1 ) {
      rep1 = node_output_fault(node->id(), 1);
    }
    new_ifault(c_name, i, 0, inode_array[i], rep0);
    new_ifault(c_name, i, 1, inode_array[i], rep1);
  }

  return node;
}

// @brief 論理式から TpgNode の木を生成する．
// @param[in] name ノード名
// @param[in] expr 式
// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
// @param[in] inode_array ファンインの対応関係を収める配列
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
//
// leaf_nodes は 変数番号 * 2 + (0/1) に
// 該当する変数の肯定/否定のリテラルが入っている．
TpgNode*
TpgNetwork::make_cplx_node(const string& name,
			   const Expr& expr,
			   const vector<TpgNode*>& leaf_nodes,
			   vector<InodeInfo>& inode_array,
			   ymuint fanout_num)
{
  if ( expr.is_posiliteral() ) {
    ymuint iid = expr.varid().val();
    return leaf_nodes[iid * 2 + 0];
  }
  if ( expr.is_negaliteral() ) {
    ymuint iid = expr.varid().val();
    return leaf_nodes[iid * 2 + 1];
  }

  GateType gate_type;
  if ( expr.is_and() ) {
    gate_type = kGateAND;
  }
  else if ( expr.is_or() ) {
    gate_type = kGateOR;
  }
  else if ( expr.is_xor() ) {
    gate_type = kGateXOR;
  }
  else {
    ASSERT_NOT_REACHED;
  }

  // 子供の論理式を表すノード(の木)を作る．
  ymuint nc = expr.child_num();
  vector<TpgNode*> fanins(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Expr& expr1 = expr.child(i);
    TpgNode* inode = make_cplx_node(string(), expr1, leaf_nodes, inode_array, 1);
    ASSERT_COND( inode != nullptr );
    fanins[i] = inode;
  }
  // fanins[] を確保するオーバーヘッドがあるが，
  // 子供のノードよりも先に親のノードを確保するわけには行かない．
  TpgNode* node = make_prim_node(name, gate_type, fanins, fanout_num);

  // オペランドがリテラルの場合，inode_array[]
  // の設定を行う．
  for (ymuint i = 0; i < nc; ++ i) {
    // 美しくないけどスマートなやり方を思いつかない．
    const Expr& expr1 = expr.child(i);
    if ( expr1.is_posiliteral() ) {
      ymuint iid = expr1.varid().val();
      if ( inode_array[iid].mNode == nullptr ) {
	inode_array[iid].set(node, i);
      }
    }
  }

  return node;
}

// @brief 組み込み型の論理ゲートを生成する．
// @param[in] name ノード名
// @param[in] type ゲートの型
// @param[in] fanin_list ファンインのリスト
// @param[in] fanout_num ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_prim_node(const string& name,
			   GateType type,
			   const vector<TpgNode*>& fanin_list,
			   ymuint fanout_num)
{
  TpgNode* node = TpgNode::make_logic(mNodeNum, type, fanin_list, fanout_num);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  ymuint id = node->id();
  ymuint fanin_num = fanin_list.size();

  mAuxInfoArray[id].set_name(name, mAlloc);
  mAuxInfoArray[id].set_fanin_num(fanin_num, mAlloc);

  return node;
}

// @brief 出力の故障を作る．
// @param[in] name 故障位置のノード名
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] node 故障位置のノード
void
TpgNetwork::new_ofault(const char* name,
		       int val,
		       TpgNode* node)
{
  TpgFault* f = new TpgStemFault(mFaultNum, name, val, node, nullptr);
  mAuxInfoArray[node->id()].set_output_fault(val, f);
  ++ mFaultNum;
}

// @brief 入力の故障を作る．
// @param[in] name 故障位置のノード名
// @param[in] ipos 故障位置のファンイン番号
// @param[in] node 故障位置のノード
// @param[in] inode_pos node 上の入力位置
// @param[in] val 故障値
// @param[in] rep 代表故障
//
// プリミティブ型の場合は ipos と inode_pos は同一だが
// 複合型の場合には異なる．
void
TpgNetwork::new_ifault(const char* name,
		       ymuint ipos,
		       int val,
		       const InodeInfo& inode_info,
		       const TpgFault* rep)
{
  TpgNode* node = inode_info.mNode;
  ymuint inode_pos = inode_info.mPos;
  TpgNode* inode = node->fanin(inode_pos);
  TpgFault* f = new TpgBranchFault(mFaultNum, name, val, ipos, node, inode, inode_pos, rep);
  mAuxInfoArray[node->id()].set_input_fault(inode_pos, val, f);
  ++ mFaultNum;
}

// @brief 代表故障を設定する．
// @param[in] node 対象のノード
void
TpgNetwork::set_rep_faults(TpgNode* node)
{
  vector<const TpgFault*> fault_list;

  if ( node->fanout_num() == 1 ) {
    TpgNode* onode = node->fanout(0);
    // ファンアウト先が一つならばそのファンイン
    // ブランチの故障と出力の故障は等価
    ymuint ni = onode->fanin_num();
    ymuint ipos = ni;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( onode->fanin(i) == node ) {
	ipos = i;
	break;
      }
    }
    ASSERT_COND( ipos < ni );

    const TpgFault* rep0 = node_input_fault(onode->id(), 0, ipos);
    TpgFault* of0 = _node_output_fault(node->id(), 0);
    if ( of0 != nullptr ) {
      of0->set_rep(rep0);
    }

    const TpgFault* rep1 = node_input_fault(onode->id(), 1, ipos);
    TpgFault* of1 = _node_output_fault(node->id(), 1);
    if ( of1 != nullptr ){
      of1->set_rep(rep1);
    }
  }

  if ( !node->is_ppo() ) {
    TpgFault* of0 = _node_output_fault(node->id(), 0);
    if ( of0 != nullptr ) {
      const TpgFault* rep0 = of0->rep_fault();
      if ( rep0 == nullptr ) {
	of0->set_rep(of0);
	fault_list.push_back(of0);
      }
      else {
	of0->set_rep(rep0->rep_fault());
      }
    }

    TpgFault* of1 = _node_output_fault(node->id(), 1);
    if ( of1 != nullptr ) {
      const TpgFault* rep1 = of1->rep_fault();
      if ( rep1 == nullptr ) {
	of1->set_rep(of1);
	fault_list.push_back(of1);
      }
      else {
	of1->set_rep(rep1->rep_fault());
      }
    }
  }

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgFault* if0 = _node_input_fault(node->id(), 0, i);
    if ( if0 != nullptr ) {
      const TpgFault* rep0 = if0->rep_fault();
      if ( rep0 == nullptr ) {
	if0->set_rep(if0);
	fault_list.push_back(if0);
      }
      else {
	if0->set_rep(rep0->rep_fault());
      }
    }

    TpgFault* if1 = _node_input_fault(node->id(), 1, i);
    if ( if1 != nullptr ) {
      const TpgFault* rep1 = if1->rep_fault();
      if ( rep1 == nullptr ) {
	if1->set_rep(if1);
	fault_list.push_back(if1);
      }
      else {
	if1->set_rep(rep1->rep_fault());
      }
    }
  }

  // 代表故障を NodeAuxInfo::mFaultList に入れる．
  mAuxInfoArray[node->id()].set_fault_list(fault_list, mAlloc);
}

// @brief TpgNetwork の内容を出力する関数
// @param[in] s 出力先のストリーム
// @param[in] network 対象のネットワーク
void
print_network(ostream& s,
	      const TpgNetwork& network)
{
  ymuint n = network.node_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = network.node(i);
    print_node(s, network, node);
    s << ": ";
    if ( node->is_primary_input() ) {
      s << "INPUT#" << node->input_id();
    }
    else if ( node->is_dff_output() ) {
      s << "INPUT#" << node->input_id()
	<< "(DFF#" << node->dff()->id() << ".output)";
    }
    else if ( node->is_primary_output() ) {
      s << "OUTPUT#" << node->output_id();
      const TpgNode* inode = node->fanin(0);
      s << " = ";
      print_node(s, network, inode);
    }
    else if ( node->is_dff_input() ) {
      s << "OUTPUT#" << node->output_id()
	<< "(DFF#" << node->dff()->id() << ".input)";
      const TpgNode* inode = node->fanin(0);
      s << " = ";
      print_node(s, network, inode);
    }
    else if ( node->is_dff_clock() ) {
      s << "DFF#" << node->dff()->id() << ".clock";
    }
    else if ( node->is_dff_clear() ) {
      s << "DFF#" << node->dff()->id() << ".clear";
    }
    else if ( node->is_dff_preset() ) {
      s << "DFF#" << node->dff()->id() << ".preset";
    }
    else if ( node->is_logic() ) {
      s << node->gate_type();
      ymuint ni = node->fanin_num();
      if ( ni > 0 ) {
	s << "(";
	for (ymuint j = 0; j < ni; ++ j) {
	  const TpgNode* inode = node->fanin(j);
	  s << " ";
	  print_node(s, network, inode);
	}
	s << " )";
      }
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << endl;
  }
  s << endl;
}

// @brief ノード名を出力する
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
print_node(ostream& s,
	   const TpgNetwork& network,
	   const TpgNode* node)
{
  ymuint id = node->id();
  s << "NODE#" << id << ": ";
  const char* name = network.node_name(id);
  if ( name != nullptr ) {
    s << name;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AuxNodeInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AuxNodeInfo::AuxNodeInfo()
{
  mName = nullptr;
  mFaultNum = 0;
  mFaultList = nullptr;
  mOutputFaults[0] = nullptr;
  mOutputFaults[1] = nullptr;
  mFaninNum = 0;
  mInputFaults = nullptr;
}

// @brief デストラクタ
AuxNodeInfo::~AuxNodeInfo()
{
  for (int val = 0; val < 2; ++ val) {
    delete mOutputFaults[val];
  }
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    for (int val = 0; val < 2; ++ val) {
      delete mInputFaults[(i * 2) + val];
    }
  }
}

// @brief 名前を設定する．
// @param[in] name 名前
// @param[in] alloc メモリアロケータ
void
AuxNodeInfo::set_name(const string& name,
		      Alloc& alloc)
{
  ymuint l = name.size();
  void* p = alloc.get_memory(sizeof(char) * (l + 1));
  mName = new (p) char[l + 1];
  for (ymuint i = 0; i < l; ++ i) {
    mName[i] = name[i];
  }
  mName[l] = '\0';
}

// @brief 出力の故障を設定する．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] f 故障
void
AuxNodeInfo::set_output_fault(int val,
			      TpgFault* f)
{
  ASSERT_COND( val == 0 || val == 1 );
  mOutputFaults[val] = f;
}

// @brief 入力数を設定する．
// @param[in] ni 入力数
void
AuxNodeInfo::set_fanin_num(ymuint ni,
			   Alloc& alloc)
{
  mFaninNum = ni;

  ymuint ni2 = ni * 2;
  void* p = alloc.get_memory(sizeof(TpgFault*) * ni2);
  mInputFaults = new (p) TpgFault*[ni2];
  for (ymuint i = 0; i < ni2; ++ i) {
    mInputFaults[i] = nullptr;
  }
}

// @brief 入力の故障を設定する．
// @param[in] ipos 入力位置
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] f 故障
void
AuxNodeInfo::set_input_fault(ymuint ipos,
			     int val,
			     TpgFault* f)
{
  ASSERT_COND( val == 0 || val == 1 );
  ASSERT_COND( ipos < mFaninNum );
  mInputFaults[(ipos * 2) + val] = f;
}

// @brief 故障リストを設定する．
// @param[in] fault_list 故障リスト
// @param[in] alloc メモリアロケータ
void
AuxNodeInfo::set_fault_list(const vector<const TpgFault*>& fault_list,
			    Alloc& alloc)
{
  mFaultNum = fault_list.size();
  void* p = alloc.get_memory(sizeof(const TpgFault*) * mFaultNum);
  mFaultList = new (p) const TpgFault*[mFaultNum];
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    mFaultList[i] = fault_list[i];
  }
}

END_NAMESPACE_YM_SATPG
