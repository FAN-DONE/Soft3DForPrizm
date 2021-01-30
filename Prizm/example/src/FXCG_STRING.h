#ifndef FXCG_STRING_H
#define FXCG_STRING_H

static inline void Memset4(void* dst, int val, unsigned int dwords) {
	int* point = (int*)dst;
	for (unsigned int i = 0; i < dwords; i++) {
		*(point++) = val;
	}
}

char *Itoa(int num,char *str,int radix) {  
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; 
	int i=0,j,k; 
	if(radix==10&&num<0)  
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num;
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 
	if(str[0]=='-') k=1; 
	else k=0; 
	char temp; 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=str[j]; 
		str[j]=str[i-j-1]; 
		str[i-j-1]=temp; 
	} 
	return str; 
}

static unsigned int lastrandom = 0x12345678;

static inline void Srand(unsigned int seed) {
	lastrandom = seed;
}

static inline int Rand(void) {
	lastrandom = 0x41C64E6D * lastrandom + 0x3039;
	return lastrandom >> 16;
}


#endif
