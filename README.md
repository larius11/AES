# **AES**
------------------------------------------------
## Description
The assignment is to implement AES-128 and AES-256 in the same program. We will be following the AES standard. Use Electronic Code Book (ECB) mode. 
The program should run like this: 

```bash
./program --keysize $KEYSIZE --keyfile $KEYFILE --inputfile $INPUTFILE --outputfile $OUTFILENAME --mode $MODE
```

- keysize: Either 128 or 256 bits keyfile: Should take in a keyfile that fits one of the specified sizes. To generate a key, use: head -c 16 &lt; /dev/urandom > key (this generates 16 random bytes ~ 128 bits) 
- inputfile: This should be able to handle any file and any size (padding might be required). All we care about are the bytes of the file. To get the byte representation of a file use xxd outpfile: what to save the result mode: encrypt or decrypt

# **My Implementation**
------------------------------------------------

# Encryption
After parsing the command line to extract the arguments I read in the _inputkey_ file and perform the **Key Expansion**

## Key Expansion with 128-bit keysize
``` C++
int rounds = 44;        //44 rounds to expand a 128-bit key to 176 bytes
```
The first 4 rounds we simply copy the input key, word by word, into the expanded key. For the remaining 40 words we follow this repetition of functions:
``` C++
// (word) refers to the current section of the expandedkey which we are currently calculating
// (word - X) refers to X words before the current section of the expanded key
word = subWord(rotWord(word-1)) XOR Rcon XOR (word-4);
 ++cur_round;
for(int subrounds = 0; subrounds < 3; ++subrounds){
    word = (word - 1) XOR (word - 4);
    ++cur_round;}
```
Notice that this code goes through a total of 4 rounds. That is because the first line of code needs to happen every 4 rounds. The example code we were given uses a modulo but I thought it better to arrange it in this manner. Here are the functions used on the first line:
``` C++
// This function simply shifts the bytes in the word to the left and sets the last byte
// to the old first byte 
unsigned char *rotWord(unsigned char *b) {
  unsigned char c = b[0];
  b[0] = b[1];
  b[1] = b[2];
  b[2] = b[3];
  b[3] = c;
  return b;}
  
// This function uses the SBox lookup to replace each byte in the word with its 
// equivalent substitution
unsigned char *subWord(unsigned char *b) {
  for (int i = 0; i < 4; ++i)
    b[i] = subBytes(b[i]); 
  return b;}
```
## 128-bit keysize encryption:
Before we begin the iteration of functions we perform the addRoundKey() function on the initial state. This function simply XOR's the current 16-byte block with a section of the expanded key which is determined by the current round.
``` C++
unsigned char *addRoundkey(unsigned char *state, unsigned char *expandedkey, int offset) {
  for (int i = 0; i < 16; ++i)
    state[i] = state[i] ^ expandedkey[offset+i]; 
  return state;}
```
After that I apply the following functions for the next 9 rounds:
``` C++
for (int n = 0; n < 16; ++n)
  state[n] = subBytes(state[n]);
state = shiftRows(state);
state = mixColumns(state);
state = addRoundkey(state, expandedKey, (cur_round+1)*16);
```
The last round is similar except there is not mixColumns call for it. Here are the functions used:
``` C++ 
// This function gets the corresponding SBox byte to the given byte 'b' 
int subBytes(unsigned char b) {
  return Sbox[b];}
 
//This function rearranges our state by using the matrix instantiated in r[]
unsigned char *shiftRows(unsigned char *b) {
  unsigned char r[16] = {
    b[0], b[5], b[10], b[15],
    b[4], b[9], b[14], b[3],
    b[8], b[13], b[2], b[7],
    b[12], b[1], b[6], b[11]};
  for (int i = 0; i < 16; ++i)
    b[i] = r[i];
  return b;}
  
// This function transforms the bytes by treating them as four-term polynomials
// Instead of doing all the calculations I simply have a look up table at the beginning
// of my file file that automates the outcomes of the operations.
unsigned char *mixColumns(unsigned char *b) {
  for (int i = 0; i < 4; ++i) {
    int x1, x2, x3, x4, l1, l2;
    x1 = b[(i*4)];
    x2 = b[(i*4)+1];
    x3 = b[(i*4)+2];
    x4 = b[(i*4)+3];
    //Missing some overflow logic to condense code, look at aes.c++ for entire code
    l1 = Lbox[x1] + Lbox[2];
    l2 = Lbox[x2] + Lbox[3];
    b[(i*4)] = Ebox[l1] ^ Ebox[l2] ^ (x3) ^ (x4);
    l1 = Lbox[x2] + Lbox[2];
    l2 = Lbox[x3] + Lbox[3];
    b[(i*4)+1] = (x1) ^ Ebox[l1] ^ Ebox[l2] ^ (x4);
    l1 = Lbox[x3] + Lbox[2];
    l2 = Lbox[x4] + Lbox[3];
    b[(i*4)+2] = (x1) ^ (x2) ^ Ebox[l1] ^ Ebox[l2];
    l1 = Lbox[x4] + Lbox[2];
    l2 = Lbox[x1] + Lbox[3];
    b[(i*4)+3] = Ebox[l2] ^ (x2) ^ (x3) ^ Ebox[l1];
  }
  return b;}
```
Once all that is done, we have finished encryption using AES with a 128-bit key!

