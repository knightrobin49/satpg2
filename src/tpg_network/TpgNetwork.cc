
/// @File TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgNodeMap.h"
#include "TpgInput.h"
#include "TpgOutput.h"
#include "TpgFault.h"
#include "TpgOutputFault.h"
#include "TpgInputFault.h"
#include "TpgMap.h"
#include "ym/BnFuncType.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// BnFuncType を GateType に変換する．
inline
GateType
conv_to_gate_type(BnFuncType::Type type)
{
  switch ( type ) {
  case BnFuncType::kFt_C0:   return kGateCONST0;
  case BnFuncType::kFt_C1:   return kGateCONST1;
  case BnFuncType::kFt_BUFF: return kGateBUFF;
  case BnFuncType::kFt_NOT:  return kGateNOT;
  case BnFuncType::kFt_AND:  return kGateAND;
  case BnFuncType::kFt_NAND: return kGateNAND;
  case BnFuncType::kFt_OR:   return kGateOR;
  case BnFuncType::kFt_NOR:  return kGateNOR;
  case BnFuncType::kFt_XOR:  return kGateXOR;
  case BnFuncType::kFt_XNOR: return kGateXNOR;
  case BnFuncType::kFt_EXPR: return kGateCPLX;
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
	  const char* src_str)
{
  if ( src_str == nullptr ) {
    return nullptr;
  }

  ymuint l = strlen(src_str);
  void* p = alloc.get_memory(sizeof(char) * (l + 1));
  char* d = new (p) char[l + 1];
  for (ymuint i = 0; i < l; ++ i) {
    d[i] = src_str[i];
  }
  d[l] = '\0';
  return d;
}

// 論理式中の演算子の数を数える．
ymuint
count_expr(const Expr& expr)
{
  if ( !expr.is_op() ) {
    return 0;
  }

  ymuint n = 1;
  ymuint nc = expr.child_num();
  for (ymuint i = 0; i < nc; ++ i) {
    n += count_expr(expr.child(i));
  }
  return n;
}

// 追加で必要なノード数を数える．
ymuint
extra_node_count(const Expr& expr,
		 ymuint ni)
{
  // まず入力部分に挿入されるノード数を数える．
  ymuint n = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint p_num = expr.litnum(VarId(i), false);
    ymuint n_num = expr.litnum(VarId(i), true);
    ASSERT_COND( p_num > 0 || n_num > 0 );
    if ( n_num == 0 ) {
      if ( p_num > 1 ) {
	n = 1;
      }
    }
    else {
      n = 1;
      if ( p_num > 0 ) {
	++ n;
      }
    }
  }

  // 次に論理式自体で必要となるノード数を数える．
  // ただし，根のノードはカウント済みなので1つ減らす．
  n += count_expr(expr) - 1;

  return n;
}

// calc_c_val の下請け関数
Val3
ccv_sub(const Expr& expr,
	const vector<Val3>& ivals)
{
  if ( expr.is_zero() ) {
    return kVal0;
  }
  if ( expr.is_one() ) {
    return kVal1;
  }
  if ( expr.is_posiliteral() ) {
    ymuint iid = expr.varid().val();
    return ivals[iid];
  }
  if ( expr.is_negaliteral() ) {
    ymuint iid = expr.varid().val();
    return ~ivals[iid];
  }

  ymuint nc = expr.child_num();
  if ( expr.is_and() ) {
    bool has_x = false;
    for (ymuint i = 0; i < nc; ++ i) {
      Val3 ival = ccv_sub(expr.child(i), ivals);
      if ( ival == kVal0 ) {
	return kVal0;
      }
      if ( ival == kValX ) {
	has_x = true;
      }
    }
    if ( has_x ) {
      return kValX;
    }
    return kVal1;
  }

  if ( expr.is_or() ) {
    bool has_x = false;
    for (ymuint i = 0; i < nc; ++ i) {
      Val3 ival = ccv_sub(expr.child(i), ivals);
      if ( ival == kVal1 ) {
	return kVal1;
      }
      if ( ival == kValX ) {
	has_x = true;
      }
    }
    if ( has_x ) {
      return kValX;
    }
    return kVal0;
  }

  if ( expr.is_or() ) {
    Val3 val = kVal0;
    for (ymuint i = 0; i < nc; ++ i) {
      Val3 ival = ccv_sub(expr.child(i), ivals);
      if ( ival == kValX ) {
	return kValX;
      }
      val = val ^ ival;
    }
    return val;
  }

  ASSERT_NOT_REACHED;
  return kValX;
}

