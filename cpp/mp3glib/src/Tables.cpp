#include "Tables.h"

#include "Granule.h"

void buildLookup() {
	int i = 0;

	int htabASum = 0;
	int htabBSum = 0;

	// build lookup table for 3^4=81 small values in table a+b
	for(int a = -1; a <= +1; a++) {
		for(int b = -1; b <= +1; b++) {
			for(int c = -1; c <= +1; c++) {
				for(int d = -1; d <= +1; d++) {
					byte quad = 0;
					int position = 0;
					int vals[4] = {a, b, c, d};
					//cout << dec << a << " " << b << " " << c << " " << d << " " << hex << (int) quad << endl;

					// build the 4-bit quadruple index
					for(int j = 0; j < 4; j++)
						setBool(&quad, position, vals[j] != 0);

					quad >>= 4;
					int htabALength = htabA[quad][1];
					int htabBLength = 4;

					int extra = 0;
					for(int j = 0; j < 4; j++)
						if(vals[j] != 0)
							extra++;

					cout << (int) htabALength << "," << htabBLength << "/" << extra << "=" << (htabALength+extra) << "," << (htabBLength+extra) << endl;

					htabASum += htabALength+extra;
					htabBSum += htabBLength+extra;

					i++;
				}
			}
		}
	}

	cout << "htabA ~ " << ((float) htabASum) / (float) 81 << endl;
	cout << "htabB ~ " << ((float) htabBSum) / (float) 81 << endl;
}
