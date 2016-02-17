
/// @File TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TpgMap.h"
#include "ym/BnFuncType.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// 指定された型の配列を確保するテンプレート関数
template <typename T>
T*
alloc_array(Alloc& alloc,
	    ymuint n)
{
  void* p = alloc.get_memory(sizeof(T) * n);
  return new (p) T[n];
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
c_val(GateType type,
      Val3 ival)
{
  switch ( type ) {
  case kGateBuff:
    // そのまま返す．
    return ival;

  case kGateNot:
    // 反転して返す．
    return ~ival;

  case kGateAnd:
    // 0 の時のみ 0
    return ival == kVal0 ? kVal0 : kValX;

  case kGateNand:
    // 0 の時のみ 1
    return ival == kVal0 ? kVal1 : kValX;

  case kGateOr:
    // 1 の時のみ 1
    return ival == kVal1 ? kVal1 : kValX;

  case kGateNor:
    // 1 の時のみ 0
    return ival == kVal1 ? kVal0 : kValX;

  case kGateXor:
  case kGateXnor:
    // 常に X
    return kValX;

  case kGateCplx:
    // これは使わない．
    ASSERT_NOT_REACHED;
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

// TFIbits の clear を行う．
void
TFIbits_clear(ymuint64* bits,
	      ymuint size)
{
  for (ymuint i = 0; i < size; ++ i) {
    bits[i] = 0UL;
  }
}

// TFIbits をコピーする．
void
TFIbits_copy(ymuint64* dst_bits,
	     const ymuint64* src_bits,
	     ymuint size)
{
  for (ymuint i = 0; i < size; ++ i) {
    dst_bits[i] = src_bits[i];
  }
}

// TFIbits のすべてのビットをセットする．
void
TFIbits_set_all(ymuint64* bits,
		ymuint size)
{
  for (ymuint i = 0; i < size; ++ i) {
    bits[i] = 0xFFFFFFFFFFFFFFFFUL;
  }
}

// TFIbits の set を行う．
inline
void
TFIbits_set(ymuint64* bits,
	    ymuint pos)
{
  ymuint blk = pos / 64;
  ymuint sft = pos % 64;
  bits[blk] |= (1UL << sft);
}

// TFIbits の bitwise-OR を行う．
inline
void
TFIbits_or(ymuint64* dst_bits,
	   ymuint64* src_bits,
	   ymuint size)
{
  for (ymuint i = 0; i < size; ++ i) {
    dst_bits[i] |= src_bits[i];
  }
}

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

// @brief blif ファイルを読み込んでインスタンスを作る．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @note エラーが起こったら nullptr を返す．
TpgNetwork*
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  BnNetwork bnnetwork;

  bool stat = bnnetwork.read_blif(filename, cell_library);
  if ( !stat ) {
    return nullptr;
  }

  TpgNetwork* network = new TpgNetwork(bnnetwork);

  return network;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @note エラーが起こったら nullptr を返す．
TpgNetwork*
TpgNetwork::read_iscas89(const string& filename)
{
  BnNetwork bnnetwork;

  bool stat = bnnetwork.read_iscas89(filename);
  if ( !stat ) {
    return nullptr;
  }

  TpgNetwork* network = new TpgNetwork(bnnetwork);

  return network;
}

// @brief コンストラクタ
// @param[in] bnnetwork もとのネットワーク
TpgNetwork::TpgNetwork(const BnNetwork& bnnetwork) :
  mAlloc(4096)
{
  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  ymuint nn_orig = bnnetwork.node_num();
  ymuint nl = bnnetwork.logic_num();
  ymuint nn = nn_orig;
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

  mNodeNum = nn;
  mNodeArray = alloc_array<TpgNode>(mAlloc, nn);

  mNodeMap = alloc_array<TpgNode*>(mAlloc, nn_orig);
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

  mMaxNodeId = 0;

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    const BnNode* bnnode = bnnetwork.input(i);
    TpgNode* node = make_input_node(bnnode);
    mInputArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = bnnetwork.sorted_logic(i);
    TpgNode* node = make_logic_node(bnnode, bnnetwork);
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i) {
    const BnNode* bnnode = bnnetwork.output(i);
    TpgNode* node = make_output_node(bnnode);
    mOutputArray[i] = node;
  }

  ASSERT_COND( mNodeNum == nn );


  //////////////////////////////////////////////////////////////////////
  // 故障を作る．
  //////////////////////////////////////////////////////////////////////

  // 全故障数を数える．
  ymuint nfault = 0;
  for (ymuint i = 0; i < nn_orig; ++ i) {
    const BnNode* bnnode = bnnetwork.node(i);
    if ( !bnnode->is_output() ) {
      nfault += (bnnode->fanin_num() + 1) * 2;
    }
  }

  // 故障を生成する．
  mFaultChunk = alloc_array<TpgFault>(mAlloc, nfault);
  mFaultNum = 0;
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = bnnetwork.sorted_logic(nl - i - 1);
    make_faults(bnnode, en_hash);
  }
  for (ymuint i = 0; i < npi; ++ i) {
    const BnNode* bnnode = bnnetwork.input(i);
    make_faults(bnnode, en_hash);
  }

  ASSERT_COND( mFaultNum == nfault );

  // 代表故障のリストを作る．
  ymuint nrep = 0;
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      ++ nrep;
    }
  }
  mRepFaults.reserve(nrep);
  for (ymuint i = 0; i < mFaultNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      mRepFaults.push_back(f);
    }
  }

  check_network_connection(*this);

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
    onode->mFanoutNum = i;
  }

  // 全部アクティブにしておく．
  activate_all();

  // TFIbits を作る．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = &mNodeArray[mNodeNum - i - 1];
    TFIbits_clear(node->mTFIbits, tfibits_size());
    if ( node->is_output() ) {
      // 外部出力の場合は自分自身の番号をセットする．
      TFIbits_set(node->mTFIbits, node->output_id2());
    }
    else {
      // ファンアウト先の TFIbits の OR をとる．
      ymuint nfo = node->fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	TpgNode* onode = node->fanout(i);
	TFIbits_or(node->mTFIbits, onode->mTFIbits, tfibits_size());
      }
    }
  }
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  // このオブジェクトが確保したメモリは mAlloc のデストラクタが
  // 開放してくれる．
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
    TpgNode* node = &mNodeArray[i];
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
      node->mActFanoutNum = 0;
      level_array[node->id()] = 0;
    }
    else {
      sort(tmp_folist.begin(), tmp_folist.end(), FoNodeLt(level_array));
      ymuint act_nfo = tmp_folist.size();
      ymuint min_level = level_array[tmp_folist[0]->id()];
      for (ymuint i = 0; i < act_nfo; ++ i) {
	TpgNode* onode = tmp_folist[i];
	node->mActFanouts[i] = onode;
	ymuint olevel = level_array[onode->id()];
	if ( min_level > olevel ) {
	  min_level = olevel;
	}
      }
      node->mActFanoutNum = act_nfo;
      level_array[node->id()] = min_level + 1;
    }
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      ASSERT_COND( node->is_output() );
      node->mImmDom = nullptr;
    }
    else {
      TpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == nullptr ) {
	  break;
	}
      }
      node->mImmDom = node0;
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
  return &mNodeArray[pos];
}

