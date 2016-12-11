
/// @file SimNodeTest.cc
/// @brief SimNodeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "satpg.h"
#include "SimNode.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

class SimNodeTest :
  public ::testing::Test
{
public:

  /// @brief 入力ノードのテストを行う．
  void
  test_input();

  /// @brief 論理ノードのテストを行う．
  /// @param[in] ni 入力数
  /// @param[in] gate_type ゲートの種類
  /// @param[in] vals 真理値表ベクタ
  void
  test_gate(ymuint ni,
	    GateType gate_type,
	    int vals[]);

  /// @brief gval の書き込み読み出しテスト
  /// @param[in] node 対象のノード
  /// @param[in] val0, val1 書き込む値
  void
  test_gval(SimNode* node,
	    PackedVal val0,
	    PackedVal val1);

  /// @brief fval の書き込み読み出しテスト
  /// @param[in] node 対象のノード
  /// @param[in] val0, val1 書き込む値
  void
  test_fval(SimNode* node,
	    PackedVal val0,
	    PackedVal val1);

};

BEGIN_NONAMESPACE

// gval/fval を初期化する．
void
init_val(SimNode* node,
	 PackedVal val0,
	 PackedVal val1)
{
  node->set_gval(val0, val1);
  node->set_fval(val0, val1);
}

END_NONAMESPACE

// @brief 入力ノードのテストを行う．
void
SimNodeTest::test_input()
{
  SimNode* node = SimNode::new_input(0);

  // gval の書き込み読み出しテスト
  init_val(node, kPvAll0, kPvAll0);

  test_gval(node, kPvAll1, kPvAll0);
  test_gval(node, 0x5555555555555555UL, 0x5555555555555555UL);
  test_gval(node, 0xaaaaaaaaaaaaaaaaUL, 0xaaaaaaaaaaaaaaaaUL);
  test_gval(node, kPvAll0, kPvAll1);

  // fval の書き込み読み出しテスト
  init_val(node, kPvAll1, kPvAll1);

  test_fval(node, kPvAll1, kPvAll0);
  test_fval(node, 0x5555555555555555UL, 0x5555555555555555UL);
  test_fval(node, 0xaaaaaaaaaaaaaaaaUL, 0xaaaaaaaaaaaaaaaaUL);
  test_fval(node, kPvAll0, kPvAll1);

  delete node;
}

// @brief 論理ノードのテストを行う．
// @param[in] ni 入力数
// @param[in] gate_type ゲートの種類
// @param[in] vals 真理値表ベクタ
void
SimNodeTest::test_gate(ymuint ni,
		       GateType gate_type,
		       int vals[])
{
  vector<SimNode*> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    inputs[i] = SimNode::new_input(i);
  }
  SimNode* node = SimNode::new_node(ni, gate_type, inputs);

  // gval の書き込み読み出しテスト
  init_val(node, kPvAll0, kPvAll0);

  test_gval(node, kPvAll1, kPvAll0);
  test_gval(node, 0x5555555555555555UL, 0x5555555555555555UL);
  test_gval(node, 0xaaaaaaaaaaaaaaaaUL, 0xaaaaaaaaaaaaaaaaUL);
  test_gval(node, kPvAll0, kPvAll1);

  // fval の書き込み読み出しテスト
  init_val(node, kPvAll1, kPvAll1);

  test_fval(node, kPvAll1, kPvAll0);
  test_fval(node, 0x5555555555555555UL, 0x5555555555555555UL);
  test_fval(node, 0xaaaaaaaaaaaaaaaaUL, 0xaaaaaaaaaaaaaaaaUL);
  test_fval(node, kPvAll0, kPvAll1);

  // _calc_gval() のテスト
  // ここで書き込む値に対して意味はない．
  init_val(node, kPvAll0, kPvAll0);
  for (ymuint i = 0; i < ni; ++ i) {
    init_val(inputs[i], kPvAll0, kPvAll0);
  }

  ymuint np = 1;
  for (ymuint i = 0; i < ni; ++ i) {
    np *= 3;
  }
  for (ymuint p = 0; p < np; ++ p) {
    ymuint x = p;
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint y = x % 3;
      x /= 3;
      switch ( y ) {
      case 0: inputs[i]->set_gval(kPvAll1, kPvAll0); break;
      case 1: inputs[i]->set_gval(kPvAll0, kPvAll1); break;
      case 2: inputs[i]->set_gval(kPvAll1, kPvAll1); break;
      }
    }
    node->_calc_gval();
    PackedVal val0 = node->gval_0();
    PackedVal val1 = node->gval_1();
    switch ( vals[p] ) {
    case 0:
      EXPECT_EQ( val0, kPvAll1 );
      EXPECT_EQ( val1, kPvAll0 );
      break;

    case 1:
      EXPECT_EQ( val0, kPvAll0 );
      EXPECT_EQ( val1, kPvAll1 );
      break;

    case 2:
      EXPECT_EQ( val0, kPvAll1 );
      EXPECT_EQ( val1, kPvAll1 );
      break;
    }
  }

  // _calc_fval() のテスト
  // ここで書き込む値に対して意味はない．
  init_val(node, kPvAll1, kPvAll1);
  for (ymuint i = 0; i < ni; ++ i) {
    init_val(inputs[i], kPvAll1, kPvAll1);
  }

  for (ymuint p = 0; p < np; ++ p) {
    ymuint x = p;
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint y = x % 3;
      x /= 3;
      switch ( y ) {
      case 0: inputs[i]->set_fval(kPvAll1, kPvAll0); break;
      case 1: inputs[i]->set_fval(kPvAll0, kPvAll1); break;
      case 2: inputs[i]->set_fval(kPvAll1, kPvAll1); break;
      }
    }
    node->_calc_fval(kPvAll1);
    PackedVal val0 = node->fval_0();
    PackedVal val1 = node->fval_1();
    switch ( vals[p] ) {
    case 0:
      EXPECT_EQ( val0, kPvAll1 );
      EXPECT_EQ( val1, kPvAll0 );
      break;

    case 1:
      EXPECT_EQ( val0, kPvAll0 );
      EXPECT_EQ( val1, kPvAll1 );
      break;

    case 2:
      EXPECT_EQ( val0, kPvAll1 );
      EXPECT_EQ( val1, kPvAll1 );
      break;
    }
  }

