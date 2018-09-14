# AES
Assignment is to implement AES-128 and AES-256 in the same program. We will be following the AES standard. Use Electronic Code Book (ECB) mode. 
The program should run like this: 

./program --keysize $KEYSIZE --keyfile $KEYFILE --inputfile $INPUTFILE --outputfile$OUTFILENAME --mode $MODE 
keysize: Either 128 or 256 bits keyfile: Should take in a keyfile that fits one of the specified sizes. 
To generate a key, use: head -c 16 &lt; /dev/urandom > key (this generates 16 random bytes ~ 128 bits) 
inputfile: This should be able to handle any file and any size (padding might be required). 
All we care about are the bytes of the file. 
To get the byte representation of a file use xxd outpfile: what to save the result mode: encrypt or decrypt