// @brief 入力ノードを生成する．
// @param[in] iid 入力の番号
// @param[in] name ノード名
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_input_node(ymuint iid,
			    const char* name)
{
  TpgNode* node = new_node();
  init_node(node, 0, 0);
  node->mTypeId = 1U | (iid << 3);

  set_node_name(node, name);

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
  TpgNode* node = new_node();
  init_node(node, 1, 0);
  node->mTypeId = 2U | (oid << 3);

  connect(inode, node, 0);

  set_node_name(node, name);

  return node;
}

// @brief 論理ノードを生成する．
// @param[in] name ノード名
// @param[in] inode_list ファンインのリスト
// @param[in] gate_type ゲートの型
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_logic_node(const char* name,
			    const vector<TpgNode*>& inode_list,
			    GateType gate_type)
{
  ymuint ni = inode_list.size();
  TpgNode* node = make_prim_node(gate_type, true, ni, 0);
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = inode_list[i];
    connect(inode, node, i);
  }
  void* p = mAlloc.get_memory(sizeof(TpgPrimMap));
  node->mInputMap = new (p) TpgPrimMap(node);
  set_node_name(node, name);

  return name;
}

// @brief 論理ノードを生成する．
// @param[in] name ノード名
// @param[in] inode_list ファンインのリスト
// @param[in] expr 論理式
// @return 生成したノードを返す．
//
// 場合によっては複数の TpgNode を生成する．
TpgNode*
TpgNetwork::make_logic_node(const char* name,
			    const vector<TpgNode*>& inode_list,
			    const Expr& expr)
{
  ymuint ni = inode_list.size();
  // expr の内容を表す TpgNode の木を作る．
  vector<TpgNode*> leaf_nodes(ni * 2, nullptr);
  vector<pair<TpgNode*, ymuint> > input_map(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint p_num = expr.litnum(VarId(i), false);
    ymuint n_num = expr.litnum(VarId(i), true);
    TpgNode* inode = inode_list[i];
    if ( n_num == 0 ) {
      if ( p_num == 1 ) {
	leaf_nodes[i * 2 + 0] = inode;
	// ダミー
	input_map[i] = make_pair(static_cast<TpgNode*>(nullptr), 0);
      }
      else {
	TpgNode* dummy_buff = make_prim_node(kGateBuff, false, 1, p_num);
	connect(inode, dummy_buff, 0);
	leaf_nodes[i * 2 + 0] = dummy_buff;
	input_map[i] = make_pair(dummy_buff, 0);
      }
    }
    else {
      if ( p_num > 0 ) {
	TpgNode* dummy_buff = make_prim_node(kGateBuff, false, 1, p_num + 1);
	connect(inode, dummy_buff, 0);
	inode = dummy_buff;
      }

      TpgNode* not_gate = make_prim_node(kGateNot, false, 1, n_num);
      connect(inode, not_gate, 0);
      leaf_nodes[i * 2 + 1] = not_gate;

      if ( p_num > 0 ) {
	input_map[i] = make_pair(inode, 0);
      }
      else {
	input_map[i] = make_pair(not_gate, 0);
      }
    }
  }
  node = make_cplx_node(expr, nfo, leaf_nodes, input_map);
  void* p = mAlloc.get_memory(sizeof(TpgCplxMap));
  TpgCplxMap* tmap = new (p) TpgCplxMap();
  tmap->mInputNodeArray = alloc_array<TpgNode*>(mAlloc, ni);
  tmap->mInputPosArray = alloc_array<ymuint>(mAlloc, ni);
  for (ymuint i = 0; i < ni; ++ i) {
    tmap->mInputNodeArray[i] = input_map[i].first;
    tmap->mInputPosArray[i] = input_map[i].second;
  }
  node->mInputMap = tmap;
  set_node_name(node, name);

  return node;
}

