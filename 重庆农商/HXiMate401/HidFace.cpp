#include "stdafx.h"
#include <stdio.h>
#include "hidapi.h"
#include "HidFace.h"

static int sg_fd=-1;
static unsigned char sg_hidbuffer[2500]={0};
static long sg_hiddatatahead = 0;
static int sg_hiddatatail = 0;


unsigned char ucHidOpen(unsigned short vendor_id, unsigned short product_id)
{
	hid_device *handle;

	if ( sg_fd >=0 ) {
		hid_close((hid_device *)sg_fd);
		sg_fd = -1;
	}

	handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
	if ( !handle ) {
		sg_fd = -1;
 		return 2;
	}
	
	// Set the hid_read() function to be non-blocking.
  	hid_set_nonblocking(handle, 1);
  	
	sg_hiddatatahead = 0;
	sg_hiddatatail = 0;

	sg_fd = (int)handle;
   	
   	return 0;
}

int iGetHidHandle(void)
{
	return sg_fd;
}

unsigned char ucHidClose(void)
{
	if ( sg_fd < 0 )
		return 0;
	hid_close((hid_device *)sg_fd);

	sg_fd = -1;

	return 0;
}

unsigned char ucHidReset(void)
{
	unsigned char buffer[64];
	
	if ( sg_fd < 0 )
		return 1;
		
	while ( hid_read_timeout((hid_device *)sg_fd, buffer, 64, 0) > 0 );
	
	sg_hiddatatahead = 0;
	sg_hiddatatail = 0;

	return 0;
}

unsigned char ucHidTest(void)
{
	int ret;
    unsigned char buffer[64];
    
	if ( sg_fd < 0 )
		return 0;
    
    if ( sg_hiddatatail - sg_hiddatatahead )
    	return 1;
	else
		sg_hiddatatail = sg_hiddatatahead = 0;
	
  	ret = hid_read_timeout((hid_device *)sg_fd, buffer, 64, 0);

  	if ( ret > 0 && ret == 64 && buffer[0] == 0x02 ) {
		if ( sg_hiddatatail+buffer[1] < sizeof(sg_hidbuffer) ) {
			memcpy(sg_hidbuffer+sg_hiddatatail,buffer+2,(unsigned int)buffer[1]);
			sg_hiddatatail += (unsigned int)buffer[1];
		}
  		return 1;
	}
	
	return 0;
}

unsigned char ucHidGet(void)
{
	unsigned char c;

	while (ucHidGetBuf(&c,1,1000)!=0 )
		;;

	return (c);
}

unsigned char ucHidSendBuf(void *pBuf, unsigned int uiLen)
{
    int offset;
    unsigned char buffer[64],ucPackeNo = 1;
    
	if ( sg_fd < 0 )
		return 1;
    
    offset = 0;
    while ( uiLen ) {
    	memset(buffer,0,sizeof(buffer));
    	buffer[0] = 1;
	    if ( uiLen >= 62 ) {
	    	buffer[1] = 62;
	    	memcpy(buffer+2,(unsigned char*)pBuf+offset,62);
	    	uiLen -= 62;
			offset += 62;
	    }
	    else {
	    	buffer[1] = uiLen;
	    	memcpy(buffer+2,(unsigned char*)pBuf+offset,uiLen);
	    	uiLen = 0;
	    }
	    if ( hid_write((hid_device *)sg_fd, buffer, 64) < 0 )
	    	return 2;
	}
	return 0;
}


