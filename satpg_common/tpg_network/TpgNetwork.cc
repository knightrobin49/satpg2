
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
#include "TpgMap.h"
#include "ym/BnBlifReader.h"
#include "ym/BnIscas89Reader.h"
#include "ym/BnBuilder.h"
#include "ym/Cell.h"
#include "ym/Expr.h"


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

// 指定された型の配列を確保するテンプレート関数
template <typename T>
T*
alloc_array(Alloc& alloc,
	    ymuint n)
{
  void* p = alloc.get_memory(sizeof(T) * n);
  return new (p) T[n];
}

// 文字列用のメモリ領域を確保する関数
const char*
alloc_str(Alloc& alloc,
	  const string& src_str)
{
  if ( src_str == string() ) {
    return nullptr;
  }

  ymuint l = src_str.size();
  void* p = alloc.get_memory(sizeof(char) * (l + 1));
  char* d = new (p) char[l + 1];
  for (ymuint i = 0; i < l; ++ i) {
    d[i] = src_str[i];
  }
  d[l] = '\0';
  return d;
}

// immediate dominator リストをマージする．
TpgNode*
merge(TpgNode* node1,
      TpgNode* node2)
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

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TpgNetwork::TpgNetwork() :
  mAlloc(4096)
{
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  // このオブジェクトが確保したメモリは mAlloc のデストラクタが
  // 開放してくれる．
}

// @brief blif ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @return 読み込みが成功したら true を返す．
bool
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  BnBuilder builder;
  BnBlifReader reader;

  bool stat = reader.read(builder, filename, cell_library);
  if ( stat ) {
    set(builder);
  }

  return stat;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @return 読み込みが成功したら true を返す．
bool
TpgNetwork::read_iscas89(const string& filename)
{
  BnBuilder builder;
  BnIscas89Reader reader;

  bool stat = reader.read(builder, filename);
  if ( stat ) {
    set(builder);
  }

  return stat;
}

BEGIN_NONAMESPACE

void
get_mffc_elem(TpgNode* node,
	      vector<bool>& mark,
	      vector<TpgNode*>& tmp_list)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;
  if ( node->imm_dom() != nullptr && node->ffr_root() == node ) {
    tmp_list.push_back(node);
  }

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->imm_dom() != nullptr ) {
      get_mffc_elem(inode, mark, tmp_list);
    }
  }
}

END_NONAMESPACE

