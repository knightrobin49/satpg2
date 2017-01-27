
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/satpg_common/sa/fsim3
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  Fsim2Test
# ===================================================================

set ( Fsim3Test_SOURCES
  Fsim3Test.cc
  )

add_executable ( Fsim3Test
  Fsim3Test.cc
  $<TARGET_OBJECTS:satpg_common_a>
  $<TARGET_OBJECTS:ym_common_a>
  $<TARGET_OBJECTS:ym_logic_a>
  $<TARGET_OBJECTS:ym_cell_a>
  $<TARGET_OBJECTS:ym_bnet_a>
  $<TARGET_OBJECTS:ym_sat_a>
  $<TARGET_OBJECTS:ym_mincov_a>
  $<TARGET_OBJECTS:ym_udgraph_a>
  )

target_compile_options ( Fsim3Test
  PRIVATE "-O3"
  )

target_compile_definitions ( Fsim3Test
  PRIVATE "-DNDEBUG"
  )

target_link_libraries ( Fsim3Test
  ${YM_LIB_DEPENDS}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( Fsim3Test
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()