// @brief 論理式から TpgNode の木を生成する．
// @param[in] expr 式
// @param[in] nfo 根のノードのファンアウト数
// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
// @param[in] input_map ファンインの対応関係を収める配列
// @return 生成したノードを返す．
//
// leaf_nodes は 変数番号 * 2 + (0/1) に
// 該当する変数の肯定/否定のリテラルが入っている．
TpgNode*
TpgNetwork::make_cplx_node(const Expr& expr,
			   ymuint nfo,
			   const vector<TpgNode*>& leaf_nodes,
			   vector<pair<TpgNode*, ymuint> >& input_map)
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
    gate_type = kGateAnd;
  }
  else if ( expr.is_or() ) {
    gate_type = kGateOr;
  }
  else if ( expr.is_xor() ) {
    gate_type = kGateXor;
  }
  else {
    ASSERT_NOT_REACHED;
  }

  ymuint nc = expr.child_num();
  vector<TpgNode*> fanins(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Expr& expr1 = expr.child(i);
    TpgNode* inode = make_cplx_node(expr1, 1, leaf_nodes, input_map);
    fanins[i] = inode;
  }
  // fanins[] を確保するオーバーヘッドがあるが，
  // 子供のノードよりも先に親のノードを確保するわけには行かない．
  TpgNode* node = make_prim_node(gate_type, false, nc, nfo);
  for (ymuint i = 0; i < nc; ++ i) {
    TpgNode* inode = fanins[i];
    connect(inode, node, i);
    // 美しくないけどスマートなやり方を思いつかない．
    const Expr& expr1 = expr.child(i);
    if ( expr1.is_posiliteral() ) {
      ymuint iid = expr1.varid().val();
      if ( input_map[iid].first == nullptr ) {
	input_map[iid] = make_pair(node, i);
      }
    }
  }

  return node;
}

