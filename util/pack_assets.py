import os

SCAN_DIR = 'data/'
OUT_FILE = 'tmp/data/data.dat'

out_data = open(OUT_FILE, 'wb')

for parent, subdirs, files in os.walk(SCAN_DIR):
    for fname in files:
        fpath = os.path.join(parent, fname)
        fpathrel = fpath[len(SCAN_DIR) :]

        with open(fpath, 'rb') as file_in:
            out_data.write(f'{fpathrel}\n'.encode('utf-8'))
            out_data.write(f'{len(file_in.read()) + 1}\n'.encode('utf-8'))
            file_in.seek(0, 0)
            out_data.write(file_in.read())

        out_data.write('\0\n'.encode('utf-8'))

out_data.close()
