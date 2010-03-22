#include "Tables.h"

#include "Granule.h"

string binary(short x, int length) {
	string out = "";
	byte* y = (byte*) &x;
	if(length > 8) {
		out += binary(y[1], length - 8);
		length = 8;
	}
	out += binary(y[0], length);
	return out;
}

string binary(byte x, int length) {
	string out = "";
	for(int i = 0; i < length; i++) {
		int position = length - i - 1;
		byte mask = 1 << position;
		out += mask & x ? "1" : "0";
	}
	return out;
}

void buildLookup() {
	int i = 0;

	// build lookup table for 3^4=81 small values in table a+b
	for(int a = -1; a <= +1; a++) {
		for(int b = -1; b <= +1; b++) {
			for(int c = -1; c <= +1; c++) {
				for(int d = -1; d <= +1; d++) {
					int vals[4] = {a, b, c, d};
					byte quad = 0;
					int quadPosition = 0;
					byte signs = 0;
					int signsPosition = 0;

					// build the 4-bit index and signs
					for(int j = 0; j < 4; j++) {
						setBool(&quad, quadPosition, vals[j] != 0);
						if(vals[j] != 0)
							setBool(&signs, signsPosition, vals[j] == -1);
					}

					// right align both
					quad >>= 4;
					signs >>= 8 - signsPosition;

					byte htabAValue = htabA[quad][0];
					int htabALength = htabA[quad][1] + signsPosition;
					byte htabBValue = (~quad) & 0x0f;
					int htabBLength = 4 + signsPosition;

					short* sla = Granule::smallLookupA[i];
					sla[0] = htabAValue;
					sla[0] <<= signsPosition;
					sla[0] |= signs;
					sla[1] = htabALength;

					short* slb = Granule::smallLookupB[i];
					slb[0] = htabBValue;
					slb[0] <<= signsPosition;
					slb[0] |= signs;
					slb[1] = htabBLength;
/*
					cout << dec << a << " " << b << " " << c << " " << d << " " << hex << ": " <<
						binary(quad, 4) << " " << binary(signs, signsPosition) << endl;
					cout << binary(sla[0], sla[1]) <<	" " << binary(slb[0], slb[1]) << endl;
*/
					i++;
				}
			}
		}
	}
}
