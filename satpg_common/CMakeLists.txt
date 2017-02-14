
# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( sa/fsim )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (tpg_network_SOURCES
  tpg_network/TpgFault.cc
  tpg_network/TpgFaultMgr.cc
  tpg_network/TpgNetwork.cc
  tpg_network/TpgDff.cc
  tpg_network/TpgGateInfo.cc
  tpg_network/TpgNode.cc
  tpg_network/NodeMap.cc
  tpg_network/TpgPPI.cc
  tpg_network/TpgPPO.cc
  tpg_network/TpgInput.cc
  tpg_network/TpgOutput.cc
  tpg_network/TpgDffInput.cc
  tpg_network/TpgDffOutput.cc
  tpg_network/TpgDffControl.cc
  tpg_network/TpgDffClock.cc
  tpg_network/TpgDffClear.cc
  tpg_network/TpgDffPreset.cc
  tpg_network/TpgLogic.cc
  tpg_network/TpgLogicC0.cc
  tpg_network/TpgLogicC1.cc
  tpg_network/TpgLogicBUFF.cc
  tpg_network/TpgLogicNOT.cc
  tpg_network/TpgLogicAND.cc
  tpg_network/TpgLogicNAND.cc
  tpg_network/TpgLogicOR.cc
  tpg_network/TpgLogicNOR.cc
  tpg_network/TpgLogicXOR.cc
  tpg_network/TpgLogicXNOR.cc

  tpg_network/GateLitMap_vect.cc
  tpg_network/GateLitMap_vid.cc
  )

set (sa_SOURCES
  sa/bt/BackTracer.cc
  sa/bt/BtImpl.cc
  sa/bt/BtJust1.cc
  sa/bt/BtJust2.cc
  sa/bt/BtJustBase.cc
  sa/bt/BtSimple.cc

  sa/dop/DopBase.cc
  sa/dop/DopDrop.cc
  sa/dop/DopDummy.cc
  sa/dop/DopList.cc
  sa/dop/DopTvList.cc
  sa/dop/DopVerify.cc

  sa/dtpg/Dtpg.cc
  sa/dtpg/DtpgImpl.cc
  sa/dtpg/DtpgImplM.cc

  sa/fsim/Fsim.cc

  sa/uop/UopBase.cc
  sa/uop/UopDummy.cc
  sa/uop/UopList.cc
  sa/uop/UopSkip.cc

  sa/minpat/Verifier.cc
  sa/minpat/MinPatBase.cc
  sa/minpat/MinPatNaive.cc
  sa/minpat/MinPatSimple.cc
  sa/minpat/MinPatSimple2.cc
  sa/minpat/MinPatDsatur.cc
  sa/minpat/MinPatDsatur2.cc
  sa/minpat/FaultAnalyzer.cc
  sa/minpat/EqChecker.cc
  sa/minpat/DomChecker.cc
  sa/minpat/ConflictChecker.cc
  sa/minpat/FgMgrBase.cc
  sa/minpat/Compactor.cc
  sa/minpat/EqSet.cc
  sa/minpat/McCompactor.cc

  sa/rtpg/RtpgImpl.cc

  sa/struct_sat/StructSat.cc
  sa/struct_sat/ConeBase.cc
  sa/struct_sat/FoCone.cc
  sa/struct_sat/MffcCone.cc
  sa/struct_sat/NodeValList.cc
  sa/struct_sat/Extractor.cc
  sa/struct_sat/ModelValMap.cc

  sa/tvect/TestVector.cc
  sa/tvect/TvMgr.cc
  )

set (td_SOURCES
  td/bt/BackTracer.cc
  td/bt/BtImpl.cc
  td/bt/BtJust1.cc
  td/bt/BtJust2.cc
  td/bt/BtJustBase.cc
  td/bt/BtSimple.cc

  td/dtpg/DtpgSat.cc
  td/dtpg/DtpgSatS.cc

  td/fsim2/EventQ.cc
  td/fsim2/Fsim2.cc
  td/fsim2/SimNode.cc
  td/fsim2/SnAnd.cc
  td/fsim2/SnGate.cc
  td/fsim2/SnInput.cc
  td/fsim2/SnOr.cc
  td/fsim2/SnXor.cc

  td/rtpg/FopRtpg.cc
  td/rtpg/RtpgP1.cc
  td/rtpg/RtpgP2.cc

  td/struct_sat/StructSat.cc
  td/struct_sat/ConeBase.cc
  td/struct_sat/FoCone.cc
  td/struct_sat/MffcCone.cc
  td/struct_sat/NodeValList.cc
  td/struct_sat/ModelValMap.cc
  td/struct_sat/Extractor.cc

  td/tvect/TestVector.cc
  td/tvect/TvMgr.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
ym_add_object_library ( satpg_common
  ${sa_SOURCES}
  ${td_SOURCES}
  ${tpg_network_SOURCES}
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
