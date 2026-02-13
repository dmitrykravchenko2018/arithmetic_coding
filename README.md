# Arithmetic coding 
C++ implementation of arithmetic coding algorithm based on algorithm proposed in article: Arithmetic Coding for Data Compression by Ian H. Witten, Radford M. Neal, John G. Cleary (1987).Communications of the ACM. Volume 30. Number 6. 520-540pp.

## Usage

```bash
make
./arithm-coding e <input_file> <output_file>   # encode (compress)
./arithm-coding d <input_file> <output_file>   # decode (decompress)
```

Example:
```bash
./arithm-coding e myfile.txt myfile.bin
./arithm-coding d myfile.bin myfile_decoded.txt
```
