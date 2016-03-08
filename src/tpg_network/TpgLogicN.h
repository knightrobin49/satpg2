#ifndef TPGLOGICN_H
#define TPGLOGICN_H

/// @file TpgLogicN.h
/// @brief TpgLogicN のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "TpgLogic.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgLogicN TpgLogicN.h "TpgLogicN.h"
/// @brief N入力の TpgLogic
//////////////////////////////////////////////////////////////////////
class TpgLogicN :
  public TpgLogic
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] fanin_num ファンイン数
  /// @param[in] fanin_array ファンインの配列
  /// @param[in] fault_array 入力の故障の配列
  TpgLogicN(ymuint id,
	    const char* name,
	    ymuint fanin_num,
	    TpgNode** fanin_array,
	    TpgFault** fault_array);

  /// @brief デストラクタ
  ~TpgLogicN();


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

  // ファンイン数
  ymuint mFaninNum;

  // ファンインの配列
  // サイズは mFaninNum
  TpgNode** mFaninArray;

  // 入力の故障の配列
  // サイズは mFaninNum * 2
  TpgFault** mInputFaults;

};

END_NAMESPACE_YM_SATPG

#endif // TPGLOGICN_H
