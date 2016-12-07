
# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (base_SOURCES
  )

set (tpg_network_SOURCES
  tpg_network/FaultMgr.cc
  tpg_network/TpgFault.cc
  tpg_network/TpgMap.cc
  tpg_network/TpgNetwork.cc
  tpg_network/TpgDff.cc
  tpg_network/TpgNode.cc
  tpg_network/TpgNodeInfo.cc
  tpg_network/TpgNodeMap.cc
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
  )

set (sa_test_SOURCES
  sa_test/dtpg/bt/BackTracer.cc
  sa_test/dtpg/bt/BtImpl.cc
  sa_test/dtpg/bt/BtJust1.cc
  sa_test/dtpg/bt/BtJust2.cc
  sa_test/dtpg/bt/BtJustBase.cc
  sa_test/dtpg/bt/BtSimple.cc

  sa_test/dtpg/dop/DopBase.cc
  sa_test/dtpg/dop/DopDrop.cc
  sa_test/dtpg/dop/DopDummy.cc
  sa_test/dtpg/dop/DopList.cc
  sa_test/dtpg/dop/DopTvList.cc
  sa_test/dtpg/dop/DopVerify.cc
  sa_test/dtpg/dop/FopDrop.cc

  sa_test/dtpg/main/DtpgSat.cc
  sa_test/dtpg/main/DtpgSatS.cc
  sa_test/dtpg/main/DtpgSatH.cc

  sa_test/dtpg/uop/UopBase.cc
  sa_test/dtpg/uop/UopDummy.cc
  sa_test/dtpg/uop/UopList.cc
  sa_test/dtpg/uop/UopSkip.cc

  sa_test/fsim/fop/FopKDet.cc

  sa_test/fsim/fsim2/EventQ.cc
  sa_test/fsim/fsim2/Fsim2.cc
  sa_test/fsim/fsim2/SimNode.cc
  sa_test/fsim/fsim2/SnAnd.cc
  sa_test/fsim/fsim2/SnGate.cc
  sa_test/fsim/fsim2/SnInput.cc
  sa_test/fsim/fsim2/SnOr.cc
  sa_test/fsim/fsim2/SnXor.cc

  sa_test/fsim/fsim3/EventQ.cc
  sa_test/fsim/fsim3/Fsim3.cc
  sa_test/fsim/fsim3/SimNode.cc
  sa_test/fsim/fsim3/SnAnd.cc
  sa_test/fsim/fsim3/SnGate.cc
  sa_test/fsim/fsim3/SnInput.cc
  sa_test/fsim/fsim3/SnOr.cc
  sa_test/fsim/fsim3/SnXor.cc

  sa_test/minpat/DetOp.cc
  sa_test/minpat/Verifier.cc
  sa_test/minpat/MinPatBase.cc
  sa_test/minpat/MinPatNaive.cc
  sa_test/minpat/MinPatSimple.cc
  sa_test/minpat/MinPatSimple2.cc
  sa_test/minpat/MinPatDsatur.cc
  sa_test/minpat/MinPatDsatur2.cc
  sa_test/minpat/FaultAnalyzer.cc
  sa_test/minpat/EqChecker.cc
  sa_test/minpat/DomChecker.cc
  sa_test/minpat/ConflictChecker.cc
  sa_test/minpat/FgMgrBase.cc
  sa_test/minpat/Compactor.cc
  sa_test/minpat/EqSet.cc
  sa_test/minpat/McOp.cc
  sa_test/minpat/McCompactor.cc

  sa_test/rtpg/FopRtpg.cc
  sa_test/rtpg/RtpgImpl.cc

  sa_test/struct_sat/StructSat.cc
  sa_test/struct_sat/ConeBase.cc
  sa_test/struct_sat/FoCone.cc
  sa_test/struct_sat/MffcCone.cc
  sa_test/struct_sat/NodeValList.cc
  sa_test/struct_sat/Extractor.cc
  sa_test/struct_sat/LitMap.cc
  sa_test/struct_sat/ModelValMap.cc
  sa_test/struct_sat/VidLitMap.cc
  sa_test/struct_sat/GenVidMap.cc

  sa_test/tvect/TestVector.cc
  sa_test/tvect/TvMgr.cc
  )

set (td_test_SOURCES
  td_test/dtpg/bt/BackTracer.cc
  td_test/dtpg/bt/BtImpl.cc
  td_test/dtpg/bt/BtJust1.cc
  td_test/dtpg/bt/BtJust2.cc
  td_test/dtpg/bt/BtJustBase.cc
  td_test/dtpg/bt/BtSimple.cc

  td_test/dtpg/main/DtpgSat.cc
  td_test/dtpg/main/DtpgSatS.cc

  td_test/fsim/fsim2/EventQ.cc
  td_test/fsim/fsim2/Fsim2.cc
  td_test/fsim/fsim2/SimNode.cc
  td_test/fsim/fsim2/SnAnd.cc
  td_test/fsim/fsim2/SnGate.cc
  td_test/fsim/fsim2/SnInput.cc
  td_test/fsim/fsim2/SnOr.cc
  td_test/fsim/fsim2/SnXor.cc

  td_test/rtpg/FopRtpg.cc
  td_test/rtpg/RtpgP1.cc
  td_test/rtpg/RtpgP2.cc

  td_test/struct_sat/ModelValMap.cc
  td_test/struct_sat/NodeValList.cc

  td_test/tvect/TestVector.cc
  td_test/tvect/TvMgr.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
ym_add_object_library ( satpg_common
  ${sa_test_SOURCES}
  ${td_test_SOURCES}
  ${tpg_network_SOURCES}
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