unsigned char ucHidGetBufBymillsecond(void *pBuf, unsigned int uiLen, unsigned int uiTimeOut)
{
	int ret;
    unsigned long ulTimer;
    int offset,bufferdatalen;
    unsigned char buffer[64];
    
	if ( sg_fd < 0 )
		return -1;

    //_vSetTimer(&ulTimer,uiTimeOut*100L);

	ulTimer = GetTickCount();

	offset = 0;
   	while ( uiLen )
   	{	   			  			
    	if ( GetTickCount() > ulTimer + (unsigned long)uiTimeOut)
    		return 1;

		//如果缓冲区没有有效数据,将指针清零
		if ( sg_hiddatatail == sg_hiddatatahead )
			sg_hiddatatail = sg_hiddatatahead = 0;
	
		memset(buffer,0,sizeof(buffer));
	  	ret = hid_read_timeout((hid_device *)sg_fd, buffer, 64, 0);
  		if ( ret < 0 )
  			return 2;

		if ( ret > 0 && ret == 64 && buffer[0] == 0x02 ) {
			if ( sg_hiddatatail >= sizeof(sg_hidbuffer) )
				return 3;
			memcpy(sg_hidbuffer+sg_hiddatatail,buffer+2,(unsigned int)buffer[1]);
			sg_hiddatatail += (unsigned int)buffer[1];
		}	
		bufferdatalen = sg_hiddatatail - sg_hiddatatahead;
   		if ( bufferdatalen ) {
   			if ( uiLen <= (unsigned int)bufferdatalen ) {
	   			memcpy((unsigned char*)pBuf+offset,sg_hidbuffer+sg_hiddatatahead,uiLen);
	   			sg_hiddatatahead += uiLen;
	   			offset += uiLen;
	   			uiLen = 0;
	   		}
	   		else {
	   			memcpy((unsigned char*)pBuf+offset,sg_hidbuffer+sg_hiddatatahead,bufferdatalen);
	   			offset += bufferdatalen;
	   			uiLen -= bufferdatalen;
	   			sg_hiddatatail = sg_hiddatatahead = 0;
	   		}
	   	}
	}		
	return 0;
}



unsigned char ucHidGetBuf(void *pBuf, unsigned int uiLen, unsigned int uiTimeOut)
{
	int ret;
    unsigned long ulTimer;
    int offset,bufferdatalen;
    unsigned char buffer[64];
    
	if ( sg_fd < 0 )
		return -1;

    //_vSetTimer(&ulTimer,uiTimeOut*100L);

	ulTimer = GetTickCount();

	offset = 0;
   	while ( uiLen )
   	{	   			  			
    	if ( GetTickCount() > ulTimer + (unsigned long)uiTimeOut*1000L )
    		return 1;

		//如果缓冲区没有有效数据,将指针清零
		if ( sg_hiddatatail == sg_hiddatatahead )
			sg_hiddatatail = sg_hiddatatahead = 0;
	
		memset(buffer,0,sizeof(buffer));
	  	ret = hid_read_timeout((hid_device *)sg_fd, buffer, 64, 0);
  		if ( ret < 0 )
  			return 2;

		if ( ret > 0 && ret == 64 && buffer[0] == 0x02 ) {
			if ( sg_hiddatatail >= sizeof(sg_hidbuffer) )
				return 3;
			memcpy(sg_hidbuffer+sg_hiddatatail,buffer+2,(unsigned int)buffer[1]);
			sg_hiddatatail += (unsigned int)buffer[1];
		}	
		bufferdatalen = sg_hiddatatail - sg_hiddatatahead;
   		if ( bufferdatalen ) {
   			if ( uiLen <= (unsigned int)bufferdatalen ) {
	   			memcpy((unsigned char*)pBuf+offset,sg_hidbuffer+sg_hiddatatahead,uiLen);
	   			sg_hiddatatahead += uiLen;
	   			offset += uiLen;
	   			uiLen = 0;
	   		}
	   		else {
	   			memcpy((unsigned char*)pBuf+offset,sg_hidbuffer+sg_hiddatatahead,bufferdatalen);
	   			offset += bufferdatalen;
	   			uiLen -= bufferdatalen;
	   			sg_hiddatatail = sg_hiddatatahead = 0;
	   		}
	   	}
	}		
	return 0;
}

//0 没有打开 1已经打开
unsigned char ucHidIfOpened(void)
{
	if ( sg_fd == -1 )
		return 0;

	return 1;
}

unsigned char ucHidTestOnLine(void)
{
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	int res;

	if ( sg_fd < 0 )
		return 1;

 	// Read the Product String
 	wstr[0] = 0x0000;
 	res = hid_get_product_string((hid_device *)sg_fd, wstr, MAX_STR);
	if (res < 0) 
		return 1;

	return 0;
}