#if 0
  // calc_gobs() のテスト
  for (ymuint ipos = 0; ipos < ni; ++ ipos) {
    // ここで書き込む値に対して意味はない．
    init_val(node, kPvAll0, kPvAll1);
    for (ymuint i = 0; i < ni; ++ i) {
      init_val(inputs[i], kPvAll0, kPvAll1);
    }

    vector<int> ivals(ni, 0);
    for ( ; ; ) {
      ymuint p = 0;
      ymuint q = 0;
      ymuint w = 1;
      for (ymuint i = 0; i < ni; ++ i) {
	switch ( ivals[i] ) {
	case 0: inputs[i]->set_gval(kPvAll1, kPvAll0); break;
	case 1: inputs[i]->set_gval(kPvAll0, kPvAll1); break;
	case 2: inputs[i]->set_gval(kPvAll1, kPvAll1); break;
	}

	if ( i == ipos ) {
	  q += w;
	}
	else {
	  p += ivals[i] * w;
	  q += ivals[i] * w;
	}
	w *= 3;
      }

      PackedVal val = node->calc_gobs(ipos);
      if ( vals[q] != vals[p] ) {
	EXPECT_EQ( val, kPvAll1 );
      }
      else {
	EXPECT_EQ( val, kPvAll0 );
      }

      for (ymuint i = 0; i < ni; ++ i) {
	++ ivals[i];
	if ( ivals[i] == 3 ) {
	  ivals[i] = 0;
	}
	else {
	  break;
	}
      }
    }
  }
#endif

  for (ymuint i = 0; i < ni; ++ i) {
    delete inputs[i];
  }
  delete node;
}

// @brief gval の書き込み読み出しテスト
// @param[in] node 対象のノード
// @param[in] val0, val1 書き込む値
void
SimNodeTest::test_gval(SimNode* node,
		       PackedVal val0,
		       PackedVal val1)
{
  node->set_gval(val0, val1);
  EXPECT_EQ( node->gval_0(), val0 );
  EXPECT_EQ( node->gval_1(), val1 );
}

// @brief fval の書き込み読み出しテスト
// @param[in] node 対象のノード
// @param[in] val0, val1 書き込む値
void
SimNodeTest::test_fval(SimNode* node,
		       PackedVal val0,
		       PackedVal val1)
{
  node->set_fval(val0, val1);
  EXPECT_EQ( node->fval_0(), val0 );
  EXPECT_EQ( node->fval_1(), val1 );
}

TEST_F(SimNodeTest, INPUT)
{
  test_input();
}