// @brief 内容を設定する．
// @param[in] builder ビルダーオブジェクト
void
TpgNetwork::set(const BnBuilder& builder)
{
  mAlloc.destroy();

  //////////////////////////////////////////////////////////////////////
  // NodeInfoMgr にノードの論理関数を登録する．
  //////////////////////////////////////////////////////////////////////
  TpgNodeInfoMgr node_info_mgr;
  ymuint nexpr = builder.expr_num();
  vector<const TpgNodeInfo*> node_info_list(nexpr);
  ymuint extra_node_num = 0;
  for (ymuint i = 0; i < nexpr; ++ i) {
    Expr expr = builder.expr(i);
    ymuint ni = expr.input_size();
    const TpgNodeInfo* node_info = node_info_mgr.complex_type(ni, expr);
    node_info_list[i] = node_info;
  }

  //////////////////////////////////////////////////////////////////////
  // 追加で生成されるノード数を数える．
  //////////////////////////////////////////////////////////////////////
  ymuint nl = builder.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const BnBuilder::NodeInfo src_node_info = builder.logic(i);
    BnLogicType logic_type = src_node_info.mLogicType;
    if ( logic_type == kBnLt_EXPR ) {
      const TpgNodeInfo* node_info = node_info_list[src_node_info.mFuncId];
      extra_node_num += node_info->extra_node_num();
    }
  }

  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////
  mInputNum = builder.input_num();
  mOutputNum = builder.output_num();
  mFFNum = builder.dff_num();

  ymuint nn = mInputNum + mOutputNum + mFFNum + mFFNum + nl + extra_node_num;

  mNodeArray = alloc_array<TpgNode*>(mAlloc, nn);

  mInputArray = alloc_array<TpgNode*>(mAlloc, input_num2());
  mOutputArray = alloc_array<TpgNode*>(mAlloc, output_num2());
  mOutputArray2 = alloc_array<TpgNode*>(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_array<TpgNode*>(mAlloc, nn);

  mTmpNodeNum = 0;
  mTmpNodeList = alloc_array<TpgNode*>(mAlloc, nn);

  mTmpMark = alloc_array<bool>(mAlloc, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    mTmpMark[i] = false;
  }

  TpgNodeMap node_map;

  mNodeNum = 0;
  mFaultNum = 0;

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mInputNum; ++ i) {
    const BnBuilder::NodeInfo& node_info = builder.input(i);
    TpgNode* node = make_input_node(i, node_info.mName);
    mInputArray[i] = node;
    node_map.reg(node_info.mId, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < nl; ++ i) {
    const BnBuilder::NodeInfo& src_node_info = builder.logic(i);
    const TpgNodeInfo* node_info = nullptr;
    BnLogicType logic_type = src_node_info.mLogicType;
    if ( logic_type == kBnLt_EXPR ) {
      node_info = node_info_list[src_node_info.mFuncId];
    }
    else {
      ASSERT_COND( logic_type != kBnLt_TV );
      GateType gate_type = conv_to_gate_type(logic_type);
      node_info = node_info_mgr.simple_type(gate_type);
    }

    // ファンインのノードを取ってくる．
    ymuint ni = src_node_info.mFaninList.size();
    vector<TpgNode*> fanin_array(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      fanin_array[j] = node_map.get(src_node_info.mFaninList[j]);
    }
    TpgNode* node = make_logic_node(src_node_info.mName, node_info, fanin_array);

    // ノードを登録する．
    node_map.reg(src_node_info.mId, node);
  }

  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mOutputNum; ++ i) {
    const BnBuilder::NodeInfo& src_node_info = builder.output(i);
    TpgNode* inode = node_map.get(src_node_info.mFaninList[0]);
    string buf = "*";
    buf += src_node_info.mName;
    TpgNode* node = make_output_node(i, buf.c_str(), inode);
    mOutputArray[i] = node;
  }

  ASSERT_COND( mNodeNum == nn );


  // ファンアウトの情報をセットする．
  set_fanouts();


  //////////////////////////////////////////////////////////////////////
  // 代表故障を求める．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    // ノードごとに代表故障を設定する．
    // この処理は出力側から行う必要がある．
    TpgNode* node = mNodeArray[mNodeNum - i - 1];
    set_rep_faults(node);
  }


  //////////////////////////////////////////////////////////////////////
  // TFI のサイズの昇順に並べた出力順を
  // mOutputArray2 に記録する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  vector<pair<ymuint, ymuint> > tmp_list(npo);
  for (ymuint i = 0; i < npo; ++ i) {
    TpgNode* onode = output(i);
    // onode の TFI のノード数を計算する．
    tfimark(onode);
    ymuint n = mTmpNodeNum;
    clear_tfimark();
    tmp_list[i] = make_pair(n, i);
  }

  // TFI のサイズの昇順にソートする．
  sort(tmp_list.begin(), tmp_list.end(), Lt());

  // tmp_list の順に mOutputArray2 にセットする．
  for (ymuint i = 0; i < npo; ++ i) {
    ymuint opos = tmp_list[i].second;
    TpgNode* onode = mOutputArray[opos];
    mOutputArray2[i] = onode;
    onode->set_output_id2(i);
  }

  // 全部アクティブにしておく．
  activate_all();

  // MFFC 内の FFR の情報をセットしておく．
  vector<bool> mark(node_num(), false);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( node->imm_dom() == nullptr ) {
      // node を根とする MFFC 内の FFR の根のノードのリストを作る．
      vector<TpgNode*> tmp_list;
      tmp_list.push_back(node);
      get_mffc_elem(node, mark, tmp_list);
      ymuint n = tmp_list.size();
      void* p = mAlloc.get_memory(sizeof(TpgNode*) * n);
      TpgNode** root_list = new (p) TpgNode*[n];
      for (ymuint i = 0; i < n; ++ i) {
	root_list[i] = tmp_list[i];
      }
      ASSERT_COND( root_list[0] == node );
      node->set_root_list(n, root_list);
    }
  }
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po_pos 出力番号
void
TpgNetwork::activate_po(ymuint po_pos)
{
  // po_pos は output2() 上の位置
  TpgNode* onode = output2(po_pos);

  activate_po(onode);
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po 出力ノード
void
TpgNetwork::activate_po(TpgNode* po)
{
  ASSERT_COND( po->is_output() );

  // po から到達可能なノードにマークをつける．
  tfimark(po);

  activate_sub();
}

// @brief 全てのノードをアクティブにする．
void
TpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    tfimark(output(i));
  }

  activate_sub();
}

