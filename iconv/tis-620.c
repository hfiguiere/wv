/* Originally contributed to mswordview by */
#include <stdlib.h>
#include <stdio.h>
#include "wv.h"
#include "iconv_internal.h"



#define	Wchar1 (1UL<<7)-1

union {
	unsigned char ch[2];
	unsigned long ulong;
} un;

struct unichar {
	unsigned char ch0;
	unsigned char ch1;
};

static struct unichar tis2uni[256] = {
	/* 0x00*/
	{0x00, 0x00}, {0x01, 0x00}, {0x02, 0x00}, {0x03, 0x00},
	{0x04, 0x00}, {0x05, 0x00}, {0x06, 0x00}, {0x07, 0x00},
	{0x08, 0x00}, {0x09, 0x00}, {0x0a, 0x00}, {0x0b, 0x00},
	{0x0c, 0x00}, {0x0d, 0x00}, {0x0e, 0x00}, {0x0f, 0x00},
	/* 0x10*/
	{0x10, 0x00}, {0x11, 0x00}, {0x12, 0x00}, {0x13, 0x00},
	{0x14, 0x00}, {0x15, 0x00}, {0x16, 0x00}, {0x17, 0x00},
	{0x18, 0x00}, {0x19, 0x00}, {0x1a, 0x00}, {0x1b, 0x00},
	{0x1c, 0x00}, {0x1d, 0x00}, {0x1e, 0x00}, {0x1f, 0x00},
	/* 0x20*/
	{0x20, 0x00}, {0x21, 0x00}, {0x22, 0x00}, {0x23, 0x00},
	{0x24, 0x00}, {0x25, 0x00}, {0x26, 0x00}, {0x27, 0x00},
	{0x28, 0x00}, {0x29, 0x00}, {0x2a, 0x00}, {0x2b, 0x00},
	{0x2c, 0x00}, {0x2d, 0x00}, {0x2e, 0x00}, {0x2f, 0x00},
	/* 0x30*/
	{0x30, 0x00}, {0x31, 0x00}, {0x32, 0x00}, {0x33, 0x00},
	{0x34, 0x00}, {0x35, 0x00}, {0x36, 0x00}, {0x37, 0x00},
	{0x38, 0x00}, {0x39, 0x00}, {0x3a, 0x00}, {0x3b, 0x00},
	{0x3c, 0x00}, {0x3d, 0x00}, {0x3e, 0x00}, {0x3f, 0x00},
	/* 0x40*/
	{0x40, 0x00}, {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00},
	{0x44, 0x00}, {0x45, 0x00}, {0x46, 0x00}, {0x47, 0x00},
	{0x48, 0x00}, {0x49, 0x00}, {0x4a, 0x00}, {0x4b, 0x00},
	{0x4c, 0x00}, {0x4d, 0x00}, {0x4e, 0x00}, {0x4f, 0x00},
	/* 0x50*/
	{0x50, 0x00}, {0x51, 0x00}, {0x52, 0x00}, {0x53, 0x00},
	{0x54, 0x00}, {0x55, 0x00}, {0x56, 0x00}, {0x57, 0x00},
	{0x58, 0x00}, {0x59, 0x00}, {0x5a, 0x00}, {0x5b, 0x00},
	{0x5c, 0x00}, {0x5d, 0x00}, {0x5e, 0x00}, {0x5f, 0x00},
	/* 0x60*/
	{0x60, 0x00}, {0x61, 0x00}, {0x62, 0x00}, {0x63, 0x00},
	{0x64, 0x00}, {0x65, 0x00}, {0x66, 0x00}, {0x67, 0x00},
	{0x68, 0x00}, {0x69, 0x00}, {0x6a, 0x00}, {0x6b, 0x00},
	{0x6c, 0x00}, {0x6d, 0x00}, {0x6e, 0x00}, {0x6f, 0x00},
	/* 0x70*/
	{0x70, 0x00}, {0x71, 0x00}, {0x72, 0x00}, {0x73, 0x00},
	{0x74, 0x00}, {0x75, 0x00}, {0x76, 0x00}, {0x77, 0x00},
	{0x78, 0x00}, {0x79, 0x00}, {0x7a, 0x00}, {0x7b, 0x00},
	{0x7c, 0x00}, {0x7d, 0x00}, {0x7e, 0x00}, {0x7f, 0x00},
	/* 0x80*/
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
	{0x00, 0x00}, {0x26, 0x20}, {0x00, 0x00}, {0x00, 0x00},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
	/* 0x90*/
	{0x00, 0x00}, {0x18, 0x20}, {0x19, 0x20}, {0x1c, 0x20},
	{0x1d, 0x20}, {0x22, 0x20}, {0x13, 0x20}, {0x14, 0x20},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
	/* 0xa0*/
	{0xa0, 0x00}, {0x01, 0x0e}, {0x02, 0x0e}, {0x03, 0x0e},
	{0x04, 0x0e}, {0x05, 0x0e}, {0x06, 0x0e}, {0x07, 0x0e},
	{0x08, 0x0e}, {0x09, 0x0e}, {0x0a, 0x0e}, {0x0b, 0x0e},
	{0x0c, 0x0e}, {0x0d, 0x0e}, {0x0e, 0x0e}, {0x0f, 0x0e},
	/* 0xb0*/
	{0x10, 0x0e}, {0x11, 0x0e}, {0x12, 0x0e}, {0x13, 0x0e},
	{0x14, 0x0e}, {0x15, 0x0e}, {0x16, 0x0e}, {0x17, 0x0e},
	{0x18, 0x0e}, {0x19, 0x0e}, {0x1a, 0x0e}, {0x1b, 0x0e},
	{0x1c, 0x0e}, {0x1d, 0x0e}, {0x1e, 0x0e}, {0x1f, 0x0e},
	/* 0xc0*/
	{0x20, 0x0e}, {0x21, 0x0e}, {0x22, 0x0e}, {0x23, 0x0e},
	{0x24, 0x0e}, {0x25, 0x0e}, {0x26, 0x0e}, {0x27, 0x0e},
	{0x28, 0x0e}, {0x29, 0x0e}, {0x2a, 0x0e}, {0x2b, 0x0e},
	{0x2c, 0x0e}, {0x2d, 0x0e}, {0x2e, 0x0e}, {0x2f, 0x0e},
	/* 0xd0*/
	{0x30, 0x0e}, {0x31, 0x0e}, {0x32, 0x0e}, {0x33, 0x0e},
	{0x34, 0x0e}, {0x35, 0x0e}, {0x36, 0x0e}, {0x37, 0x0e},
	{0x38, 0x0e}, {0x39, 0x0e}, {0x3a, 0x0e}, {0x00, 0x00},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x3f, 0x0e},
	/* 0xe0*/
	{0x40, 0x0e}, {0x41, 0x0e}, {0x42, 0x0e}, {0x43, 0x0e},
	{0x44, 0x0e}, {0x45, 0x0e}, {0x46, 0x0e}, {0x47, 0x0e},
	{0x48, 0x0e}, {0x49, 0x0e}, {0x4a, 0x0e}, {0x4b, 0x0e},
	{0x4c, 0x0e}, {0x4d, 0x0e}, {0x4e, 0x0e}, {0x4f, 0x0e},
	/* 0xf0*/
	{0x50, 0x0e}, {0x51, 0x0e}, {0x52, 0x0e}, {0x53, 0x0e},
	{0x54, 0x0e}, {0x55, 0x0e}, {0x56, 0x0e}, {0x57, 0x0e},
	{0x58, 0x0e}, {0x59, 0x0e}, {0x5a, 0x0e}, {0x5b, 0x0e},
	{0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
};

size_t wvConvertUnicodeToTIS620(const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
    {
    size_t ret=0;
    U16 c1,c2;
    while(  (*inbytesleft) && (*outbytesleft)   )
        {
		c2 = (U8)**inbuf;
        (*inbuf)++;
        c2 = (c2<< 8)&0xFF00;
        c2 += (U8)**inbuf;
        (*inbuf)++;
        (*inbytesleft)-=2;
		
        c1 = ConvertUnicodeToTIS620(c2);
        if (c1 == 0xffff)
            **outbuf = '?';
        else
            **outbuf = (char)c1;
        (*outbuf)++;
        (*outbytesleft)--;
        ret++;
        }
    return(ret);
    }


U16 ConvertUnicodeToTIS620(U16 wc)
	{
	U16 ret;
	if(wc & (~Wchar1)) 
		{
		un.ulong = wc;
		ret = un.ch[0];
		if(un.ch[1] == 0x0e) 
			ret += 0xa0;
		return(ret);
		}
	ret = 0x00 | wc;
	return(ret);
	}
