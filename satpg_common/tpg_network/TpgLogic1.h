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
  /// @param[in] name 名前
  /// @param[in] inode ファンインのノード
  TpgLogic1(ymuint id,
	    const char* name,
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

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  virtual
  TpgFault*
  input_fault(int val,
	      ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の故障を設定する．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  /// @param[in] fault 故障
  virtual
  void
  set_input_fault(int val,
		  ymuint pos,
		  TpgFault* fault);


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