// 制御値の計算を行う．
Val3
calc_c_val(const Expr& expr,
	   ymuint ni,
	   ymuint ipos,
	   Val3 val)
{
  vector<Val3> ivals(ni, kValX);
  ivals[ipos] = val;
  return ccv_sub(expr, ivals);
}

// 制御値の計算を行う．
Val3
c_val(BnFuncType::Type type,
      Val3 ival)
{
  switch ( type ) {
  case BnFuncType::kFt_BUFF:
    // そのまま返す．
    return ival;

  case BnFuncType::kFt_NOT:
    // 反転して返す．
    return ~ival;

  case BnFuncType::kFt_AND:
    // 0 の時のみ 0
    return ival == kVal0 ? kVal0 : kValX;

  case BnFuncType::kFt_NAND:
    // 0 の時のみ 1
    return ival == kVal0 ? kVal1 : kValX;

  case BnFuncType::kFt_OR:
    // 1 の時のみ 1
    return ival == kVal1 ? kVal1 : kValX;

  case BnFuncType::kFt_NOR:
    // 1 の時のみ 0
    return ival == kVal1 ? kVal0 : kValX;

  case BnFuncType::kFt_XOR:
  case BnFuncType::kFt_XNOR:
    // 常に X
    return kValX;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return kValX;
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
  BnNetwork bnnetwork;

  bool stat = bnnetwork.read_blif(filename, cell_library);
  if ( stat ) {
    set(bnnetwork);
  }

  return stat;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @return 読み込みが成功したら true を返す．
bool
TpgNetwork::read_iscas89(const string& filename)
{
  BnNetwork bnnetwork;

  bool stat = bnnetwork.read_iscas89(filename);
  if ( stat ) {
    set(bnnetwork);
  }

  return stat;
}

// @brief 内容を設定する．
// @param[in] bnnetwork もとのネットワーク
void
TpgNetwork::set(const BnNetwork& bnnetwork)
{
  mAlloc.destroy();

  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  ymuint bn_nn = bnnetwork.node_num();
  ymuint nl = bnnetwork.logic_num();
  ymuint nn = bn_nn;
  HashMap<ymuint, CplxInfo*> en_hash;
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = bnnetwork.logic(i);
    const BnFuncType* func_type = bnnode->func_type();
    if ( func_type->type() == BnFuncType::kFt_EXPR ) {
      ymuint fid = func_type->id();
      CplxInfo* cinfo;
      if ( en_hash.find(fid, cinfo) ) {
	// すでに同じ論理式を処理済みの場合は
	// 以前の結果を利用する．
	nn += cinfo->mExtraNodeCount;
      }
      else {
	// 論理式を取り出す．
	Expr expr = func_type->expr();
	ymuint ni = bnnode->fanin_num();
	// 追加で必要となるノード数を計算する．
	ymuint n = extra_node_count(expr, ni);
	cinfo = new CplxInfo(n, ni);
	// 入力の制御値を計算する．
	for (ymuint j = 0; j < ni; ++ j) {
	  cinfo->mCVal[j * 2 + 0] = calc_c_val(expr, ni, j, kVal0);
	  cinfo->mCVal[j * 2 + 1] = calc_c_val(expr, ni, j, kVal1);
	}
	// ハッシュに登録する．
	en_hash.add(fid, cinfo);
	nn += n;
      }
    }
  }

  mInputNum = bnnetwork.input_num();
  mOutputNum = bnnetwork.output_num();
  mFFNum = bnnetwork.dff_num();

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

  mNodeNum = 0;

  TpgNodeMap node_map(bn_nn);

  // 全故障数を数える．
  ymuint nfault = 0;
  for (ymuint i = 0; i < bn_nn; ++ i) {
    const BnNode* bnnode = bnnetwork.node(i);
    nfault += (bnnode->fanin_num() + 1) * 2;
  }

  // 故障用の配列を用意する．
  mFaultNum = 0;
  mFaultList = alloc_array<TpgFault*>(mAlloc, nfault);


  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    const BnNode* bnnode = bnnetwork.input(i);
    TpgNode* node = make_input_node(i, bnnode->name());
    mInputArray[i] = node;
    node_map.reg(bnnode->id(), node);
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  vector<const BnNode*> sorted_node_list;
  bnnetwork.topological_sort(sorted_node_list);
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = sorted_node_list[i];
    ymuint ni = bnnode->fanin_num();
    vector<TpgNode*> fanin_array(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      fanin_array[j] = node_map.get(bnnode->fanin_id(j));
    }
    const BnFuncType* func_type = bnnode->func_type();
    GateType gate_type = conv_to_gate_type(func_type->type());
    TpgNode* node;
    if ( gate_type == kGateCPLX ) {
      node = make_logic_node(bnnode->name(), func_type->expr(), fanin_array);
    }
    else {
      node = make_logic_node(bnnode->name(), gate_type, fanin_array);
    }
    node_map.reg(bnnode->id(), node);
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i) {
    const BnNode* bnnode = bnnetwork.output(i);
    TpgNode* inode = node_map.get(bnnode->inode_id());
    TpgNode* node = make_output_node(i, bnnode->name(), inode);
    mOutputArray[i] = node;
    node_map.reg(bnnode->id(), node);
  }

  ASSERT_COND( mNodeNum == nn );
  ASSERT_COND( mFaultNum == nfault );


  //////////////////////////////////////////////////////////////////////
  // ファンアウト数を数える
  //////////////////////////////////////////////////////////////////////
  vector<ymuint> nfo_array(mNodeNum, 0);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( node->is_logic() || node->is_output() ) {
      for (ymuint j = 0; j < node->fanin_num(); ++ j) {
	TpgNode* inode = node->fanin(j);
	++ nfo_array[inode->id()];
      }
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

    // これは別件だけどここで一緒にやる．
    node->set_tfibits(alloc_array<ymuint64>(mAlloc, tfibits_size()));
  }

  //////////////////////////////////////////////////////////////////////
  // ファンアウトをセットする．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[i];
    if ( node->is_logic() || node->is_output() ) {
      for (ymuint j = 0; j < node->fanin_num(); ++ j) {
	TpgNode* inode = node->fanin(j);
	ymuint& fo_pos = nfo_array[inode->id()];
	inode->set_fanout(fo_pos, node);
	++ fo_pos;
      }
    }
  }

  check_network_connection(*this);


  //////////////////////////////////////////////////////////////////////
  // 代表故障のリストを作る．
  //////////////////////////////////////////////////////////////////////

  ymuint nrep = 0;
  for (ymuint i = 0; i < npo; ++ i) {
    const BnNode* bnnode = bnnetwork.output(i);
    nrep += make_repfaults(bnnode, bnnetwork, node_map, en_hash);
  }
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = sorted_node_list[nl - i - 1];
    nrep += make_faults(bnnode, bnnetwork, node_map, en_hash);
  }
  for (ymuint i = 0; i < npi; ++ i) {
    const BnNode* bnnode = bnnetwork.input(i);
    nrep += make_faults(bnnode, bnnetwork, node_map, en_hash);
  }

  // 全代表故障のリストを作る．
  mRepFaults.reserve(nrep);
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    TpgFault* f = mFaultList[i];
    if ( f->is_rep() ) {
      mRepFaults.push_back(f);
    }
  }

  // TFI のサイズの昇順に並べた出力順を
  // mOutputArray2 に記録する．
  vector<pair<ymuint, ymuint> > tmp_list(output_num2());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    TpgNode* onode = output(i);
    tfimark(onode);
    ymuint n = mTmpNodeNum;
    clear_tfimark();
    tmp_list[i] = make_pair(n, i);
  }

  sort(tmp_list.begin(), tmp_list.end(), Lt());

  for (ymuint i = 0; i < output_num2(); ++ i) {
    TpgNode* onode = mOutputArray[tmp_list[i].second];
    mOutputArray2[i] = onode;
    onode->set_output_id2(i);
  }

  // 全部アクティブにしておく．
  activate_all();

  // TFIbits を作る．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = mNodeArray[mNodeNum - i - 1];
    node->tfibits_clear(tfibits_size());
    if ( node->is_output() ) {
      // 外部出力の場合は自分自身の番号をセットする．
      node->tfibits_bitset(node->output_id2());
    }
    else {
      // ファンアウト先の TFIbits の OR をとる．
      ymuint nfo = node->fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	TpgNode* onode = node->fanout(i);
	node->tfibits_or(onode, tfibits_size());
      }
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

