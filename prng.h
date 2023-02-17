#include <iostream>
#include <vector>

/*

This file includes functions used to generate psuedo-random numbers
and manipulate the AES block cipher.

*/

//pre-defined feedback terms
constexpr uint64_t bytes_64[256] = {
  0x0000000000000000, 0xc70000000000000b, 0x8e0000000000000d, 0x4900000000000006, 0x1c00000000000001, 0xdb0000000000000a, 0x920000000000000c, 0x5500000000000007,
  0x3800000000000002, 0xff00000000000009, 0xb60000000000000f, 0x7100000000000004, 0x2400000000000003, 0xe300000000000008, 0xaa0000000000000e, 0x6d00000000000005,
  0x7000000000000004, 0xb70000000000000f, 0xfe00000000000009, 0x3900000000000002, 0x6c00000000000005, 0xab0000000000000e, 0xe200000000000008, 0x2500000000000003,
  0x4800000000000006, 0x8f0000000000000d, 0xc60000000000000b, 0x0100000000000000, 0x5400000000000007, 0x930000000000000c, 0xda0000000000000a, 0x1d00000000000001,
  0xe000000000000008, 0x2700000000000003, 0x6e00000000000005, 0xa90000000000000e, 0xfc00000000000009, 0x3b00000000000002, 0x7200000000000004, 0xb50000000000000f,
  0xd80000000000000a, 0x1f00000000000001, 0x5600000000000007, 0x910000000000000c, 0xc40000000000000b, 0x0300000000000000, 0x4a00000000000006, 0x8d0000000000000d,
  0x900000000000000c, 0x5700000000000007, 0x1e00000000000001, 0xd90000000000000a, 0x8c0000000000000d, 0x4b00000000000006, 0x0200000000000000, 0xc50000000000000b,
  0xa80000000000000e, 0x6f00000000000005, 0x2600000000000003, 0xe100000000000008, 0xb40000000000000f, 0x7300000000000004, 0x3a00000000000002, 0xfd00000000000009,
  0xc00000000000000b, 0x0700000000000000, 0x4e00000000000006, 0x890000000000000d, 0xdc0000000000000a, 0x1b00000000000001, 0x5200000000000007, 0x950000000000000c,
  0xf800000000000009, 0x3f00000000000002, 0x7600000000000004, 0xb10000000000000f, 0xe400000000000008, 0x2300000000000003, 0x6a00000000000005, 0xad0000000000000e,
  0xb00000000000000f, 0x7700000000000004, 0x3e00000000000002, 0xf900000000000009, 0xac0000000000000e, 0x6b00000000000005, 0x2200000000000003, 0xe500000000000008,
  0x880000000000000d, 0x4f00000000000006, 0x0600000000000000, 0xc10000000000000b, 0x940000000000000c, 0x5300000000000007, 0x1a00000000000001, 0xdd0000000000000a,
  0x2000000000000003, 0xe700000000000008, 0xae0000000000000e, 0x6900000000000005, 0x3c00000000000002, 0xfb00000000000009, 0xb20000000000000f, 0x7500000000000004,
  0x1800000000000001, 0xdf0000000000000a, 0x960000000000000c, 0x5100000000000007, 0x0400000000000000, 0xc30000000000000b, 0x8a0000000000000d, 0x4d00000000000006,
  0x5000000000000007, 0x970000000000000c, 0xde0000000000000a, 0x1900000000000001, 0x4c00000000000006, 0x8b0000000000000d, 0xc20000000000000b, 0x0500000000000000,
  0x6800000000000005, 0xaf0000000000000e, 0xe600000000000008, 0x2100000000000003, 0x7400000000000004, 0xb30000000000000f, 0xfa00000000000009, 0x3d00000000000002,
  0x800000000000000d, 0x4700000000000006, 0x0e00000000000000, 0xc90000000000000b, 0x9c0000000000000c, 0x5b00000000000007, 0x1200000000000001, 0xd50000000000000a,
  0xb80000000000000f, 0x7f00000000000004, 0x3600000000000002, 0xf100000000000009, 0xa40000000000000e, 0x6300000000000005, 0x2a00000000000003, 0xed00000000000008,
  0xf000000000000009, 0x3700000000000002, 0x7e00000000000004, 0xb90000000000000f, 0xec00000000000008, 0x2b00000000000003, 0x6200000000000005, 0xa50000000000000e,
  0xc80000000000000b, 0x0f00000000000000, 0x4600000000000006, 0x810000000000000d, 0xd40000000000000a, 0x1300000000000001, 0x5a00000000000007, 0x9d0000000000000c,
  0x6000000000000005, 0xa70000000000000e, 0xee00000000000008, 0x2900000000000003, 0x7c00000000000004, 0xbb0000000000000f, 0xf200000000000009, 0x3500000000000002,
  0x5800000000000007, 0x9f0000000000000c, 0xd60000000000000a, 0x1100000000000001, 0x4400000000000006, 0x830000000000000d, 0xca0000000000000b, 0x0d00000000000000,
  0x1000000000000001, 0xd70000000000000a, 0x9e0000000000000c, 0x5900000000000007, 0x0c00000000000000, 0xcb0000000000000b, 0x820000000000000d, 0x4500000000000006,
  0x2800000000000003, 0xef00000000000008, 0xa60000000000000e, 0x6100000000000005, 0x3400000000000002, 0xf300000000000009, 0xba0000000000000f, 0x7d00000000000004,
  0x4000000000000006, 0x870000000000000d, 0xce0000000000000b, 0x0900000000000000, 0x5c00000000000007, 0x9b0000000000000c, 0xd20000000000000a, 0x1500000000000001,
  0x7800000000000004, 0xbf0000000000000f, 0xf600000000000009, 0x3100000000000002, 0x6400000000000005, 0xa30000000000000e, 0xea00000000000008, 0x2d00000000000003,
  0x3000000000000002, 0xf700000000000009, 0xbe0000000000000f, 0x7900000000000004, 0x2c00000000000003, 0xeb00000000000008, 0xa20000000000000e, 0x6500000000000005,
  0x0800000000000000, 0xcf0000000000000b, 0x860000000000000d, 0x4100000000000006, 0x1400000000000001, 0xd30000000000000a, 0x9a0000000000000c, 0x5d00000000000007,
  0xa00000000000000e, 0x6700000000000005, 0x2e00000000000003, 0xe900000000000008, 0xbc0000000000000f, 0x7b00000000000004, 0x3200000000000002, 0xf500000000000009,
  0x980000000000000c, 0x5f00000000000007, 0x1600000000000001, 0xd10000000000000a, 0x840000000000000d, 0x4300000000000006, 0x0a00000000000000, 0xcd0000000000000b,
  0xd00000000000000a, 0x1700000000000001, 0x5e00000000000007, 0x990000000000000c, 0xcc0000000000000b, 0x0b00000000000000, 0x4200000000000006, 0x850000000000000d,
  0xe800000000000008, 0x2f00000000000003, 0x6600000000000005, 0xa10000000000000e, 0xf400000000000009, 0x3300000000000002, 0x7a00000000000004, 0xbd0000000000000f,
};


