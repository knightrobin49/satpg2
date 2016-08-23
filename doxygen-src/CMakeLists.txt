
# ===================================================================
# doxygen 起動用のコマンド
# ===================================================================

# Doxyfile の生成
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
  "Doxyfile"
  )

# doxygen 用ターゲット
add_custom_target(dox ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html"
  )

# doxygen の対象となるヘッダファイル(のパタン)を YM_DOXY_SRCS に入れる．
set ( YM_DOXY_SRCS )
foreach (dir
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    )
  list ( APPEND YM_DOXY_SRCS ${dir} )
endforeach()

# doxygen 起動ルール
add_custom_command(
  COMMAND "${DOXYGEN_EXECUTABLE}" ">" "doxygen.log"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" ${YM_DOXY_SRCS}
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/docs/html/index.html"
  COMMENT "generating doxygen documents"
  )

# 生成物のインストールルール
install (DIRECTORY
  "${CMAKE_CURRENT_BINARY_DIR}/docs"
  DESTINATION share/matoth-${MATOTH_VERSION}/
  )