TEST_F(SimNodeTest, BUFF)
{
  int vals[] = {
    0,
    1,
    2,
  };

  test_gate(1, kGateBUFF, vals);
}

TEST_F(SimNodeTest, NOT)
{
  int vals[] = {
    1,
    0,
    2,
  };

  test_gate(1, kGateNOT, vals);
}

TEST_F(SimNodeTest, AND2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( x == 0 || y == 0 ) {
	val = 0;
      }
      else if ( x == 1 && y == 1 ) {
	val = 1;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateAND, vals);
}

TEST_F(SimNodeTest, AND3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 2;
	if ( x == 0 || y == 0 || z == 0 ) {
	  val = 0;
	}
	else if ( x == 1 && y == 1 && z == 1 ) {
	  val = 1;
	}
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateAND, vals);
}

TEST_F(SimNodeTest, AND4)
{
  int vals[3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  int val = 2;
	  if ( x == 0 || y == 0 || z == 0 || w == 0 ) {
	    val = 0;
	  }
	  else if ( x == 1 && y == 1 && z == 1 && w == 1 ) {
	    val = 1;
	  }
	  vals[((x * 3 + y) * 3 + z) * 3 + w] = val;
	}
      }
    }
  }

  test_gate(4, kGateAND, vals);
}

TEST_F(SimNodeTest, AND5)
{
  int vals[3 * 3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  for (ymuint u = 0; u < 3; ++ u) {
	    int val = 2;
	    if ( x == 0 || y == 0 || z == 0 || w == 0 || u == 0 ) {
	      val = 0;
	    }
	    else if ( x == 1 && y == 1 && z == 1 && w == 1 && u == 1 ) {
	      val = 1;
	    }
	    vals[(((x * 3 + y) * 3 + z) * w) * 3 + u] = val;
	  }
	}
      }
    }
  }

  test_gate(5, kGateAND, vals);
}

TEST_F(SimNodeTest, NAND2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( x == 0 || y == 0 ) {
	val = 1;
      }
      else if ( x == 1 && y == 1 ) {
	val = 0;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateNAND, vals);
}

TEST_F(SimNodeTest, NAND3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 2;
	if ( x == 0 || y == 0 || z == 0 ) {
	  val = 1;
	}
	else if ( x == 1 && y == 1 && z == 1 ) {
	  val = 0;
	}
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateNAND, vals);
}

TEST_F(SimNodeTest, NAND4)
{
  int vals[3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  int val = 2;
	  if ( x == 0 || y == 0 || z == 0 || w == 0 ) {
	    val = 1;
	  }
	  else if ( x == 1 && y == 1 && z == 1 && w == 1 ) {
	    val = 0;
	  }
	  vals[((x * 3 + y) * 3 + z) * 3 + w] = val;
	}
      }
    }
  }

  test_gate(4, kGateNAND, vals);
}

TEST_F(SimNodeTest, NAND5)
{
  int vals[3 * 3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  for (ymuint u = 0; u < 3; ++ u) {
	    int val = 2;
	    if ( x == 0 || y == 0 || z == 0 || w == 0 || u == 0 ) {
	      val = 1;
	    }
	    else if ( x == 1 && y == 1 && z == 1 && w == 1 && u == 1 ) {
	      val = 0;
	    }
	    vals[(((x * 3 + y) * 3 + z) * w) * 3 + u] = val;
	  }
	}
      }
    }
  }

  test_gate(5, kGateNAND, vals);
}

TEST_F(SimNodeTest, OR2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( x == 0 && y == 0 ) {
	val = 0;
      }
      else if ( x == 1 || y == 1 ) {
	val = 1;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateOR, vals);
}

TEST_F(SimNodeTest, OR3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 2;
	if ( x == 0 && y == 0 && z == 0 ) {
	  val = 0;
	}
	else if ( x == 1 || y == 1 || z == 1 ) {
	  val = 1;
	}
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateOR, vals);
}

TEST_F(SimNodeTest, OR4)
{
  int vals[3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  int val = 2;
	  if ( x == 0 && y == 0 && z == 0 && w == 0 ) {
	    val = 0;
	  }
	  else if ( x == 1 || y == 1 || z == 1 || w == 1 ) {
	    val = 1;
	  }
	  vals[((x * 3 + y) * 3 + z) * 3 + w] = val;
	}
      }
    }
  }

  test_gate(4, kGateOR, vals);
}

