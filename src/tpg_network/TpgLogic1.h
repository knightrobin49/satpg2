#ifndef TPGLOGIC1_H
#define TPGLOGIC1_H

/// @file TpgLogic1.h
/// @brief TpgLogic1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogic1 TpgLogic1.h "TpgLogic1.h"
/// @brief 0入力の TpgLogic
//////////////////////////////////////////////////////////////////////
class TpgLogic1 :
  public TpgLogic
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] inode ファンインのノード
  TpgLogic1(ymuint id,
	    TpgNode* inode);

  /// @brief デストラクタ
  ~TpgLogic1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン数を得る．
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  virtual
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;

  /// @brief このノードに関係する故障数を返す．
  virtual
  ymuint
  fault_num() const;

  /// @brief このノードに関係する故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  virtual
  const TpgFault*
  fault(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンイン
  TpgNode* mFanin;

  // 入力の故障
  TpgFault* mInputFaults[2];

};

END_NAMESPACE_YM_SATPG

#endif // TPGLOGIC1_H