// @brief ノードを得る．
const TpgNode*
TpgNetwork::node(ymuint pos) const
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  ASSERT_COND( pos < mNodeNum );
  return mNodeArray[pos];
}

// @brief 入力ノードを生成する．
// @param[in] iid 入力の番号
// @param[in] name ノード名
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_input_node(ymuint iid,
			    const char* name)
{
  const char* d_name = alloc_str(mAlloc, name);
  void* p = mAlloc.get_memory(sizeof(TpgInput));
  TpgNode* node = new (p) TpgInput(mNodeNum, d_name, iid);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  // 出力位置の故障を生成
  new_ofault(d_name, node, 0);
  new_ofault(d_name, node, 1);

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] oid 出力の番号
// @param[in] name ノード名
// @param[in] inode 入力のノード
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_output_node(ymuint oid,
			     const char* name,
			     TpgNode* inode)
{
  const char* d_name = alloc_str(mAlloc, name);
  void* p = mAlloc.get_memory(sizeof(TpgOutput));
  TpgNode* node = new (p) TpgOutput(mNodeNum, d_name, oid, inode);
  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  // 入力位置の故障を生成
  new_ifault(d_name, node, 0, node, 0, 0);
  new_ifault(d_name, node, 0, node, 0, 1);

  return node;
}

