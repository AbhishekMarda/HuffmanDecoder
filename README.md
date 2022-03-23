<h1> Huffman decoder </h1>


Your compressed file, say `compressedfilename.dat` must be in the format of `output.dat` file from [my Huffman Encoder project](https://github.com/AbhishekMarda/HuffmanEncoder).

To run from terminal, do the following steps:

1. Go to project direcotry and run `make` to create the executable

2. Put compressedfilename.dat in the directory of the executable

3. Using terminal in the folder that contains the target file, run `./decoder compressedfilename.dat outputfile`

4. The code will create an `outputfile.txt` file which will be the uncompressed version for the file.

5. To cleanup executable run `make clean`
