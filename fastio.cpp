const int SZ = 1<<21;   
struct fastio{   //fast io
    char inbuf[SZ];  
    char outbuf[SZ];  
    fastio(){  
        setvbuf(stdin,inbuf,_IOFBF,SZ);  
        setvbuf(stdout,outbuf,_IOFBF,SZ);  
    }  
}io; 