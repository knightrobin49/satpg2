
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/satpg_common/sa/fsim2
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_executable ( SaFsim2SimNode2Test
  SimNode2Test.cc
  $<TARGET_OBJECTS:satpg_common_d>
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_logic_d>
  $<TARGET_OBJECTS:ym_cell_d>
  $<TARGET_OBJECTS:ym_bnet_d>
  $<TARGET_OBJECTS:ym_sat_d>
  $<TARGET_OBJECTS:ym_mincov_d>
  $<TARGET_OBJECTS:ym_udgraph_d>
  )

target_compile_options ( SaFsim2SimNode2Test
  PRIVATE "-g"
  )

target_link_libraries ( SaFsim2SimNode2Test
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( SaFsim2Test
  SaFsim2SimNode2Test
  )

add_executable ( SaFsim2SimNode3Test
  SimNode3Test.cc
  $<TARGET_OBJECTS:satpg_common_d>
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_logic_d>
  $<TARGET_OBJECTS:ym_cell_d>
  $<TARGET_OBJECTS:ym_bnet_d>
  $<TARGET_OBJECTS:ym_sat_d>
  $<TARGET_OBJECTS:ym_mincov_d>
  $<TARGET_OBJECTS:ym_udgraph_d>
  )

target_compile_options ( SaFsim2SimNode3Test
  PRIVATE "-g"
  )

target_link_libraries ( SaFsim2SimNode3Test
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( SaFsim2Test
  SaFsim2SimNode3Test
  )