// @brief 組み込み型の論理ゲートを生成する．
// @param[in] type ゲートの型
// @param[in] root 根のノードの時 true
// @param[in] ni ファンイン数
// @param[in] nfo ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_prim_node(GateType type,
			   bool root,
			   ymuint ni,
			   ymuint nfo)
{
  TpgNode* node = new_node();
  init_node(node, ni, nfo);
  node->mTypeId = 4U | (static_cast<ymuint>(type) << 3);
  if ( !root ) {
    node->mTypeId |= 1U;
  }

  switch ( type ) {
  case kGateAnd:
  case kGateNand:
    node->mCval = kVal0;
    node->mNval = kVal1;
    break;

  case kGateOr:
  case kGateNor:
    node->mCval = kVal1;
    node->mNval = kVal0;
    break;

  default:
    break;
  }

  switch ( type ) {
  case kGateAnd:
  case kGateNor:
    node->mCOval = kVal0;
    node->mNOval = kVal1;
    break;

  case kGateOr:
  case kGateNand:
    node->mCOval = kVal1;
    node->mNOval = kVal0;
    break;

  default:
    break;
  }

  return node;
}

// @brief TpgNode を初期化する．
// @param[in] ni ファンイン数
// @param[in] nfo ファンアウト数
void
TpgNetwork::init_node(TpgNode* node,
		      ymuint ni,
		      ymuint nfo)
{
  node->mName = nullptr;

  node->mCval = kValX;
  node->mNval = kValX;
  node->mCOval = kValX;
  node->mNOval = kValX;

  node->mFaninNum = ni;
  if ( ni > 0 ) {
    node->mFanins = alloc_array<TpgNode*>(mAlloc, ni);
    node->mInputFault = alloc_array<const TpgFault*>(mAlloc, ni * 2);
  }
  else {
    node->mFanins = nullptr;
    node->mInputFault = nullptr;
  }

  node->mFanoutsSize = nfo;
  node->mFanoutNum = 0;
  node->mFanouts = alloc_array<TpgNode*>(mAlloc, nfo);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_array<TpgNode*>(mAlloc, nfo);

  node->mTFIbits = alloc_array<ymuint64>(mAlloc, tfibits_size());

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = nullptr;
    node->mInputFault[i * 2 + 1] = nullptr;
  }
  node->mOutputFault[0] = nullptr;
  node->mOutputFault[1] = nullptr;

  node->mMarks = 0U;
}

// @brief TpgNode を生成する．
// @return 生成したノードを返す．
//
// 実際には mNodeChunk で確保した領域から返す．
TpgNode*
TpgNetwork::new_node()
{
  ASSERT_COND( mMaxNodeId < mNodeNum );
  TpgNode* node = &mNodeArray[mMaxNodeId];
  node->mId = mMaxNodeId;
  ++ mMaxNodeId;

  return node;
}

// @brief ノード間の接続を行う．
// @param[in] src ソースノード
// @param[in] dst ディスティネーションノード
// @param[in] ipos ファンイン番号
void
TpgNetwork::connect(TpgNode* src,
		    TpgNode* dst,
		    ymuint ipos)
{
  ASSERT_COND( src->mFanoutNum < src->mFanoutsSize );
  dst->mFanins[ipos] = src;
  src->mFanouts[src->mFanoutNum] = dst;
  ++ src->mFanoutNum;
}

// @brief TpgNode と TpgNode の対応付けを行う．
// @param[in] node TpgNode
// @param[in] bnnode もととなる BnNodep
void
TpgNetwork::bind(TpgNode* node,
		 const char* name
		 const BnNode* bnnode)
{
  const char* src_name = bnnode->name();
  mNodeMap[bnnode->id()] = node;
}

// @brief 名前を設定する．
void
TpgNetwork::set_node_name(TpgNode* node,
			  const char* src_name)
{
  char* dst_name = nullptr;
  if ( src_name != nullptr ) {
    ymuint len = strlen(src_name) + 1;
    dst_name = alloc_array<char>(mAlloc, len);
    for (ymuint i = 0; i < len; ++ i) {
      dst_name[i] = src_name[i];
    }
  }
  node->mName = dst_name;
}

