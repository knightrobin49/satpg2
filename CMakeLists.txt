# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


# C++11 の指定
set (CMAKE_CXX_STANDARD 11)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (satpg)
set (SATPG_VERSION_MAJOR 1)
set (SATPG_VERSION_MINOR 0)
set (SATPG_VERSION ${SATPG_VERSION_MAJOR}.${SATPG_VERSION_MINOR})

# 名前空間
set (SATPG_NAMESPACE "nsSatpg")

# ライブラリのインストールディレクトリ
set ( YM_INSTALL_LIBDIR "lib/satpg" )


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )

include (FindDoxygen)

# Python のバージョン指定
set( Python_ADDITIONAL_VERSIONS 3.6 3.5 3.4 3.3 3.2 3.1 )
include( FindPythonInterp )
include( FindPythonLibs )

include( UseCython )

find_package(Gperftools)

include (YmUtils)

ym_init ( "" )


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)
enable_testing()


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

configure_file(
  "${PROJECT_SOURCE_DIR}/etc/version_config.h.in"
  "${PROJECT_BINARY_DIR}/version_config.h"
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-bnet/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-logic/include
  ${PROJECT_SOURCE_DIR}/ym-mincov/include
  ${PROJECT_SOURCE_DIR}/ym-sat/include
  ${PROJECT_SOURCE_DIR}/ym-tclpp/include
  ${PROJECT_SOURCE_DIR}/ym-udgraph/include
  ${PROJECT_BINARY_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( ym-common )
add_subdirectory ( ym-logic )
add_subdirectory ( ym-cell )
add_subdirectory ( ym-bnet )
add_subdirectory ( ym-udgraph )
add_subdirectory ( ym-mincov )
add_subdirectory ( ym-sat )
add_subdirectory ( ym-tclpp )

add_subdirectory ( gtest )
add_subdirectory ( atpg_tclsh )
add_subdirectory ( python )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (base_SOURCES
  src/base/FaultMgr.cc
  src/base/NodeSet.cc
  src/base/NodeValList.cc
  src/base/TestVector.cc
  src/base/TfiBits.cc
  src/base/TvMgr.cc
  )

set (sat_engine_SOURCES
  src/sat_engine/VidLitMap.cc
  src/sat_engine/GvalCnf.cc
  src/sat_engine/FoCone.cc
  src/sat_engine/FvalCnf.cc
  src/sat_engine/FvalCnfBase.cc
  src/sat_engine/MvalCnf.cc
  src/sat_engine/MffcCone.cc
  src/sat_engine/Extractor.cc
  src/sat_engine/LitMap.cc
  src/sat_engine/ModelValMap.cc
  src/sat_engine/GenVidMap.cc
  src/sat_engine/StructSat.cc
  )

set (dtpg_SOURCES
  src/dtpg/bt/BackTracer.cc
  src/dtpg/bt/BtImpl.cc
  src/dtpg/bt/BtJust1.cc
  src/dtpg/bt/BtJust2.cc
  src/dtpg/bt/BtJustBase.cc
  src/dtpg/bt/BtSimple.cc

  src/dtpg/dop/DopBase.cc
  src/dtpg/dop/DopDrop.cc
  src/dtpg/dop/DopDummy.cc
  src/dtpg/dop/DopList.cc
  src/dtpg/dop/DopTvList.cc
  src/dtpg/dop/DopVerify.cc
  src/dtpg/dop/FopDrop.cc

  src/dtpg/main/DtpgSat.cc
  src/dtpg/main/DtpgSatBaseM.cc
  src/dtpg/main/DtpgSatS.cc
  src/dtpg/main/DtpgSatM.cc
  src/dtpg/main/DtpgSatF.cc

  src/dtpg/uop/UopBase.cc
  src/dtpg/uop/UopDummy.cc
  src/dtpg/uop/UopList.cc
  src/dtpg/uop/UopSkip.cc
  )

set (fsim_SOURCES
  src/fsim/fop/FopKDet.cc

  src/fsim/fsim2/EventQ.cc
  src/fsim/fsim2/Fsim2.cc
  src/fsim/fsim2/SimNode.cc
  src/fsim/fsim2/SnAnd.cc
  src/fsim/fsim2/SnGate.cc
  src/fsim/fsim2/SnInput.cc
  src/fsim/fsim2/SnOr.cc
  src/fsim/fsim2/SnXor.cc

  src/fsim/fsim3/EventQ.cc
  src/fsim/fsim3/Fsim3.cc
  src/fsim/fsim3/SimNode.cc
  src/fsim/fsim3/SnAnd.cc
  src/fsim/fsim3/SnGate.cc
  src/fsim/fsim3/SnInput.cc
  src/fsim/fsim3/SnOr.cc
  src/fsim/fsim3/SnXor.cc
  )

set (minpat_SOURCES
  src/minpat/DetOp.cc
  src/minpat/Verifier.cc
  src/minpat/MinPatBase.cc
  src/minpat/MinPatNaive.cc
  src/minpat/MinPatSimple.cc
  src/minpat/MinPatSimple2.cc
  src/minpat/MinPatDsatur.cc
  src/minpat/MinPatDsatur2.cc
  src/minpat/FaultAnalyzer.cc
  src/minpat/EqChecker.cc
  src/minpat/DomChecker.cc
  src/minpat/ConflictChecker.cc
  src/minpat/FgMgrBase.cc
  src/minpat/Compactor.cc
  src/minpat/EqSet.cc
  src/minpat/McOp.cc
  src/minpat/McCompactor.cc
  )

set (rtpg_SOURCES
  src/rtpg/FopRtpg.cc
  src/rtpg/RtpgImpl.cc
  )

set (tpg_network_SOURCES
  src/tpg_network/TpgFault.cc
  src/tpg_network/TpgInput.cc
  src/tpg_network/TpgLogic.cc
  src/tpg_network/TpgLogic0.cc
  src/tpg_network/TpgLogic1.cc
  src/tpg_network/TpgLogic2.cc
  src/tpg_network/TpgLogic3.cc
  src/tpg_network/TpgLogic4.cc
  src/tpg_network/TpgLogicAND.cc
  src/tpg_network/TpgLogicBUFF.cc
  src/tpg_network/TpgLogicC0.cc
  src/tpg_network/TpgLogicC1.cc
  src/tpg_network/TpgLogicN.cc
  src/tpg_network/TpgLogicNAND.cc
  src/tpg_network/TpgLogicNOR.cc
  src/tpg_network/TpgLogicNOT.cc
  src/tpg_network/TpgLogicOR.cc
  src/tpg_network/TpgLogicXNOR.cc
  src/tpg_network/TpgLogicXOR.cc
  src/tpg_network/TpgMap.cc
  src/tpg_network/TpgNetwork.cc
  src/tpg_network/TpgNode.cc
  src/tpg_network/TpgNodeInfo.cc
  src/tpg_network/TpgNodeMap.cc
  src/tpg_network/TpgOutput.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
ym_add_object_library ( satpg_common
  ${base_SOURCES}
  ${sat_engine_SOURCES}
  ${dtpg_SOURCES}
  ${fsim_SOURCES}
  ${minpat_SOURCES}
  ${rtpg_SOURCES}
  ${tpg_network_SOURCES}
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
