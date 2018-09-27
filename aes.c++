#include <fstream>
#include <getopt.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

static struct option long_options[] = {
    {"keysize", required_argument, 0, 'k'},
    {"keyfile", required_argument, 0, 'f'},
    {"inputfile", required_argument, 0, 'i'},
    {"outputfile", required_argument, 0, 'o'},
    {"mode", required_argument, 0, 'm'},
    {0, 0, 0, 0}};

int keysize;
string keyfile;
string inputfile;
string outputfile;
int mode;

void parseCommandLine(int argc, char **argv) {
  int c;
  while (1) {
    c = getopt_long(argc, argv, "k:f:i:o:m:", long_options, NULL);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
    case 'k':
      keysize = stoi(optarg);
      break;

    case 'f':
      keyfile = optarg;
      break;

    case 'i':
      inputfile = optarg;
      break;

    case 'o':
      outputfile = optarg;
      break;

    case 'm':
      mode = stoi(optarg);
      break;

    case '?':
      /* getopt_long already printed an error message. */
      break;

    default:
      abort();
    }
  }
}

unsigned char getSBoxValue(unsigned char num) {

  unsigned char sbox[256] = {
      0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
      0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
      0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
      0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
      0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
      0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
      0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
      0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
      0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
      0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
      0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
      0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
      0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
      0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
      0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
      0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
      0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
      0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
      0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
      0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
      0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
      0xb0, 0x54, 0xbb, 0x16};
  return sbox[num];
}

int subBytes(unsigned char b) {
  //Simply index into the SBox array to find what the bits should be substituted for
  return getSBoxValue(b);
}

char* shiftRows(char *b) {
  //Shift rows 2, 3, 4 are stored in new char*
  char r[12] = {b[5],b[6],b[7],b[4],b[10],b[11],b[8],b[9],b[15],b[12],b[13],b[14]};
  for (int i = 0; i < 12; ++i)
    b[i + 4] = r[i];
  return b;
}

char* mixColumns(char *b) {
  char c1[4] = {b[0],b[4],b[8],b[12]};
  char c2[4] = {b[1],b[5],b[9],b[13]};
  char c3[4] = {b[2],b[6],b[10],b[14]};
  char c4[4] = {b[3],b[7],b[11],b[15]};
  char mixArray[16] = {2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2};
  /*       *\
    2 3 1 1
    1 2 3 1
    1 1 2 3
    3 1 1 2
  \*       */

  for (int i = 0; i < 4; ++i){
    b[i*4] = (c1[0]*mixArray[i*4]) ^ (c1[1]*mixArray[(i*4)+1]) ^ (c1[2]*mixArray[(i*4)+2]) ^ (c1[3]*mixArray[(i*4)+3]);
    b[(i*4)+1] = (c2[0]*mixArray[i*4]) ^ (c2[1]*mixArray[(i*4)+1]) ^ (c2[2]*mixArray[(i*4)+2]) ^ (c2[3]*mixArray[(i*4)+3]);
    b[(i*4)+2] = (c3[0]*mixArray[i*4]) ^ (c3[1]*mixArray[(i*4)+1]) ^ (c3[2]*mixArray[(i*4)+2]) ^ (c3[3]*mixArray[(i*4)+3]);
    b[(i*4)+3] = (c4[0]*mixArray[i*4]) ^ (c4[1]*mixArray[(i*4)+1]) ^ (c4[2]*mixArray[(i*4)+2]) ^ (c4[3]*mixArray[(i*4)+3]);
  }

  return b;

}

int addRoundkey() {}

int main(int argc, char **argv) {

  parseCommandLine(argc, argv);

  /**
   * TODO:
   *
   * Read input file
   * Encrypt/Decrypt bytes
   * Write to output file
   */

  streampos size;
  char *memblock;

  // Read bytes from input file
  ifstream file(inputfile, ios::in | ios::binary | ios::ate);
  if (file.is_open()) {

    size = file.tellg();
    memblock = new char[16];
    file.seekg(0, ios::beg);
    file.read(memblock, 16);

    for (int i = 0; i < size; i+=16) {
      if(file.gcount() < 16){
        for (int x = file.gcount(); x < 16; ++x)
          memblock[x] = 0;
      }
      cout << "Msg Before:\t" << memblock << endl;
      for (int n = 0; n < 16; ++n)
        memblock[n] = subBytes(memblock[n]);
      memblock = shiftRows(memblock);
      memblock = mixColumns(memblock);
      cout << "Ecnrypted:\t" << memblock << endl;
      file.read(memblock, 16);
    }

    file.close();

    delete[] memblock;
  } else
    cout << "Unable to open file";

  return 0;
}