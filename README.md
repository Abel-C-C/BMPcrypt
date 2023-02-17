# BMPcrypt

Warning/Preface: Don't try to secure sensitive data with this program. This program was made to learn about specific cryptographic protocols and their application. This program has not gone through the proper analysis to be considered secure, and should therefore be assumed to not be. Once again, this program was made out of personal interest and was considered to be more of a learning opportunity, than a secure standard or implementation of any cryptosystem. Despite this, I will assume throughout this file that the program WILL be used to secure real data, and will explain my design decisions for the sake of self-education, and once again, personal interest.



BMPcrypt is a cryptographic algorithm based on AES-128 in PCBC mode. It uses the data from BPM images to encrypt and decrypt data in text files. There is also a second program to pseudo-randomly and deterministically generate such data, and save it as a BMP image. The BMP image will act as a key for the encrpytion, once you encrypt or decrypt a file with the BMP image, the BMP image will be overwritten with 0's.

(TBC)
