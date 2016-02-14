

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${TCL_INCLUDE_PATH}
  ${YmTclpp_INCLUDE_DIRS}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (atpg_tclsh_SOURCES
  tclAppInit.cc
  atpg_init.cc
  AtpgCmd.cc
  AtpgMgr.cc
  AtpgMsgHandler.cc
  NtwkIoCmd.cc
  PrintFaultCmd.cc
  PrintPatCmd.cc
  PrintPatStatsCmd.cc
  PrintStatsCmd.cc
  TimeCmd.cc
  RtpgCmd.cc
  DtpgCmd.cc
  MinPatCmd.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================

add_executable(atpg_tclsh
  ${atpg_tclsh_SOURCES}
  $<TARGET_OBJECTS:satpg_common>
  )

target_compile_options (atpg_tclsh
  PRIVATE "-O3"
  )

target_compile_definitions (atpg_tclsh
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(atpg_tclsh
  ym_networks
  ym_tclpp
  ym_algo
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_executable(atpg_tclsh_p
  ${atpg_tclsh_SOURCES}
  $<TARGET_OBJECTS:satpg_common_p>
  )

target_compile_options (atpg_tclsh_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (atpg_tclsh_p
  PRIVATE "-DNDEBUG"
  )

target_link_libraries(atpg_tclsh_p
  ym_networks_p
  ym_tclpp_p
  ym_algo_p
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( atpg_tclsh_p
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_executable(atpg_tclsh_d
  ${atpg_tclsh_SOURCES}
  $<TARGET_OBJECTS:satpg_common_d>
  )

target_compile_options (atpg_tclsh_d
  PRIVATE "-g"
  )

target_link_libraries(atpg_tclsh_d
  ym_networks_d
  ym_tclpp_d
  ym_algo_d
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install (
  TARGETS atpg_tclsh atpg_tclsh_p atpg_tclsh_d
  DESTINATION bin
  )