// @brief 組み込み型の論理ノードを生成する．
// @param[in] name ノード名
// @param[in] gate_type ゲートの種類
// @param[in] inode_list ファンインのリスト
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_logic_node(const char* name,
			    GateType gate_type,
			    const vector<TpgNode*>& inode_list)
{
  const char* d_name = alloc_str(mAlloc, name);
  TpgNode* node = make_prim_node(d_name, gate_type, inode_list);

  // 出力位置の故障を生成
  new_ofault(d_name, node, 0);
  new_ofault(d_name, node, 1);

  // 入力位置の故障を生成
  ymuint ni = inode_list.size();
  for (ymuint i = 0; i < ni; ++ i) {
    new_ifault(d_name, node, i, node, i, 0);
    new_ifault(d_name, node, i, node, i, 1);
  }

  return node;
}

// @brief 複合型の論理ノードを生成する．
// @param[in] name ノード名
// @param[in] expr ノードの論理式
// @param[in] inode_list ファンインのリスト
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_logic_node(const char* name,
			    const Expr& expr,
			    const vector<TpgNode*>& inode_list)
{
  ymuint ni = inode_list.size();

  // expr の内容を表す TpgNode の木を作る．
  vector<TpgNode*> leaf_nodes(ni * 2, nullptr);
  TpgNode** inode_array = alloc_array<TpgNode*>(mAlloc, ni);
  ymuint* ipos_array = alloc_array<ymuint>(mAlloc, ni);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint p_num = expr.litnum(VarId(i), false);
    ymuint n_num = expr.litnum(VarId(i), true);
    TpgNode* inode = inode_list[i];
    if ( n_num == 0 ) {
      if ( p_num == 1 ) {
	leaf_nodes[i * 2 + 0] = inode;
	// ダミー
	inode_array[i] = nullptr;
	ipos_array[i] = 0;
      }
      else {
	TpgNode* dummy_buff = make_prim_node(nullptr, kGateBUFF, vector<TpgNode*>(1, inode));
	leaf_nodes[i * 2 + 0] = dummy_buff;
	inode_array[i] = dummy_buff;
	ipos_array[i] = 0;
      }
    }
    else {
      if ( p_num > 0 ) {
	TpgNode* dummy_buff = make_prim_node(nullptr, kGateBUFF, vector<TpgNode*>(1, inode));
	inode = dummy_buff;
      }

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

  const char* d_name = alloc_str(mAlloc, name);
  TpgNode* node = make_cplx_node(d_name, expr, leaf_nodes, inode_array, ipos_array);

  void* p = mAlloc.get_memory(sizeof(TpgMap));
  TpgMap* tmap = new (p) TpgMap(inode_array, ipos_array);
  node->set_tmap(tmap);

  // 出力位置の故障を生成
  new_ofault(d_name, node, 0);
  new_ofault(d_name, node, 1);

  // 入力位置の故障を生成
  for (ymuint i = 0; i < ni; ++ i) {
    new_ifault(d_name, node, i, inode_array[i], ipos_array[i], 0);
    new_ifault(d_name, node, i, inode_array[i], ipos_array[i], 1);
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
TpgNetwork::make_cplx_node(const char* name,
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

  ymuint nc = expr.child_num();
  vector<TpgNode*> fanins(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Expr& expr1 = expr.child(i);
    TpgNode* inode = make_cplx_node(nullptr, expr1, leaf_nodes, inode_array, ipos_array);
    fanins[i] = inode;
  }
  // fanins[] を確保するオーバーヘッドがあるが，
  // 子供のノードよりも先に親のノードを確保するわけには行かない．
  TpgNode* node = make_prim_node(name, gate_type, fanins);

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
TpgNetwork::make_prim_node(const char* name,
			   GateType type,
			   const vector<TpgNode*>& inode_list)
{
  ymuint ni = inode_list.size();
  if ( ni > 2 ) {
    // 2入力以上の XOR/XNOR ゲートを2入力に分解する．
    if ( type == kGateXOR ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = inode_list[0];
      tmp_list[1] = inode_list[1];
      TpgNode* tmp_node = make_prim_node(nullptr, type, tmp_list);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = inode_list[i];
	const char* tmp_name = (i == ni - 1) ? name : nullptr;
	tmp_node = make_prim_node(tmp_name, type, tmp_list);
      }
      return tmp_node;
    }
    else if ( type == kGateXNOR ) {
      vector<TpgNode*> tmp_list(2);
      tmp_list[0] = inode_list[0];
      tmp_list[1] = inode_list[1];
      TpgNode* tmp_node = make_prim_node(nullptr, kGateXOR, tmp_list);
      for (ymuint i = 2; i < ni; ++ i) {
	tmp_list[0] = tmp_node;
	tmp_list[1] = inode_list[i];
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

  TpgNode* node = TpgNode::new_primitive(mAlloc, mNodeNum, name, type, inode_list);

  mNodeArray[mNodeNum] = node;
  ++ mNodeNum;

  return node;
}

#if 0
ymuint
TpgNetwork::make_faults(const BnNode* bnnode,
			const BnNetwork& bnnetwork,
			const TpgNodeMap& node_map,
			const HashMap<ymuint, CplxInfo*>& en_hash)
{
  const TpgFault* rep0 = nullptr;
  const TpgFault* rep1 = nullptr;

  if ( bnnode->fanout_num() == 1 ) {
    const BnNode* bnonode = bnnetwork.node(bnnode->fanout_id(0));
    // 唯一のファンアウト先が一つならば
    // そのファンインブランチの故障と出力の故障
    // は等価
    TpgNode* onode = node_map.get(bnonode->id());
    const TpgNode* inode = nullptr;
    ymuint tpg_ipos = 0;
    if ( bnonode->is_output() ) {
      inode = onode;
    }
    else {
      ymuint ni = bnonode->fanin_num();
      ymuint ipos = ni;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( bnonode->fanin_id(i) == bnnode->id() ) {
	  ipos = i;
	  break;
	}
      }
      ASSERT_COND( ipos < ni );
      inode = onode->input_map(ipos);
      tpg_ipos = onode->ipos_map(ipos);
    }
    rep0 = inode->input_fault(0, tpg_ipos);
    rep1 = inode->input_fault(1, tpg_ipos);
  }

  TpgNode* node = node_map.get(bnnode->id());

  // node に関係する代表故障のリスト
  vector<const TpgFault*> fault_list;

  const TpgFault* of0 = nullptr;
  const TpgFault* of1 = nullptr;
  if ( !bnnode->is_output() ) {
    // 出力の故障を生成
    of0 = new_ofault(bnnode, node, 0, rep0);
    of1 = new_ofault(bnnode, node, 1, rep1);

    if ( of0->is_rep() ) {
      fault_list.push_back(of0);
    }
    if ( of1->is_rep() ) {
      fault_list.push_back(of1);
    }
  }

  // 入力の故障を生成
  if ( bnnode->is_logic() ) {
    const BnFuncType* func_type = bnnode->func_type();
    ymuint ni = bnnode->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      Val3 oval0;
      Val3 oval1;
      if ( func_type->type() == BnFuncType::kFt_EXPR ) {
	ymuint fid = func_type->id();
	ASSERT_COND( en_hash.check(fid) );
	const CplxInfo* cinfo = en_hash[fid];
	oval0 = cinfo->mCVal[i * 2 + 0];
	oval1 = cinfo->mCVal[i * 2 + 1];
      }
      else {
	oval0 = c_val(func_type->type(), kVal0);
	oval1 = c_val(func_type->type(), kVal1);
      }

      const TpgFault* rep0 = nullptr;
      if ( oval0 == kVal0 ) {
	rep0 = of0;
      }
      else if ( oval0 == kVal1 ) {
	rep0 = of1;
      }

      const TpgFault* rep1 = nullptr;
      if ( oval1 == kVal0 ) {
	rep1 = of0;
      }
      else if ( oval1 == kVal1 ) {
	rep1 = of1;
      }

      TpgNode* inode = node->input_map(i);
      ymuint ipos = node->ipos_map(i);
      const TpgFault* if0 = new_ifault(bnnode, node, i, inode, ipos, 0, rep0);
      const TpgFault* if1 = new_ifault(bnnode, node, i, inode, ipos, 1, rep1);
      if ( if0->is_rep() ) {
	fault_list.push_back(if0);
      }
      if ( if1->is_rep() ) {
	fault_list.push_back(if1);
      }
    }
  }
  else if ( bnnode->is_output() ) {
    const TpgFault* f0 = new_ifault(bnnode, node, 0, node, 0, 0, nullptr);
    const TpgFault* f1 = new_ifault(bnnode, node, 0, node, 0, 1, nullptr);
    fault_list.push_back(f0);
    fault_list.push_back(f1);
  }

  // 代表故障を mFaultList に入れる．
  ymuint nf = fault_list.size();
  const TpgFault** f_list = alloc_array<const TpgFault*>(mAlloc, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    f_list[i] = fault_list[i];
  }
  node->set_fault_list(nf, f_list);

  return nf;
}

// @brief 出力の故障を作る．
// @param[in] bnnode 故障位置の BnNode
// @param[in] tpgnode 故障位置の TpgNode
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] rep 代表故障
//
// 自分自身が代表故障の場合には rep に nullptr を入れる．
const TpgFault*
TpgNetwork::new_ofault(const BnNode* bnnode,
		       TpgNode* tpgnode,
		       ymuint val,
		       const TpgFault* rep)
{
  const char* name = alloc_str(mAlloc, bnnode->name());
  void* p = mAlloc.get_memory(sizeof(TpgOutputFault));
  TpgFault* f = new (p) TpgOutputFault(mFaultNum, name, tpgnode, val, rep);
  tpgnode->set_output_fault(val, f);
  mFaultList[mFaultNum] = f;
  ++ mFaultNum;

  return f;
}

// @brief 入力の故障を作る．
// @param[in] bnnode 故障位置の BnNode
// @param[in] tpgnode 故障位置の TpgNode
// @param[in] ipos ファンイン番号 ( 0 <= ipos < node->fanin_num() )
// @param[in] i_tpgnode 入力側の TpgNode
// @param[in] tpgpos i_tpgnode 上の入力位置
// @param[in] val 故障値
// @param[in] rep 代表故障
//
// 自分自身が代表故障の場合には rep に nullptr を入れる．
const TpgFault*
TpgNetwork::new_ifault(const BnNode* bnnode,
		       TpgNode* tpgnode,
		       ymuint ipos,
		       TpgNode* i_tpgnode,
		       ymuint tpgpos,
		       ymuint val,
		       const TpgFault* rep)
{
  const char* name = alloc_str(mAlloc, bnnode->name());
  void* p = mAlloc.get_memory(sizeof(TpgInputFault));
  TpgFault* f = new (p) TpgInputFault(mFaultNum, name, tpgnode, ipos, i_tpgnode, tpgpos, val, rep);
  tpgnode->set_input_fault(val, tpgpos, f);
  mFaultList[mFaultNum] = f;
  ++ mFaultNum;

  return f;
}
#endif

// @brief 出力の故障を作る．
// @param[in] name 故障位置のノード名
// @param[in] node 故障位置のノード
// @param[in] val 故障値 ( 0 / 1 )
const TpgFault*
TpgNetwork::new_ofault(const char* name,
		       TpgNode* node,
		       int val)
{
  void* p = mAlloc.get_memory(sizeof(TpgOutputFault));
  TpgFault* f = new (p) TpgOutputFault(mFaultNum, name, node, val, nullptr);
  node->set_output_fault(val, f);
  mFaultList[mFaultNum] = f;
  ++ mFaultNum;

  return f;
}

// @brief 入力の故障を作る．
// @param[in] name 故障位置のノード名
// @param[in] node 故障位置のノード
// @param[in] ipos ファンイン番号 ( 0 <= ipos < node->fanin_num() )
// @param[in] inode 入力側のノード
// @param[in] inode_pos inode 上の入力位置
// @param[in] val 故障値
const TpgFault*
TpgNetwork::new_ifault(const char* name,
		       TpgNode* node,
		       ymuint ipos,
		       TpgNode* inode,
		       ymuint inode_pos,
		       int val)
{
  void* p = mAlloc.get_memory(sizeof(TpgInputFault));
  TpgFault* f = new (p) TpgInputFault(mFaultNum, name, node, ipos, inode, indoe_pos, val, nullptr);
  inode->set_input_fault(val, inode_pos, f);
  mFaultList[mFaultNum] = f;
  ++ mFaultNum;

  return f;
}

ymuint
TpgNetwork::make_repfaults(TpgNode* node,
			   const TpgNodeMap& node_map,
			   const HashMap<ymuint, CplxInfo*>& en_hash)
{
  const TpgFault* rep0 = nullptr;
  const TpgFault* rep1 = nullptr;

  if ( node->fanout_num() == 1 ) {
    TpgNode* onode = node->fanout(0);
    // 唯一のファンアウト先が一つならば
    // そのファンインブランチの故障と出力の故障
    // は等価
    ymuint ni = bnonode->fanin_num();
    ymuint ipos = ni;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( onode->fanin(i) == node ) {
	ipos = i;
	break;
      }
    }
    ASSERT_COND( ipos < ni );
    rep0 = onode->input_fault(0, ipos);
    rep1 = onode->input_fault(1, ipos);
  }

  // node に関係する代表故障のリスト
  vector<const TpgFault*> fault_list;

  const TpgFault* of0 = nullptr;
  const TpgFault* of1 = nullptr;
  if ( !node->is_output() ) {
    of0 = node->output_fault(0);
    of1 = node->output_fault(1);

    if ( rep0 != nullptr ) {
      of0->set_rep(rep0);
    }
    else {
      fault_list.push_back(of0);
    }

    if ( rep1 != nullptr ) {
      of1->set_rep(rep1);
    }
    else {
      fault_list.push_back(of1);
    }
  }

  // 入力の故障を生成
  if ( bnnode->is_logic() ) {
    const BnFuncType* func_type = bnnode->func_type();
    ymuint ni = bnnode->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      Val3 oval0;
      Val3 oval1;
      if ( func_type->type() == BnFuncType::kFt_EXPR ) {
	ymuint fid = func_type->id();
	ASSERT_COND( en_hash.check(fid) );
	const CplxInfo* cinfo = en_hash[fid];
	oval0 = cinfo->mCVal[i * 2 + 0];
	oval1 = cinfo->mCVal[i * 2 + 1];
      }
      else {
	oval0 = c_val(func_type->type(), kVal0);
	oval1 = c_val(func_type->type(), kVal1);
      }

      const TpgFault* rep0 = nullptr;
      if ( oval0 == kVal0 ) {
	rep0 = of0;
      }
      else if ( oval0 == kVal1 ) {
	rep0 = of1;
      }

      const TpgFault* rep1 = nullptr;
      if ( oval1 == kVal0 ) {
	rep1 = of0;
      }
      else if ( oval1 == kVal1 ) {
	rep1 = of1;
      }

      TpgNode* inode = node->input_map(i);
      ymuint ipos = node->ipos_map(i);
      const TpgFault* if0 = new_ifault(bnnode, node, i, inode, ipos, 0, rep0);
      const TpgFault* if1 = new_ifault(bnnode, node, i, inode, ipos, 1, rep1);
      if ( if0->is_rep() ) {
	fault_list.push_back(if0);
      }
      if ( if1->is_rep() ) {
	fault_list.push_back(if1);
      }
    }
  }
  else if ( bnnode->is_output() ) {
    const TpgFault* f0 = node->input_fault(0, 0);
    const TpgFault* f1 = node->input_fault(1, 0);
    fault_list.push_back(f0);
    fault_list.push_back(f1);
  }

  // 代表故障を TpgNode::mFaultList に入れる．
  ymuint nf = fault_list.size();
  const TpgFault** f_list = alloc_array<const TpgFault*>(mAlloc, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    f_list[i] = fault_list[i];
  }
  node->set_fault_list(nf, f_list);

  return nf;
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
