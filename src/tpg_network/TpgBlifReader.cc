
/// @file TpgBlifReader.cc
/// @brief TpgBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgBlifReader.h"
#include "ym/BlifParser.h"
#include "TpgBlifHandler.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TpgBlifReader::TpgBlifReader() :
  mParser(new BlifParser),
  mHandler(new TpgBlifHandler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
TpgBlifReader::~TpgBlifReader()
{
  delete mParser;
  // mHandler は BlifParser が責任をもって破壊してくれる．
}

// @brief blif ファイルを読み込む．
bool
TpgBlifReader::operator()(const string& filename,
			  TpgNetwork& network,
			  const CellLibrary* cell_library)
{
  mHandler->set_network(network);
  bool stat = mParser->read(filename, cell_library);
  return stat;
}

END_NAMESPACE_YM_SATPG
