#
# @file satpg.pyx
# @brief satpg の python インターフェイス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016 Yusuke Matsunaga
# All rights reserved.


from cpython.object cimport Py_LT, Py_LE, Py_EQ, Py_GE, Py_GT, Py_NE
from libcpp.string cimport string
cimport CXX_cell
cimport CXX_satpg as cxx


# @brief CellLibrary の Python バージョン
cdef class CellLibrary :
    cdef CXX_cell.CellLibrary* _thisptr

    def __cinit__(CellLibrary self) :
        self._thisptr = CXX_cell.CellLibrary.new_obj()

    def read_mislib(CellLibrary self, str filename) :
        cdef CXX_cell.CellMislibReader reader
        cdef string c_filename = filename.encode('UTF-8')
        return reader.read(c_filename, self._thisptr)

    def read_dotlib(CellLibrary self, str filename) :
        cdef CXX_cell.CellDotlibReader reader
        cdef string c_filename = filename.encode('UTF-8')
        return reader.read(c_filename, self._thisptr)


# @brief TpgNetwork の Python ヴァージョン
cdef class TpgNetwork :
    cdef cxx.TpgNetwork _this

    def read_blif(TpgNetwork self, str filename, CellLibrary cell_library = None) :
        cdef string c_filename = filename.encode('UTF-8')
        if cell_library == None :
            return self._this.read_blif(c_filename)
        else :
            return self._this.read_blif(c_filename, cell_library._thisptr)

    def read_iscas89(TpgNetwork self, str filename) :
        cdef string c_filename = filename.encode('UTF-8')
        return self._this.read_iscas89(c_filename)