// @brief ノードの TFI にマークをつける．
// @note 結果は mTmpMark[node->id()] に格納される．
void
TpgNetwork::tfimark(TpgNode* node)
{
  if ( mTmpMark[node->id()] ) {
    return;
  }

  mTmpMark[node->id()] = true;
  mTmpNodeList[mTmpNodeNum] = node;
  ++ mTmpNodeNum;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    tfimark(node->fanin(i));
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

// @brief activate_po(), activate_all() の下請け関数
void
TpgNetwork::activate_sub()
{
  vector<ymuint> level_array(mNodeNum);

  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( mTmpMark[i] ) {
      node->set_active();
      mActNodeArray[mActNodeNum] = node;
      ++ mActNodeNum;
    }
    else {
      node->clear_active();
    }
  }

  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    ymuint id = mActNodeNum - i - 1;
    TpgNode* node = mActNodeArray[id];

    // ファンアウトをPOまでのレベルの小さい順にならべる．
    ymuint nfo = node->fanout_num();
    vector<TpgNode*> tmp_folist;
    tmp_folist.reserve(nfo);
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* onode = node->fanout(i);
      if ( mTmpMark[onode->id()] ) {
	tmp_folist.push_back(onode);
      }
    }
    if ( tmp_folist.empty() ) {
      ASSERT_COND( node->is_output() );
      node->set_active_fanouts(tmp_folist);
      level_array[node->id()] = 0;
    }
    else {
      sort(tmp_folist.begin(), tmp_folist.end(), FoNodeLt(level_array));
      ymuint act_nfo = tmp_folist.size();
      ymuint min_level = level_array[tmp_folist[0]->id()];
      for (ymuint i = 0; i < act_nfo; ++ i) {
	TpgNode* onode = tmp_folist[i];
	ymuint olevel = level_array[onode->id()];
	if ( min_level > olevel ) {
	  min_level = olevel;
	}
      }
      node->set_active_fanouts(tmp_folist);
      level_array[node->id()] = min_level + 1;
    }
  }

  // FFR の情報をセットする．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      ASSERT_COND( node->is_output() );
      node->set_ffr_root(node);
    }
    else if ( nfo == 1 ) {
      TpgNode* onode = node->active_fanout(0);
      node->set_ffr_root(onode->ffr_root());
    }
    else {
      node->set_ffr_root(node);
    }
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      ASSERT_COND( node->is_output() );
      node->set_imm_dom(nullptr);
    }
    else {
      TpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == nullptr ) {
	  break;
	}
      }
      node->set_imm_dom(node0);
    }
  }

  // マークを消す．
  clear_tfimark();

  mMffcNum = 0;
  mFfrNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( node->imm_dom() == nullptr ) {
      ++ mMffcNum;
    }
    if ( node->fanout_num() > 1 || node->is_output() ) {
      ++ mFfrNum;
    }
  }
}

// @brief TFI マークを消す．
// @note この関数が終了すると mTmpNodeNum は 0 になる．
void
TpgNetwork::clear_tfimark()
{
  for (ymuint i = 0; i < mTmpNodeNum; ++ i) {
    ASSERT_COND( mTmpMark[mTmpNodeList[i]->id()] );
    mTmpMark[mTmpNodeList[i]->id()] = false;
  }
  mTmpNodeNum = 0;
}

