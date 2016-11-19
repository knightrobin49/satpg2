
/// @file Dop2List.cc
/// @brief Dop2List の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dop2List.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Dop2List
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Dop2List::Dop2List()
{
}

// @brief デストラクタ
Dop2List::~Dop2List()
{
  for (vector<DetectOp2*>::iterator p = mDopList.begin();
       p != mDopList.end(); ++ p) {
    delete *p;
  }
}

// @brief dop を追加する．
void
Dop2List::add(DetectOp2* dop)
{
  mDopList.push_back(dop);
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] assign_list 値の割当リスト
void
Dop2List::operator()(const TpgFault* f,
		     const NodeVal2List& assign_list)
{
  for (vector<DetectOp2*>::iterator p = mDopList.begin();
       p != mDopList.end(); ++ p) {
    DetectOp2& dop = **p;
    dop(f, assign_list);
  }
}

END_NAMESPACE_YM_SATPG
