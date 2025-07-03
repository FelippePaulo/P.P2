import numpy as np
from scipy.fftpack import dct, idct

def tdc(sinal):
    return dct(sinal, norm='ortho')

def tdc_inversa(sinal_tdc):
    return idct(sinal_tdc, norm='ortho')

# Exemplo de uso:
sinal = np.array([8.0000, 16.0000, 24.0000, 32.0000, 40.0000, 48.0000, 56.0000, 64.0000], dtype=float)
sinal_tdc = tdc(sinal)
sinal_teste = np.array([101.8234, -51.5386, -0.0000, -5.3876, 0.0000, -1.6072, -0.0000, -0.4056])
print("Sinal TDC:", sinal_tdc)

sinal_original = tdc_inversa(sinal_teste)
print("Sinal original:", sinal_original)