
/// @file TpgNode.cc
/// @brief TpgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNode.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief GateType のストリーム演算子
ostream&
operator<<(ostream& s,
	   GateType gate_type)
{
  switch ( gate_type ) {
  case kGateCONST0: s << "CONST-0"; break;
  case kGateCONST1: s << "CONST-1"; break;
  case kGateBUFF:   s << "BUFF"; break;
  case kGateNOT:    s << "NOT"; break;
  case kGateAND:    s << "AND"; break;
  case kGateNAND:   s << "NAND"; break;
  case kGateOR:     s << "OR"; break;
  case kGateNOR:    s << "NOR"; break;
  case kGateXOR:    s << "XOR"; break;
  case kGateXNOR:   s << "XNOR"; break;
  default:          s << "---"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス TpgNode
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードを作る．
// @param[in] id ノード番号
// @param[in] iid 入力番号
// @param[in] fanout_num ファンアウト数
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_input(ymuint id,
		    ymuint iid,
		    ymuint fanout_num)
{
  TpgNode* node = make_node(id, 0, fanout_num);
  node->mTypeId = (1U | (iid << 2));

  return node;
}

// @brief 出力ノードを作る．
// @param[in] id ノード番号
// @param[in] oid 出力番号
// @param[in] inode 入力ノード
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_output(ymuint id,
		     ymuint oid,
		     TpgNode* inode)
{
  TpgNode* node = make_node(id, 1, 0);
  node->mTypeId = (2U | (oid << 2));
  node->mNodeList[0] = inode;

  return node;
}

// @brief 論理ノードを作る．
// @param[in] id ノード番号
// @param[in] gate_type ゲートタイプ
// @param[in] inode_list 入力ノードのリスト
// @param[in] fanout_num ファンアウト数
// @return 作成したノードを返す．
TpgNode*
TpgNode::make_logic(ymuint id,
		    GateType gate_type,
		    const vector<TpgNode*>& inode_list,
		    ymuint fanout_num)
{
  ymuint ni = inode_list.size();
  TpgNode* node = make_node(id, ni, fanout_num);
  node->mTypeId = (3U | (static_cast<ymuint32>(gate_type) << 2));
  for (ymuint i = 0; i < ni; ++ i) {
    node->mNodeList[i] = inode_list[i];
  }

  return node;
}

// @brief ノードを生成する
// @param[in] id ID番号
// @param[in] fanin_num ファンイン数
// @param[in] fanout_num ファンアウト数
TpgNode*
TpgNode::make_node(ymuint id,
		   ymuint fanin_num,
		   ymuint fanout_num)
{
  ymuint size = sizeof(TpgNode) + (fanin_num + fanout_num - 1) * sizeof(TpgNode*);
  void* p = new char[size];
  TpgNode* node = new (p) TpgNode;
  node->mId = id;
  node->mFaninNum = fanin_num;
  node->mFanoutNum = fanout_num;

  return node;
}

// @brief ノードを削除する．
void
TpgNode::delete_node(TpgNode* node)
{
  char* p = reinterpret_cast<char*>(node);
  delete [] p;
}

// @brief コンストラクタ
TpgNode::TpgNode()
{
}

// @brief デストラクタ
TpgNode::~TpgNode()
{
}

// @brief ID番号を得る．
ymuint
TpgNode::id() const
{
  return mId;
}

// @brief controling value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::cval() const
{
  return kValX;
}

// @brief noncontroling valueを得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::nval() const
{
  return kValX;
}

// @brief controling output value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::coval() const
{
  return kValX;;
}

// @brief noncontroling output value を得る．
// @note ない場合は kValX を返す．
Val3
TpgNode::noval() const
{
  return kValX;
}

// @brief 入出力の関係を表す CNF 式を生成する．
// @param[in] solver SAT ソルバ
// @param[in] lit_map 入出力とリテラルの対応マップ
void
TpgNode::make_cnf(SatSolver& solver,
		  const LitMap& lit_map) const
{
  if ( is_input() ) {
    //cout << "INPUT" << endl;
    // なにもしない．
    return;
  }

  SatLiteral olit = lit_map.output();
  if ( is_output() ) {
    //cout << "OUTPUT" << endl;
    SatLiteral ilit = lit_map.input(0);
    solver.add_clause( ilit, ~olit);
    solver.add_clause(~ilit,  olit);

    return;
  }

  // is_logic()
  ymuint ni = fanin_num();
  switch ( gate_type() ) {
  case kGateCONST0:
    //cout << "CONST0" << endl;
    solver.add_clause(~olit);
    break;

  case kGateCONST1:
    //cout << "CONST1" << endl;
    solver.add_clause( olit);
    break;

  case kGateBUFF:
    //cout << "BUFF" << endl;
    {
      SatLiteral ilit = lit_map.input(0);
      solver.add_clause(~ilit,  olit);
      solver.add_clause( ilit, ~olit);
    }
    break;

  case kGateNOT:
    //cout << "NOT" << endl;
    {
      SatLiteral ilit = lit_map.input(0);
      solver.add_clause( ilit,  olit);
      solver.add_clause(~ilit, ~olit);
    }
    break;

  case kGateAND:
    //cout << "AND" << endl;
    switch ( ni ) {
    case 2:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	solver.add_clause(~ilit0, ~ilit1,  olit);
	solver.add_clause( ilit0,         ~olit);
	solver.add_clause(         ilit1, ~olit);
      }
      break;

    case 3:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	solver.add_clause(~ilit0, ~ilit1, ~ilit2,  olit);
	solver.add_clause( ilit0,                 ~olit);
	solver.add_clause(         ilit1,         ~olit);
	solver.add_clause(                 ilit2, ~olit);
      }
      break;

    case 4:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	SatLiteral ilit3 = lit_map.input(3);
	solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~ilit3,  olit);
	solver.add_clause( ilit0,                         ~olit);
	solver.add_clause(         ilit1,                 ~olit);
	solver.add_clause(                 ilit2,         ~olit);
	solver.add_clause(                         ilit3, ~olit);
      }
      break;

    default:
      {
	vector<SatLiteral> tmp_lits(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  SatLiteral ilit = lit_map.input(i);
	  tmp_lits[i] = ~ilit;
	}
	tmp_lits[ni] = olit;
	solver.add_clause(tmp_lits);
	for (ymuint i = 0; i < ni; ++ i) {
	  SatLiteral ilit = lit_map.input(i);
	  solver.add_clause( ilit, ~olit);
	}
      }
      break;
    }
    break;

  case kGateNAND:
    //cout << "NAND" << endl;
    switch ( ni ) {
    case 2:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	solver.add_clause(~ilit0, ~ilit1, ~olit);
	solver.add_clause( ilit0,          olit);
	solver.add_clause(         ilit1,  olit);
      }
      break;

    case 3:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~olit);
	solver.add_clause( ilit0,                  olit);
	solver.add_clause(         ilit1,          olit);
	solver.add_clause(                 ilit2,  olit);
      }
      break;

    case 4:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	SatLiteral ilit3 = lit_map.input(3);
	solver.add_clause(~ilit0, ~ilit1, ~ilit2, ~ilit3, ~olit);
	solver.add_clause( ilit0,                          olit);
	solver.add_clause(         ilit1,                  olit);
	solver.add_clause(                 ilit2,          olit);
	solver.add_clause(                         ilit3,  olit);
      }
      break;

    default:
      {
	ymuint ni = fanin_num();
	vector<SatLiteral> tmp_lits(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  SatLiteral ilit = lit_map.input(i);
	  solver.add_clause( ilit, olit);
	  tmp_lits[i] = ~ilit;
	}
	tmp_lits[ni] = ~olit;
	solver.add_clause(tmp_lits);
      }
      break;
    }
    break;

  case kGateOR:
    //cout << "OR" << endl;
    switch ( ni ) {
    case 2:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	solver.add_clause( ilit0,  ilit1, ~olit);
	solver.add_clause(~ilit0,          olit);
	solver.add_clause(        ~ilit1,  olit);
      }
      break;

    case 3:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	solver.add_clause( ilit0,  ilit1,  ilit2, ~olit);
	solver.add_clause(~ilit0,                  olit);
	solver.add_clause(        ~ilit1,          olit);
	solver.add_clause(                ~ilit2,  olit);
      }
      break;

    case 4:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	SatLiteral ilit3 = lit_map.input(3);
	solver.add_clause( ilit0,  ilit1,  ilit2,  ilit3, ~olit);
	solver.add_clause(~ilit0,                          olit);
	solver.add_clause(        ~ilit1,                  olit);
	solver.add_clause(                ~ilit2,          olit);
	solver.add_clause(                        ~ilit3,  olit);
      }
      break;

    default:
      {
	ymuint ni = fanin_num();
	vector<SatLiteral> tmp_lits(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  SatLiteral ilit = lit_map.input(i);
	  solver.add_clause(~ilit, olit);
	  tmp_lits[i] = ilit;
	}
	tmp_lits[ni] = ~olit;
	solver.add_clause(tmp_lits);
      }
      break;
    }
    break;

  case kGateNOR:
    //cout << "NOR" << endl;
    switch ( ni ) {
    case 2:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	solver.add_clause( ilit0,  ilit1,  olit);
	solver.add_clause(~ilit0,         ~olit);
	solver.add_clause(        ~ilit1, ~olit);
      }
      break;

    case 3:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	solver.add_clause( ilit0,  ilit1,  ilit2,  olit);
	solver.add_clause(~ilit0,                 ~olit);
	solver.add_clause(        ~ilit1,         ~olit);
	solver.add_clause(                ~ilit2, ~olit);
      }
      break;

    case 4:
      {
	SatLiteral ilit0 = lit_map.input(0);
	SatLiteral ilit1 = lit_map.input(1);
	SatLiteral ilit2 = lit_map.input(2);
	SatLiteral ilit3 = lit_map.input(3);
	solver.add_clause( ilit0,  ilit1,  ilit2,  ilit3,  olit);
	solver.add_clause(~ilit0,                         ~olit);
	solver.add_clause(        ~ilit1,                 ~olit);
	solver.add_clause(                ~ilit2,         ~olit);
	solver.add_clause(                        ~ilit3, ~olit);
      }
      break;

    default:
      {
	ymuint ni = fanin_num();
	vector<SatLiteral> tmp_lits(ni + 1);
	for (ymuint i = 0; i < ni; ++ i) {
	  SatLiteral ilit = lit_map.input(i);
	  solver.add_clause(~ilit, ~olit);
	  tmp_lits[i] = ilit;
	}
	tmp_lits[ni] = olit;
	solver.add_clause(tmp_lits);
      }
      break;
    }
    break;

  case kGateXOR:
    //cout << "XOR" << endl;
    {
      ymuint ni = fanin_num();
      ASSERT_COND( ni == 2 );
      SatLiteral ilit0 = lit_map.input(0);
      SatLiteral ilit1 = lit_map.input(1);
      solver.add_clause(~ilit0,  ilit1,  olit);
      solver.add_clause( ilit0, ~ilit1,  olit);
      solver.add_clause( ilit0,  ilit1, ~olit);
      solver.add_clause(~ilit0, ~ilit1, ~olit);
    }
    break;

  case kGateXNOR:
    //cout << "XNOR" << endl;
    {
      ymuint ni = fanin_num();
      ASSERT_COND( ni == 2 );
      SatLiteral ilit0 = lit_map.input(0);
      SatLiteral ilit1 = lit_map.input(1);
      solver.add_clause(~ilit0,  ilit1, ~olit);
      solver.add_clause( ilit0, ~ilit1, ~olit);
      solver.add_clause( ilit0,  ilit1,  olit);
      solver.add_clause(~ilit0, ~ilit1,  olit);
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
}

