#include <stdio.h>

int main(void){
	unsigned int temp,temp1,temp2,code,pair1,pair2;
	int byteCount;
	int byte1,byte2,byte3,byte4;
	int flag = 1;

	byte1=getchar();                                            	//Read first byte
	byte2=getchar();                                            	//Read second byte
	pair1 = byte1 << 8;                                        	    //Shift first byte 8 bits left and...
	pair1 = pair1 +  byte2;                                     	//add the second byte to create the first pair.

	if((pair1>=0x0000 && pair1<=0xD7FF))                        	//If the first pair is in [0x0000,0xD7FF]...
		byteCount = 2;                                          	    //then it's a 2-byte sequence.
	else if((pair1>=0xD800 && pair1 <= 0xDBFF)){                	//Else if the first pair is in [0xD800,0xDBFF]
		byte3 = getchar();                                      	    //Read third byte.
		byte4 = getchar();                                      	    //Read fourth byte.
		pair2 = (byte3 << 8) +  byte4;                          	    //Create the second pair just like the first.

		if(pair2>=0xDC00 && pair2<=0xDFFF){                     //If the second pair is in [0xDC00,0xDFFF]...
			byteCount = 4;                                          //then it's a 4-byte sequence.
		}
		else                                                    //If the second pair is not in [0xDC00,0xDFFF], the sequence is illegal...
			flag = 0;                                      		    //so set flag = 0
	}
	else if(byte1 != EOF)                				        //If the first pair is not in [0xD800,0xDBFF],and is not EOF (EOF is legal) the sequence is illegal...
		flag = 0;		                                               	//so set flag = 0


	while(byte1 != EOF && flag != 0){           			//While first two bytes are not EOF, and the sequence is legal.
		if(byteCount == 2)                                      //If its a 2-byte sequence...
			code = pair1;                                  		    //Unicode code point is the same as the sequence.
		if(byteCount == 4){                                     //If its a 4-byte sequence...
			temp1 = ((pair1 & 0x3FF)<< 10);               		    //Take the 10 right bits of the first pair and shift them 10 positions to the left,
			temp2 = (pair2 & 0x3FF);                       		    //take the  and the 10 right bits of the second pair.
			code = temp1 + temp2 +  0x10000;                	    //Set code equal to the addition of these new bit strings of length 10
		}
		if(code>=0x0000 && code<=0x007F){                      	//If code point is in [0x0000,0x007F]
			putchar(code);                                 		    //print the code as is.
		}
		else if(code>=0x0080 && code<=0x07FF){                  //Else if code point is in[0x0080,0x07FF]
			temp = 0xC0 + ((code & 0x7C0)>>6);                      //set temp equal to 110xxxxx where x are the 5 left most bits of code.
			putchar(temp);                                          //print temp
			temp = 0x80 + (code & 0x3F);                            //set temp equalt to 10xxxxxx where x are the 6 right most bits of code.
			putchar(temp);                                          //printf temp
		}
		else if(code>=0x0800 && code<=0xFFFF){                  //Else if code point is in [0x0800,0xFFFF]
			temp = 0xE0 + ((code & 0xF000)>>12);                    //set temp equal to 1110xxxx where x are the 4 left most bits of code.
			putchar(temp);                                          //print temp.
			temp = 0x80 + ((code & 0xFC0)>>6);                      //set temp equal to 10xxxxxx where x are the next 6 bits of code.
			putchar(temp);                                          //print temp.
			temp = 0x80 + ((code & 0x3F));                          //set temp equal to 10xxxxxx where x are the 6 left most bits of code.
			putchar(temp);
		}
		else if(code>=0x010000 && code<=0x10FFFF){              //Else if code point is in [0x010000,0x10FFFF]
			temp = 0xF0 + ((code & 0x1C0000)>>18);                  //set temp equal to 11110xxx where x are the 3 left most bits of code.
			putchar(temp);                                          //print temp.
			temp = 0x80 + ((code & 0x3F000)>>12);                   //set temp equal to 10xxxxxx where x are the next 6 bits of code.
			putchar(temp);                                          //print temp.
			temp = 0x80 + ((code & 0xFC0)>>6);                      //set temp equal to 10xxxxxx where x are the next 6 bits of code.
			putchar(temp);                                          //print temp.
			temp = 0x80 + (code & 0x3F);                            //set temp equal to 10xxxxxx where x are the right most 6 bits of code.
			putchar(temp);                                          //print temp.
		}
		byte1=getchar();                                        	//Read first byte of next sequence.
		byte2=getchar();                                            	//Read second byte of next sequence.
		pair1 = byte1 << 8;            	                            	//Shift first byte 8 bits left and...
		pair1 = pair1 + byte2;		                                //add the second byte to create the first pair.

	if(byte1 != EOF && flag !=0){           			        //If first byte is not EOF, and the sequence is legal.
		if((pair1>=0x0000 && pair1<=0xD7FF))                	    		//If the first pair is in [0x0000,0xD7FF]...
                	byteCount = 2;                                  		    //then it's a 2-byte sequence.
            	else if((pair1>=0xD800 && pair1 <= 0xDBFF)){				//Else if the first pair is in [0xD800,0xDBFF]
                	byte3 = getchar();							//Read third byte.
                	byte4 = getchar();							//Read fourth byte.
                	pair2 = (byte3 << 8) +  byte4;						//Create the second pair just like the first.

                	if(pair2>=0xDC00 && pair2<=0xDFFF)              	        	//If the second pair is in [0xDC00,0xDFFF]...
                    		byteCount = 4;                              		    		//then it's a 4-byte sequence.
                	else									//If the second pair is not in [0xDC00,0xDFFF], then sequence is illegal
                    		flag = 0;                                                   		//so set flag = 0
            	}
            	else									//If the first pair is not in [0xDC00,0xDFFF], the sequence is illegal...
                	flag = 0;                                   				//so set flag = 0
	}
    }
	if(flag == 0)                                                		//If any sequence was illegal then print an error message.
		printf("Conversion was terminated. Invalid character in input text file.");

return 0;
}
