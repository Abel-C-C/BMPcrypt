# BMPcrypt

<h3>Preface/Warning</h3>

Don't try to secure sensitive data with this program. This program was made to learn about specific cryptographic protocols and their application. This program has not gone through the proper analysis to be considered secure, and should therefore be assumed to not be. Once again, this program was made out of personal interest and was considered to be more of a learning opportunity, than a secure standard or implementation of any cryptosystem. Despite this, I will assume throughout this file that the program WILL be used to secure real data, and will explain my design decisions for the sake of self-education, and once again, personal interest.

BMPcrypt is a cryptographic algorithm based on AES-128 in PCBC mode. It uses the data from BPM images to encrypt and decrypt data in text files. There is also a second program to pseudo-randomly and deterministically generate such data, and save it as a BMP image using SDL2. The BMP image will act as a key for the encrpytion, once you encrypt or decrypt a file with the BMP image, the BMP image will be overwritten with 0's.



<h3>How Key (BMP) Generation Works</h3>

The <i>BMPkey</i> program will generate BMP images based off of user input and save the image to the disk. The user will be prompted to enter the width and height of the image, followed by a u16-bit integer that represents the number of extra pixels generated after the full image is computed. You will also be prompted to enter 4 different u64-bit values for seeds in the PRNG and 4 different u16-bit values for the iterations in the PRNG. These values will decide the RGB colors generated, as well as the location of the extra pixels. After this the user enters the file name and the image is generated on the SDL backbuffer column by column until it is complete. Once the image is generated, its rendered by SDL and saved to the same directory. If you were going to secure data with this program, it would obviously be important to keep these values secret. You can essentially think of this program as a glorified hashing algorithm.



<h3>How Encryption Works</h3>

The <i>BMPcrypt</i> program will read RGB data from a BMP image and use it to encrypt/decrypt text files. It will then overwrite the image with 0's. As mentioned above, the algorithm used for this encryption was based off of AES-128 in PCBC mode.

<strong>-AES 128 and the BMPcrypt Algorithm</strong>

Since I was not familiar with AES, I used 128 bit keys since it was easier to implement and keep track of during development. AES 128 uses 10 rounds and 128 bit keys to encrypt and decrypt blocks of data. The keys for the encryption are derived from the RGB values in the BMP image. Mixed in with the standard rounds and transformations of AES, are transformations that use the BMP data <i>not involved in key generation</i> during the encryption.

<strong>-Propagating Cipher Block Chaining</strong>

I also used a more basic mode of operation (CBC), before switching to PCBC since it greatly increased the security. PCBC makes it possible for 2 (almost) identical keys with a difference of 1 pixel's individual RGB value, fail to decrypt the first block into plaintext, ruining the rest of the encryption. Similar to the keys used during the AES encryption, the IV is also generated using BMP data.

(TBC)