// @brief 入出力の関係を表す CNF 式を生成する(故障あり)．
// @param[in] solver SAT ソルバ
// @param[in] fpos 故障のある入力位置
// @param[in] fval 故障値 ( 0 / 1 )
// @param[in] lit_map 入出力とリテラルの対応マップ
//
// こちらは入力に故障を仮定したバージョン
void
TpgNode::make_faulty_cnf(SatSolver& solver,
			 ymuint fpos,
			 int fval,
			 const LitMap& lit_map) const
{
  ASSERT_NOT_REACHED;
}

// @brief 出力番号2をセットする．
// @param[in] id セットする番号
//
// 出力ノード以外では無効
void
TpgNode::set_output_id2(ymuint id)
{
  ASSERT_COND( is_output() );
  mOutputId2 = id;
}

// @brief ファンアウトを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
// @param[in] fo_node ファンアウト先のノード
void
TpgNode::set_fanout(ymuint pos,
		    TpgNode* fo_node)
{
  ASSERT_COND( pos < fanout_num() );
  mNodeList[mFaninNum + pos] = fo_node;
}

// @brief immediate dominator をセットする．
// @param[in] dom dominator ノード
void
TpgNode::set_imm_dom(const TpgNode* dom)
{
  mImmDom = dom;
}

// @brief MFFC 内の根のノードの情報をセットする．
// @param[in] num 要素数
// @param[in] node_list ノードのリスト
void
TpgNode::set_mffc_info(ymuint num,
		       TpgNode** node_list)
{
  mMffcElemNum = num;
  mMffcElemList = node_list;
}

END_NAMESPACE_YM_SATPG
