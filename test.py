from ctypes import *

class Board(Structure):
    _fields_ = ('n', c_int), ('w', c_int), ('h', c_int)

lib = cdll['libboardwrapper.so']

lib.create.argtypes = (c_int, c_int, c_int)
lib.create.restype = POINTER(Board)

lib.destroy.argtypes = (POINTER(Board), )
lib.destroy.restype = None

lib.get_input_vector_length.argtypes = (POINTER(Board), )
lib.get_input_vector_length.restype = c_int

lib.to_input_vector.argtypes = (POINTER(Board), )
lib.to_input_vector.restype = POINTER(c_int)

board = lib.create(3, 4, 4)
print(f'n={board.contents.n} w={board.contents.w} h={board.contents.h}')

# for i in range(4):
#     for j in range(4):
#         print(f'{i} {j} {lib.get(board, i, j)}')

v = lib.to_input_vector(board)
print(v)
a = cast(v, POINTER(c_int * lib.get_input_vector_length(board)))
print(a)

lib.destroy(board)