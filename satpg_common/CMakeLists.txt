
# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (base_SOURCES
  base/FaultMgr.cc
  base/TestVector.cc
  base/TestVector2.cc
  base/TvMgr.cc
  base/Tv2Mgr.cc
  )

set (tpg_network_SOURCES
  tpg_network/TpgFault.cc
  tpg_network/TpgMap.cc
  tpg_network/TpgNetwork.cc
  tpg_network/TpgNode.cc
  tpg_network/TpgNodeInfo.cc
  tpg_network/TpgNodeMap.cc
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

set (sat_engine_SOURCES
  sat_engine/StructSat.cc
  sat_engine/ConeBase.cc
  sat_engine/FoCone.cc
  sat_engine/MffcCone.cc
  sat_engine/NodeValList.cc
  sat_engine/Extractor.cc
  sat_engine/LitMap.cc
  sat_engine/ModelValMap.cc
  sat_engine/ModelValMap2.cc
  sat_engine/VidLitMap.cc
  sat_engine/GenVidMap.cc
  )

set (dtpg_SOURCES
  dtpg/bt/BackTracer.cc
  dtpg/bt/BtImpl.cc
  dtpg/bt/BtJust1.cc
  dtpg/bt/BtJust2.cc
  dtpg/bt/BtJustBase.cc
  dtpg/bt/BtSimple.cc

  dtpg/dop/DopBase.cc
  dtpg/dop/DopDrop.cc
  dtpg/dop/DopDummy.cc
  dtpg/dop/DopList.cc
  dtpg/dop/DopTvList.cc
  dtpg/dop/DopVerify.cc
  dtpg/dop/FopDrop.cc

  dtpg/main/DtpgSat.cc
  dtpg/main/DtpgSatS0.cc
  dtpg/main/DtpgSatS.cc
  dtpg/main/DtpgSatH.cc

  dtpg/uop/UopBase.cc
  dtpg/uop/UopDummy.cc
  dtpg/uop/UopList.cc
  dtpg/uop/UopSkip.cc
  )

set (dtpgt_SOURCES
  dtpgt/bt/BackTracer2.cc
  dtpgt/bt/Bt2Impl.cc
  dtpgt/bt/Bt2Just1.cc
  dtpgt/bt/Bt2Just2.cc
  dtpgt/bt/Bt2JustBase.cc
  dtpgt/bt/Bt2Simple.cc

  dtpgt/dop/Dop2Base.cc
 # dtpgt/dop/Dop2Drop.cc
  dtpgt/dop/Dop2Dummy.cc
  dtpgt/dop/Dop2List.cc
  dtpgt/dop/Dop2TvList.cc
  dtpgt/dop/Dop2Verify.cc
#  dtpgt/dop/Fop2Drop.cc

  dtpgt/main/DtpgTSat.cc
  dtpgt/main/DtpgTSatS.cc

#  dtpgt/uop/UopBase.cc
#  dtpgt/uop/UopDummy.cc
#  dtpgt/uop/UopList.cc
#  dtpgt/uop/UopSkip.cc
  )

set (fsim_SOURCES
  fsim/fop/FopKDet.cc

  fsim/fsim2/EventQ.cc
  fsim/fsim2/Fsim2.cc
  fsim/fsim2/SimNode.cc
  fsim/fsim2/SnAnd.cc
  fsim/fsim2/SnGate.cc
  fsim/fsim2/SnInput.cc
  fsim/fsim2/SnOr.cc
  fsim/fsim2/SnXor.cc

  fsim/fsim3/EventQ.cc
  fsim/fsim3/Fsim3.cc
  fsim/fsim3/SimNode.cc
  fsim/fsim3/SnAnd.cc
  fsim/fsim3/SnGate.cc
  fsim/fsim3/SnInput.cc
  fsim/fsim3/SnOr.cc
  fsim/fsim3/SnXor.cc

  fsim/fsimt2/EventQ.cc
  fsim/fsimt2/FsimT2.cc
  fsim/fsimt2/SimNode.cc
  fsim/fsimt2/SnAnd.cc
  fsim/fsimt2/SnGate.cc
  fsim/fsimt2/SnInput.cc
  fsim/fsimt2/SnOr.cc
  fsim/fsimt2/SnXor.cc

#  fsim/tfsim2/EventQ.cc
#  fsim/tfsim2/TFsim2.cc
#  fsim/tfsim2/SimNode.cc
#  fsim/tfsim2/SnAnd.cc
#  fsim/tfsim2/SnGate.cc
#  fsim/tfsim2/SnInput.cc
#  fsim/tfsim2/SnOr.cc
#  fsim/tfsim2/SnXor.cc
  )

set (minpat_SOURCES
  minpat/DetOp.cc
  minpat/Verifier.cc
  minpat/MinPatBase.cc
  minpat/MinPatNaive.cc
  minpat/MinPatSimple.cc
  minpat/MinPatSimple2.cc
  minpat/MinPatDsatur.cc
  minpat/MinPatDsatur2.cc
  minpat/FaultAnalyzer.cc
  minpat/EqChecker.cc
  minpat/DomChecker.cc
  minpat/ConflictChecker.cc
  minpat/FgMgrBase.cc
  minpat/Compactor.cc
  minpat/EqSet.cc
  minpat/McOp.cc
  minpat/McCompactor.cc
  )

set (rtpg_SOURCES
  rtpg/FopRtpg.cc
  rtpg/FopRtpgT.cc
  rtpg/RtpgImpl.cc
#  rtpg/RtpgTImpl.cc
  rtpg/RtpgTP1.cc
  rtpg/RtpgTP2.cc
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
ym_add_object_library ( satpg_common
  ${base_SOURCES}
  ${sat_engine_SOURCES}
  ${dtpg_SOURCES}
  ${dtpgt_SOURCES}
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
