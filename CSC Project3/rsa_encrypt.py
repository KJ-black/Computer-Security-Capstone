import sys
import pickle
import glob
import os

Usage = f'''
python3 {__file__} <n> <e> <file>
'''
# n = 22291846172619859445381409012451
# e = 65535

P = 97
Q = 83
n = P*Q
T = (P-1)*(Q-1)
e = 53
path = "./"

file = glob.glob(os.path.join(path, '*.jpg'))
print(file)

plain_bytes = b''
for filename in file:
	with open(filename, 'rb') as f:
		plain_bytes = f.read()
	cipher_int = [pow(i,e, n) for i in plain_bytes]
	with open(filename, 'wb') as f:
		pickle.dump(cipher_int, f)