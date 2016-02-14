#ifndef TPGBLIFREADER_H
#define TPGBLIFREADER_H

/// @file TpgBlifReader.h
/// @brief TpgBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "satpg.h"
#include "ym/ym_blif.h"
#include "ym/ym_cell.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgBlifReader TpgBlifReader.h "TpgBlifReader.h"
/// @brief blif 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class TpgBlifReader
{
public:

  /// @brief コンストラクタ
  TpgBlifReader();

  /// @brief デストラクタ
  ~TpgBlifReader();


public:

  /// @brief blif ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] network 設定対象のネットワーク
  /// @param[in] cell_library セルライブラリ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  operator()(const string& filename,
	     TpgNetwork& network,
	     const CellLibrary* cell_library = nullptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Blif ファイルを読み込むためのパーサ
  BlifParser* mParser;

  // Blif ファイルを TpgNetwork に変換するためのハンドラ
  TpgBlifHandler* mHandler;

};

END_NAMESPACE_YM_SATPG

#endif // TPGBLIFREADER_H
