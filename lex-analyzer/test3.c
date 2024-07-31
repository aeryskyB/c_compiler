int i = 12 * 11 - 10 / 2;
int j = (20 << 3) + (20 >> 1);
int k = i ^ j;
int l=+5, m=-100;

if (i > 100 && i <= 150 && j >= 50 && j < 200 && l != m)
	k = (i     ==  j);

l = (i & j) | !k;

while (i > 0) {
	i = i - 10;
}

for (int _ = j; _ > 0; _--) l++;

