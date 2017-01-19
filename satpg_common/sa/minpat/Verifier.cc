
/// @file Verifier.cc
/// @brief Verifier の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Verifier.h"

#include "TpgFault.h"

#include "sa/FsimOp.h"
#include "sa/TestVector.h"

#include "ym/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG_SA

#if 0
//////////////////////////////////////////////////////////////////////
/// @class FopVer Verifier.h "Verifier.h"
/// @brief Verifier で用いられる FsimOp
//////////////////////////////////////////////////////////////////////

class FopVer :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  FopVer(Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~FopVer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  virtual
  void
  operator()(const TpgFault* f,
	     PackedVal dpat);

  /// @brief det_flag を下ろす．
  void
  clear_det_flag();

  /// @brief 故障が見つかったら true を返す．
  bool
  is_detected(const TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 検出済みの故障番号を入れるハッシュ
  HashSet<ymuint> mDetSet;

};


//////////////////////////////////////////////////////////////////////
// クラス FopVer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
FopVer::FopVer(Fsim& fsim) :
  mFsim(fsim)
{
}

// @brief デeストラクタ
FopVer::~FopVer()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
void
FopVer::operator()(const TpgFault* fault,
		   PackedVal dpat)
{
  mFsim.set_skip(fault);
  mDetSet.add(fault->id());
}

// @brief det_flag を下ろす．
void
FopVer::clear_det_flag()
{
  mDetSet.clear();
}

// @brief 故障が見つかったら true を返す．
bool
FopVer::is_detected(const TpgFault* fault)
{
  return mDetSet.check(fault->id());
}
#endif


//////////////////////////////////////////////////////////////////////
// クラス Verifier
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Verifier::Verifier()
{
}

// @brief デストラクタ
Verifier::~Verifier()
{
}

// @brief 与えられたパタンリストで全検出済み故障を検出できるか調べる．
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 故障のリスト
// @param[in] pat_list パタンのリスト
bool
Verifier::check(Fsim& fsim,
		const vector<const TpgFault*>& fault_list,
		const vector<const TestVector*>& pat_list)
{
  fsim.clear_skip(fault_list);

  // 検出された故障番号を入れるハッシュ表
  HashSet<ymuint> fhash;

  const TestVector* cur_array[kPvBitLen];
  vector<pair<const TpgFault*, PackedVal> > det_fault_list;
  ymuint cpos = 0;
  for (vector<const TestVector*>::const_iterator p = pat_list.begin();
       p != pat_list.end(); ++ p) {
    const TestVector* tv = *p;
    cur_array[cpos] = tv;
    ++ cpos;
    if ( cpos == kPvBitLen ) {
      fsim.ppsfp(kPvBitLen, cur_array, det_fault_list);
      cpos = 0;
      for (ymuint i = 0; i < det_fault_list.size(); ++ i) {
	const TpgFault* f = det_fault_list[i].first;
	// どのパタンで検出できたかは調べる必要はない．
	fhash.add(f->id());
      }
    }
  }
  if ( cpos > 0 ) {
    fsim.ppsfp(cpos, cur_array, det_fault_list);
    for (ymuint i = 0; i < det_fault_list.size(); ++ i) {
      const TpgFault* f = det_fault_list[i].first;
      // どのパタンで検出できたかは調べる必要はない．
      fhash.add(f->id());
    }
  }

  bool no_error = true;
  for (vector<const TpgFault*>::const_iterator p = fault_list.begin();
       p != fault_list.end(); ++ p) {
    const TpgFault* fault = *p;
    if ( !fhash.find(fault->id()) ) {
      cout << "Error: " << fault << " has no patterns" << endl;
      no_error = false;
    }
  }

  return no_error;
}

END_NAMESPACE_YM_SATPG_SA
