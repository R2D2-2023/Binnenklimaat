#include "crc8.hpp"

unsigned char Crc8::crc8Atm (char *buf, unsigned char len){
	unsigned char crc = 0;
	if (len){
        do{
		    crc = *buf++ ^ poly_tab [crc];
        }
	    while (--len);
    } 
    return ~crc;
}

unsigned char Crc8::crc8AtmCheck (char *buf, unsigned char len){
    unsigned char bcrc;
    // Read crc from *buf
    bcrc = readUchar(buf, len, 3);
    if (bcrc == crc8Atm(buf, len)){
        return 0;
    }
    else{
        return 1;
    }
}

unsigned char Crc8::readUchar(char *buf, unsigned char offset, unsigned char length){
    unsigned char value = 0;
    while (length--){
        value = value * 10 + (*(buf+offset++) - '0');
    }
    return value;
}