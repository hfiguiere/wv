#include <stdlib.h>
#include <stdio.h>
#include "wv.h"

void wvGetBX(BX *item,FILE *fd)
	{
	item->offset = getc(fd);
	wvGetPHE(&item->phe,0,fd);
	}