// @brief 入力ノードを生成する．
// @param[in] iid 入力の番号
// @param[in] name ノード名
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_input_node(ymuint iid,
			    const string& name)
{
  const char* d_name = alloc_str(mAlloc, name);
  TpgNode* node = TpgNode::new_input(mAlloc, mNodeNum, d_name, iid);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  // 出力位置の故障を生成
  new_ofault(d_name, 0, node);
  new_ofault(d_name, 1, node);

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] oid 出力の番号
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_output_node(ymuint oid,
			     const string& name,
			     TpgNode* inode)
{
  const char* d_name = alloc_str(mAlloc, name);
  TpgNode* node = TpgNode::new_output(mAlloc, mNodeNum, d_name, oid, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  // 入力位置の故障を生成
  new_ifault(d_name, 0, 0, node, 0, nullptr);
  new_ifault(d_name, 1, 0, node, 0, nullptr);

  return node;
}

// @brief 論理ノードを生成する．
// @param[in] src_name ノード名
// @param[in] node_info 論理関数の情報
// @param[in] fanin_list ファンインのリスト
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_logic_node(const string& src_name,
			    const TpgNodeInfo* node_info,
			    const vector<TpgNode*>& fanin_list)
{
  ymuint ni = fanin_list.size();

  // 複合型の場合の入力ノードを納める配列
  // TpgMap の要素として使われるので開放してはいけない．
  TpgNode** inode_array = nullptr;
  ymuint* ipos_array = nullptr;

  const char* name = alloc_str(mAlloc, src_name);

  GateType gate_type = node_info->gate_type();
  TpgNode* node = nullptr;
  if ( gate_type != kGateCPLX ) {
    // 組み込み型の場合．
    node = make_prim_node(name, gate_type, fanin_list);
  }
  else {
    Expr expr = node_info->expr();

    inode_array = alloc_array<TpgNode*>(mAlloc, ni);
    ipos_array = alloc_array<ymuint>(mAlloc, ni);

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
	  // ダミー
	  // 実際の内容は make_cplx_node() で設定される．
	  inode_array[i] = nullptr;
	  ipos_array[i] = 0;
	}
	else {
	  // 肯定のリテラルが2回以上現れている場合
	  // ブランチの故障に対応するためにダミーのバッファをつくる．
	  TpgNode* dummy_buff = make_prim_node(nullptr, kGateBUFF, vector<TpgNode*>(1, inode));
	  leaf_nodes[i * 2 + 0] = dummy_buff;
	  // このバッファの入力が故障位置となる．
	  inode_array[i] = dummy_buff;
	  ipos_array[i] = 0;
	}
      }
      else {
	if ( p_num > 0 ) {
	  // 肯定と否定のリテラルがともに現れる場合
	  // ブランチの故障に対応するためにダミーのバッファを作る．
	  TpgNode* dummy_buff = make_prim_node(nullptr, kGateBUFF, vector<TpgNode*>(1, inode));
	  inode = dummy_buff;
	  leaf_nodes[i * 2 + 0] = dummy_buff;
	}

	// 否定のリテラルに対応するNOTゲートを作る．
	TpgNode* not_gate = make_prim_node(nullptr, kGateNOT, vector<TpgNode*>(1, inode));
	leaf_nodes[i * 2 + 1] = not_gate;

	if ( p_num > 0 ) {
	  inode_array[i] = inode;
	  ipos_array[i] = 0;
	}
	else {
	  inode_array[i] = not_gate;
	  ipos_array[i] = 0;
	}
      }
    }

    // expr の内容を表す TpgNode の木を作る．
    node = make_cplx_node(name, expr, leaf_nodes, inode_array, ipos_array);

    // 元のゲートの入力との対応関係を作る．
    void* p = mAlloc.get_memory(sizeof(TpgMap));
    TpgMap* tmap = new (p) TpgMap(inode_array, ipos_array);
    node->set_tmap(tmap);
  }

  // 出力位置の故障を生成
  new_ofault(name, 0, node);
  new_ofault(name, 1, node);

  // 入力位置の故障を生成
  for (ymuint i = 0; i < ni; ++ i) {
    Val3 oval0 = node_info->cval(i, kVal0);
    Val3 oval1 = node_info->cval(i, kVal1);

    const TpgFault* rep0 = nullptr;
    if ( oval0 == kVal0 ) {
      rep0 = node->output_fault(0);
    }
    else if ( oval0 == kVal1 ) {
      rep0 = node->output_fault(1);
    }

    const TpgFault* rep1 = nullptr;
    if ( oval1 == kVal0 ) {
      rep1 = node->output_fault(0);
    }
    else if ( oval1 == kVal1 ) {
      rep1 = node->output_fault(1);
    }

    TpgNode* inode = (gate_type == kGateCPLX) ? inode_array[i] : node;
    ymuint ipos = (gate_type == kGateCPLX) ? ipos_array[i] : i;
    new_ifault(name, 0, i, inode, ipos, rep0);
    new_ifault(name, 1, i, inode, ipos, rep1);
  }

  return node;
}

