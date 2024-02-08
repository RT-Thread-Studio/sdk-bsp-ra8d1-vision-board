# this test is meaningful only, when the firmware supports complex arrays

try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float, np.complex)

for dtype in dtypes:
    a = np.array(range(4), dtype=dtype)
    b = a.reshape((2, 2))
    print('\narray:\n', a)
    print('\nreal part:\n', np.real(a))
    print('\nimaginary part:\n', np.imag(a))
    print('\narray:\n', b)
    print('\nreal part:\n', np.real(b))
    print('\nimaginary part:\n', np.imag(b), '\n')


b = np.array([0, 1j, 2+2j, 3-3j], dtype=np.complex)
print('\nreal part:\n', np.real(b))
print('\nimaginary part:\n', np.imag(b))

b = np.array([[0, 1j, 2+2j, 3-3j], [0, 1j, 2+2j, 3-3j]], dtype=np.complex)
print('\nreal part:\n', np.real(b))
print('\nimaginary part:\n', np.imag(b))

