
/// @file TpgBlifHandler.cc
/// @brief TpgBlifHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.



#include "TpgBlifHandler.h"
#include "TpgNetwork.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
TpgBlifHandler::TpgBlifHandler()
{
}

// @brief デストラクタ
TpgBlifHandler::~TpgBlifHandler()
{
}

// @brief ネットワークをセットする．
void
TpgBlifHandler::set_network(TpgNetwork& network)
{
  mNetwork = &network;
}

// @brief 初期化
bool
TpgBlifHandler::init()
{
  mNetwork->clear();
  mConList.clear();
  return true;
}

// @brief .model 文の処理
// @param[in] loc1 .model の位置情報
// @param[in] loc2 文字列の位置情報
// @param[in] name 文字列
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TpgBlifHandler::model(const FileRegion& loc1,
		      const FileRegion& loc2,
		      const char* name)
{
  return true;
}

// @brief .input 文の読み込み
bool
TpgBlifHandler::inputs_elem(ymuint name_id)
{
  const char* name = id2str(name_id);
  TpgNode* node = mNetwork->find_node(name, true);
  mNetwork->set_to_input(node);
  return true;
}

// @brief .output 文の読み込み
bool
TpgBlifHandler::outputs_elem(ymuint name_id)
{
  TpgNode* onode = mNetwork->new_node();
  mNetwork->set_to_output(onode);
  const char* name = id2str(name_id);
  TpgNode* node = mNetwork->find_node(name, true);
  connect(node, onode, 0);
  return true;
}

// @brief .names 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 各識別子のID番号の配列
// @param[in] cover カバー
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TpgBlifHandler::names(ymuint onode_id,
		      const vector<ymuint>& inode_id_array,
		      const BlifCover* cover)
{
  ymuint ni = inode_id_array.size();

  mCurFanins.clear();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint id = inode_id_array[i];
    const char* name = id2str(id);
    TpgNode* node = mNetwork->find_node(name, true);
    mCurFanins.push_back(node);
  }

  TpgNode* node = mNetwork->find_node(id2str(onode_id), true);

  mChd2.reserve(ni);

  Expr expr;
  mChd1.clear();
  if ( opat == '1' ) {
    for (ymuint c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(Expr::make_negaliteral(VarId(i)));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(Expr::make_posiliteral(VarId(i)));
	}
      }
      mChd1.push_back(Expr::make_and(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = Expr::make_one();
    }
    else {
      expr = Expr::make_or(mChd1);
    }
  }
  else {
    for (ymuint c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(Expr::make_posiliteral(VarId(i)));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(Expr::make_negaliteral(VarId(i)));
	}
      }
      mChd1.push_back(Expr::make_or(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = Expr::make_zero();
    }
    else {
      expr = Expr::make_and(mChd1);
    }
  }

  mNetwork->set_to_logic(node, expr);

  if ( ni != node->fanin_num() ) {
    // .names の後の要素数とパタンの列数が異なる．
    ostringstream buf;
    buf << node->name() << ": illegal cover" << endl;
    MsgMgr::put_msg(__FILE__, __LINE__,
		    id2loc(onode_id),
		    kMsgError,
		    "TG_BLIF_005",
		    buf.str());
    return false;
  }
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = mCurFanins[i];
    connect(inode, node, i);
  }
  return true;
}

// @brief .gate 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 入力ノードのID番号の配列
// @param[in] cell セル
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TpgBlifHandler::gate(ymuint onode_id,
		     const vector<ymuint>& inode_id_array,
		     const Cell* cell)
{
  return true;
}

// @brief .latch 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id 入力ノードのID番号
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TpgBlifHandler::latch(ymuint onode_id,
		      ymuint inode_id,
		      const FileRegion& loc4,
		      char rval)
{
  const char* oname = id2str(onode_id);
  TpgNode* ffout = mNetwork->find_node(oname, true);
  TpgNode* ffin = mNetwork->new_node();
  mNetwork->set_to_ff(ffin, ffout);
  const char* iname = id2str(inode_id);
  TpgNode* inode = mNetwork->find_node(iname, true);
  connect(inode, ffin, 0);

  // rval は使っていない．
  return true;
}

// @brief .end 文の処理
bool
TpgBlifHandler::end(const FileRegion& loc)
{
  for (vector<Connection>::iterator p = mConList.begin();
       p != mConList.end(); ++ p) {
    const Connection& con = *p;
    mNetwork->connect(con.mFrom, con.mTo, con.mPos);
  }
  mConList.clear();

  ymuint n = mNetwork->node_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = mNetwork->node(i);
    if ( node->is_undef() ) {
      // node は定義されていない
      ostringstream buf;
      buf << node->name() << ": undefined";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "TG_BLIF_03",
		      buf.str());
      return false;
    }
  }

  return true;
}

// @brief 終了処理
void
TpgBlifHandler::normal_exit()
{
  mNetwork->wrap_up();
}

// @brief エラー終了時の処理
void
TpgBlifHandler::error_exit()
{
  mNetwork->clear();
}

// 可能なら接続する．
// できれなれば mConList に積む
void
TpgBlifHandler::connect(TpgNode* from,
			TpgNode* to,
			ymuint pos)
{
  if ( from->is_undef() ) {
    mConList.push_back(Connection(from, to, pos));
  }
  else {
    mNetwork->connect(from, to, pos);
  }
}

END_NAMESPACE_YM_SATPG