// @brief 論理式から TpgNode の木を生成する．
// @param[in] name ノード名
// @param[in] expr 式
// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
// @param[in] input_map ファンインの対応関係を収める配列
// @return 生成したノードを返す．
//
// leaf_nodes は 変数番号 * 2 + (0/1) に
// 該当する変数の肯定/否定のリテラルが入っている．
TpgNode*
TpgNetwork::make_cplx_node(const string& name,
			   const Expr& expr,
			   const vector<TpgNode*>& leaf_nodes,
			   TpgNode** inode_array,
			   ymuint* ipos_array)
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
    TpgNode* inode = make_cplx_node(nullptr, expr1, leaf_nodes, inode_array, ipos_array);
    ASSERT_COND( inode != nullptr );
    fanins[i] = inode;
  }
  // fanins[] を確保するオーバーヘッドがあるが，
  // 子供のノードよりも先に親のノードを確保するわけには行かない．
  TpgNode* node = make_prim_node(name, gate_type, fanins);

  // オペランドがリテラルの場合，inode_array[], ipos_array[]
  // の設定を行う．
  for (ymuint i = 0; i < nc; ++ i) {
    // 美しくないけどスマートなやり方を思いつかない．
    const Expr& expr1 = expr.child(i);
    if ( expr1.is_posiliteral() ) {
      ymuint iid = expr1.varid().val();
      if ( inode_array[iid] == nullptr ) {
	inode_array[iid] = node;
	ipos_array[iid] = i;
      }
    }
  }

  return node;
}

// @brief 組み込み型の論理ゲートを生成する．
// @param[in] name ノード名
// @param[in] type ゲートの型
// @param[in] ni ファンイン数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_prim_node(const string& name,
			   GateType type,
			   const vector<TpgNode*>& fanin_list)
{
  const char* c_name = alloc_str(mAlloc, name);
  ymuint ni = fanin_list.size();
  if ( ni > 2 ) {
    // 2入力以上の XOR/XNOR ゲートを2入力に分解する．
    if ( type == kGateXOR ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = fanin_list[0];
      tmp_list[1] = fanin_list[1];
      TpgNode* tmp_node = make_prim_node(nullptr, type, tmp_list);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = fanin_list[i];
	const char* tmp_name = (i == ni - 1) ? c_name : nullptr;
	tmp_node = make_prim_node(tmp_name, type, tmp_list);
      }
      return tmp_node;
    }
    else if ( type == kGateXNOR ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = fanin_list[0];
      tmp_list[1] = fanin_list[1];
      TpgNode* tmp_node = make_prim_node(nullptr, kGateXOR, tmp_list);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = fanin_list[i];
	if ( i < ni - 1 ) {
	  tmp_node = make_prim_node(nullptr, kGateXOR, tmp_list);
	}
	else {
	  tmp_node = make_prim_node(name, kGateXNOR, tmp_list);
	}
      }
      return tmp_node;
    }
  }

  TpgNode* node = TpgNode::new_primitive(mAlloc, mNodeNum, c_name, type, fanin_list);

  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  return node;
}

BEGIN_NONAMESPACE

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

