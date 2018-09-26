#include <getopt.h>
#include <iostream>
#include <fstream>
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
  }}

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
  char * memblock;

  ifstream file (inputfile, ios::in|ios::binary|ios::ate);
  if (file.is_open()){

    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    for (int i = 0; i < size; ++i){
      printf("%x\n", memblock[i]);
    }

    delete[] memblock;
  }
  else cout << "Unable to open file";

  return 0;}