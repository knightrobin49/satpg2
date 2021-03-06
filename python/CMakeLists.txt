

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  satpg.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( satpg
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:satpg_common>
  $<TARGET_OBJECTS:ym_common>
  $<TARGET_OBJECTS:ym_logic>
  $<TARGET_OBJECTS:ym_cell>
  $<TARGET_OBJECTS:ym_bnet>
  $<TARGET_OBJECTS:ym_udgraph>
  $<TARGET_OBJECTS:ym_mincov>
  $<TARGET_OBJECTS:ym_sat>
  )

target_compile_options ( satpg
  PRIVATE "-O3"
  )

target_compile_definitions ( satpg
  PRIVATE "-DNDEBUG"
  )

target_link_libraries ( satpg
  ${YM_LIB_DEPENDS}
  )

install (TARGETS
  satpg
  DESTINATION lib/python/site-packages
  )