void
TpgNetwork::make_faults(const BnNode* bnnode,
			const HashMap<ymuint, CplxInfo*>& en_hash)
{
  const TpgFault* rep0 = nullptr;
  const TpgFault* rep1 = nullptr;

  if ( bnnode->fanout_num() == 1 &&
       !bnnode->fanout(0)->is_output() ) {
    // 唯一のファンアウト先が外部出力でなければ
    // そのファンインブランチの故障と出力の故障
    // は等価
    const BnNode* bnonode = bnnode->fanout(0);
    ymuint ni = bnonode->fanin_num();
    ymuint ipos = ni;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( bnonode->fanin(i) == bnnode ) {
	ipos = i;
	break;
      }
    }
    ASSERT_COND( ipos < ni );
    TpgNode* onode = mNodeMap[bnonode->gid()];
    TpgNode* inode = onode->input_map(ipos);
    ymuint tpg_ipos = onode->ipos_map(ipos);
    rep0 = inode->input_fault(0, tpg_ipos);
    rep1 = inode->input_fault(1, tpg_ipos);
  }

  TpgNode* node = mNodeMap[bnnode->gid()];


  // 出力の故障を生成
  const TpgFault* of0 = new_ofault(node, 0, rep0);
  const TpgFault* of1 = new_ofault(node, 1, rep1);

  ymuint nf = 0;
  if ( of0->is_rep() ) {
    ++ nf;
  }
  if ( of1->is_rep() ) {
    ++ nf;
  }

  ymuint ni = bnnode->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    Val3 oval0;
    Val3 oval1;
    if ( bnnode->is_cplx_logic() ) {
      ymuint fid = bnnode->func_id();
      ASSERT_COND( en_hash.check(fid) );
      const CplxInfo* cinfo = en_hash[fid];
      oval0 = cinfo->mCVal[i * 2 + 0];
      oval1 = cinfo->mCVal[i * 2 + 1];
    }
    else {
      oval0 = c_val(bnnode->gate_type(), kVal0);
      oval1 = c_val(bnnode->gate_type(), kVal1);
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
    const TpgFault* if0 = new_ifault(inode, ipos, 0, rep0);
    const TpgFault* if1 = new_ifault(inode, ipos, 1, rep1);
    if ( if0->is_rep() ) {
      ++ nf;
    }
    if ( if1->is_rep() ) {
      ++ nf;
    }
  }

  // 代表故障を mFaultList に入れる．
  node->mFaultNum = nf;
  node->mFaultList = alloc_array<const TpgFault*>(mAlloc, nf);
  ymuint pos = 0;
  for (ymuint val = 0; val < 2; ++ val) {
    const TpgFault* f = node->mOutputFault[val];
    if ( f->is_rep() ) {
      node->mFaultList[pos] = f;
      ++ pos;
    }
  }
  for (ymuint i = 0; i < ni; ++ i) {
    for (ymuint val = 0; val < 2; ++ val) {
      const TpgFault* f = node->mInputFault[i * 2 + val];
      if ( f->is_rep() ) {
	node->mFaultList[pos] = f;
	++ pos;
      }
    }
  }
  ASSERT_COND( nf == pos );
}

// @brief 出力の故障を作る．
// @param[in] node 故障位置のノード
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] rep 代表故障
//
// 自分自身が代表故障の場合には rep に nullptr を入れる．
const TpgFault*
TpgNetwork::new_ofault(TpgNode* node,
		       ymuint val,
		       const TpgFault* rep)
{
  const TpgFault* f = new_fault(node, true, 0, val, rep);
  node->mOutputFault[val % 2] = f;

  return f;
}

// @brief 入力の故障を作る．
// @param[in] node 故障位置のノード
// @param[in] ipos ファンイン番号 ( 0 <= ipos < node->fanin_num() )
// @param[in] val 故障値
// @param[in] rep 代表故障
//
// 自分自身が代表故障の場合には rep に nullptr を入れる．
const TpgFault*
TpgNetwork::new_ifault(TpgNode* node,
		       ymuint ipos,
		       ymuint val,
		       const TpgFault* rep)
{
  const TpgFault* f = new_fault(node, false, ipos, val, rep);
  node->mInputFault[ipos * 2 + (val % 2)] = f;

  return f;
}

// @brief 故障を生成する．
// @param[in] node 対象のノード
// @param[in] is_output 出力の故障のときに true とするフラグ
// @param[in] ipos 入力の故障の時に入力番号を表す
// @param[in] val 縮退している値
// @param[in] rep 代表故障
const TpgFault*
TpgNetwork::new_fault(TpgNode* node,
		      bool is_output,
		      ymuint ipos,
		      ymuint val,
		      const TpgFault* rep)
{
  TpgFault* f = &mFaultChunk[mFaultNum];
  f->set(mFaultNum, node, is_output, ipos, val, rep);
  ++ mFaultNum;

  return f;
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
  if ( node->name() != nullptr ) {
    s << node->name();
  }
  else {
    s << "[#" << node->id() << "]";
  }
}

END_NAMESPACE_YM_SATPG