TEST_F(SimNodeTest, OR5)
{
  int vals[3 * 3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  for (ymuint u = 0; u < 3; ++ u) {
	    int val = 2;
	    if ( x == 0 && y == 0 && z == 0 && w == 0 && u == 0 ) {
	      val = 0;
	    }
	    else if ( x == 1 || y == 1 || z == 1 || w == 1 || u == 1 ) {
	      val = 1;
	    }
	    vals[(((x * 3 + y) * 3 + z) * w) * 3 + u] = val;
	  }
	}
      }
    }
  }

  test_gate(5, kGateOR, vals);
}

TEST_F(SimNodeTest, NOR2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( x == 0 && y == 0 ) {
	val = 1;
      }
      else if ( x == 1 || y == 1 ) {
	val = 0;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateNOR, vals);
}

TEST_F(SimNodeTest, NOR3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 2;
	if ( x == 0 && y == 0 && z == 0 ) {
	  val = 1;
	}
	else if ( x == 1 || y == 1 || z == 1 ) {
	  val = 0;
	}
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateNOR, vals);
}

TEST_F(SimNodeTest, NOR4)
{
  int vals[3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  int val = 2;
	  if ( x == 0 && y == 0 && z == 0 && w == 0 ) {
	    val = 1;
	  }
	  else if ( x == 1 || y == 1 || z == 1 || w == 1 ) {
	    val = 0;
	  }
	  vals[((x * 3 + y) * 3 + z) * 3 + w] = val;
	}
      }
    }
  }

  test_gate(4, kGateNOR, vals);
}

TEST_F(SimNodeTest, NOR5)
{
  int vals[3 * 3 * 3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	for (ymuint w = 0; w  < 3; ++ w) {
	  for (ymuint u = 0; u < 3; ++ u) {
	    int val = 2;
	    if ( x == 0 && y == 0 && z == 0 && w == 0 && u == 0 ) {
	      val = 1;
	    }
	    else if ( x == 1 || y == 1 || z == 1 || w == 1 || u == 1 ) {
	      val = 0;
	    }
	    vals[(((x * 3 + y) * 3 + z) * w) * 3 + u] = val;
	  }
	}
      }
    }
  }

  test_gate(5, kGateNOR, vals);
}

TEST_F(SimNodeTest, XOR2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( (x == 0 && y == 1) ||
	   (x == 1 && y == 0) ) {
	val = 1;
      }
      else if ( (x == 0 && y == 0) ||
		(x == 1 && y == 1) ) {
	val = 0;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateXOR, vals);
}

TEST_F(SimNodeTest, XOR3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 0;
	if ( x == 1 ) {
	  val ^= 1;
	}
	else if ( x == 2 ) {
	  val = 2;
	  goto skip;
	}
	if ( y == 1 ) {
	  val ^= 1;
	}
	else if ( y == 2 ) {
	  val = 2;
	  goto skip;
	}
	if ( z == 1 ) {
	  val ^= 1;
	}
	else if ( z == 2 ) {
	  val = 2;
	}
      skip:
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateXOR, vals);
}

TEST_F(SimNodeTest, XNOR2)
{
  int vals[3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      int val = 2;
      if ( (x == 0 && y == 1) ||
	   (x == 1 && y == 0) ) {
	val = 0;
      }
      else if ( (x == 0 && y == 0) ||
		(x == 1 && y == 1) ) {
	val = 1;
      }
      vals[x * 3 + y] = val;
    }
  }

  test_gate(2, kGateXNOR, vals);
}

TEST_F(SimNodeTest, XNOR3)
{
  int vals[3 * 3 * 3];
  for (ymuint x = 0; x < 3; ++ x) {
    for (ymuint y = 0; y < 3; ++ y) {
      for (ymuint z = 0; z < 3; ++ z) {
	int val = 1;
	if ( x == 1 ) {
	  val ^= 1;
	}
	else if ( x == 2 ) {
	  val = 2;
	  goto skip;
	}
	if ( y == 1 ) {
	  val ^= 1;
	}
	else if ( y == 2 ) {
	  val = 2;
	  goto skip;
	}
	if ( z == 1 ) {
	  val ^= 1;
	}
	else if ( z == 2 ) {
	  val = 2;
	}
      skip:
	vals[(x * 3 + y) * 3 + z] = val;
      }
    }
  }

  test_gate(3, kGateXNOR, vals);
}

END_NAMESPACE_YM_SATPG_FSIM