//prng(64 bit lfsr) used throughout both programs
uint64_t randGen(uint64_t seedIter, uint16_t count) {
	
	for (int i = 0; i < count; i++) {

		seedIter = (seedIter >> 8) ^ bytes_64[seedIter & 0xff];
	}

	return seedIter;
}

//initialize cipherkey and IV
void keyInit(uint64_t &start,
		unsigned char block_key[4][4], 
		std::vector<unsigned char> R, 
		std::vector<unsigned char> G, 
		std::vector<unsigned char> B) {

        for (int col = 0; col < 4; col++) {

		block_key[0][col] = randGen(start + col, (start + col + 4) % 65535);
		
		block_key[1][col] = randGen( R[(start + col) % R.size()], 
				R[(start + col + 4) % R.size()] );
		block_key[2][col] = randGen( G[(start + col) % G.size()], 
				G[(start + col + 4) % G.size()] );
		block_key[3][col] = randGen( B[(start + col) % B.size()], 
				B[(start + col + 4) % B.size()] );
	}
	start += 8;
}

//change key with bmp values during key schedule
void keyGen(uint64_t &start,
		unsigned char block_key[4][4], 
		std::vector<unsigned char> R, 
		std::vector<unsigned char> G, 
		std::vector<unsigned char> B) {
	
        for (int col = 0; col < 4; col++) {

		block_key[0][col] ^= ( 
			R[(start + col) % R.size()] * 
			G[(start + col) % G.size()] * 
			B[(start + col) % B.size()]
			) % 256;
			
		block_key[1][col] ^= R[(start + col) % R.size()];
		block_key[2][col] ^= G[(start + col) % G.size()];
		block_key[3][col] ^= B[(start + col) % B.size()];
	}
	start += 4;
	
	//int loop = 0;
	//loop = (start / R.size());

	//std::cout << "bmp(key) num: " << start << "     loop num: " << loop << '\n';
}

void add_PCBC(unsigned char block_now[4][4], unsigned char block_before[4][4]) {

        for (int col = 0; col < 4; col++) {
                for (int row = 0; row < 4; row++) {

                        //xor block value by previous block value
                        block_now[col][row] ^= block_before[col][row];
                }
        }
}

void addBmp(uint64_t &start, unsigned int round, bool mode,
		unsigned char block_cipher[4][4],
		std::vector<unsigned char> R,
		std::vector<unsigned char> G,
		std::vector<unsigned char> B) {

	//red values determine which 2 bytes to xor by green and blue values
	unsigned char blockRow, blockCol, bmpOne, bmpTen;

	if (!mode) {
		
		bmpOne = R[(start) % R.size()] % 16;
		bmpTen = R[(start) % R.size()] / 16;
		
		blockRow = bmpOne % 4;
		blockCol = bmpOne / 4;
		block_cipher[blockRow][blockCol] ^= G[start % G.size()];
		
		blockRow = bmpTen % 4;
		blockCol = bmpTen / 4;
		block_cipher[blockRow][blockCol] ^= B[start % B.size()];

		start++;
	} else {
		
		//use appropriate pixel value during encryption
		unsigned int startSub = (round * 5) + 1;


		bmpOne = R[(start - startSub) % R.size()] % 16;
		bmpTen = R[(start - startSub) % R.size()] / 16;
		
		blockRow = bmpOne % 4;
		blockCol = bmpOne / 4;
		block_cipher[blockRow][blockCol] ^= G[(start - startSub) % G.size()];


		blockRow = bmpTen % 4;
		blockCol = bmpTen / 4;
		block_cipher[blockRow][blockCol] ^= B[(start - startSub) % B.size()];

	}
}

unsigned int ternaryTrans(unsigned int val) {

	//convert bmp value to base 3

	unsigned int ternaryNum = 0;
	unsigned int rem = 0;
	int i = 1;
	
	while (val > 0) {

		rem = val % 3;
		val /= 3;

		ternaryNum += (rem * i);
		i *= 10;
	}

	return ternaryNum;
}

void extraCharDel(unsigned char block_X[4][4], int extraChar) {

        for (int row = (extraChar / 4); row < 4; row++) {
                for (int col = (extraChar % 4); col < 4; col++) {

                        block_X[row][col] = 0;
                }
        }
}