void
TpgNetwork::set_fanouts()
{
  //////////////////////////////////////////////////////////////////////
  // ファンアウト数を数える
  //////////////////////////////////////////////////////////////////////
  vector<ymuint> nfo_array(mNodeNum, 0);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgNode* inode = node->fanin(j);
      ++ nfo_array[inode->id()];
    }
  }

  //////////////////////////////////////////////////////////////////////
  // ファンアウト用の配列を確保する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    ymuint nfo = nfo_array[node->id()];
    nfo_array[node->id()] = 0;
    TpgNode** fo_array = alloc_array<TpgNode*>(mAlloc, nfo);
    TpgNode** act_fo_array = alloc_array<TpgNode*>(mAlloc, nfo);
    node->set_fanout_num(nfo, fo_array, act_fo_array);
  }

  //////////////////////////////////////////////////////////////////////
  // ファンアウトをセットする．
  //////////////////////////////////////////////////////////////////////
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

  // 接続が正しいかチェックする．
  check_network_connection(*this);
}

// @brief 出力の故障を作る．
// @param[in] name 故障位置のノード名
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] node 故障位置のノード
const TpgFault*
TpgNetwork::new_ofault(const string& name,
		       int val,
		       TpgNode* node)
{
  const char* c_name = alloc_str(mAlloc, name);
  void* p = mAlloc.get_memory(sizeof(TpgStemFault));
  TpgFault* f = new (p) TpgStemFault(mFaultNum, c_name, val, node, nullptr);
  node->set_output_fault(val, f);
  ++ mFaultNum;

  return f;
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
const TpgFault*
TpgNetwork::new_ifault(const string& name,
		       int val,
		       ymuint ipos,
		       TpgNode* node,
		       ymuint inode_pos,
		       const TpgFault* rep)
{
  const char* c_name = alloc_str(mAlloc, name);
  TpgNode* inode = node->fanin(inode_pos);
  void* p = mAlloc.get_memory(sizeof(TpgBranchFault));
  TpgFault* f = new (p) TpgBranchFault(mFaultNum, c_name, val, ipos, node, inode, inode_pos, rep);
  node->set_input_fault(val, inode_pos, f);
  ++ mFaultNum;

  return f;
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

    TpgFault* rep0 = onode->input_fault(0, ipos);
    TpgFault* of0 = node->output_fault(0);
    if ( of0 != nullptr ) {
      of0->set_rep(rep0);
    }

    TpgFault* rep1 = onode->input_fault(1, ipos);
    TpgFault* of1 = node->output_fault(1);
    if ( of1 != nullptr ){
      of1->set_rep(rep1);
    }
  }

  if ( !node->is_output() ) {
    TpgFault* of0 = node->output_fault(0);
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

    TpgFault* of1 = node->output_fault(1);
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
    TpgFault* if0 = node->input_fault(0, i);
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

    TpgFault* if1 = node->input_fault(1, i);
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

  // 代表故障を TpgNode::mFaultList に入れる．
  ymuint nf = fault_list.size();
  const TpgFault** f_list = alloc_array<const TpgFault*>(mAlloc, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = fault_list[i];
    f_list[i] = f;
  }
  node->set_fault_list(nf, f_list);
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
    print_node(s, node);
    s << ": ";
    if ( node->is_input() ) {
      s << "INPUT#" << node->input_id();
    }
    else if ( node->is_output() ) {
      s << "OUTPUT#" << node->output_id();
      const TpgNode* inode = node->fanin(0);
      s << " = ";
      print_node(s, inode);
    }
    else if ( node->is_logic() ) {
      s << node->gate_type();
      ymuint ni = node->fanin_num();
      if ( ni > 0 ) {
	s << "(";
	for (ymuint j = 0; j < ni; ++ j) {
	  const TpgNode* inode = node->fanin(j);
	  s << " ";
	  print_node(s, inode);
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
	   const TpgNode* node)
{
  s << "NODE#" << node->id() << ": ";
  if ( node->name() != nullptr ) {
    s << node->name();
  }
}

END_NAMESPACE_YM_SATPG
