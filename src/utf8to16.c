#include <stdio.h>

int main(void){

	int counter,i,flag,ch1,ch,temp,shiftTemp;
	unsigned int code,firstSur,secndSur;
	flag = 1;
	ch1 = getchar();                                            //Read first byte of each sequence

	while(ch1 != EOF && flag == 1){                             //While its not EOF...
		i = 1;
		counter = 0;
		temp = 0x80;                                            //Temp is 0x80 , which translates to 1000000 in binary.

		while((ch1 & temp) != 0){                               //While bitwise and between ch1 and temp is not zero...
			temp = (temp >> 1);                                     //Shift temp one position to the right.
			counter++;                                              //Add one to counter
		}                                                           //Basically this while loop counts how many consecutive "1"s ch1 has at its beginning.

		if(counter == 0)                                        //If counter is zero, then...
			code = ch1;                                         //The Unicode code point is the same as the UTF encoding.
		else{
            	if(counter == 1)                                    	//Else... if counter = 1,
                	flag = 0;                                       	    //then it's an illegal UTF-8 initial byte. Flag = 0.
            	else if(counter == 2)                               	//Else if counter = 2,
                    code = ch1 & 0x1F;                          		    //then set the code point equal to the last 5 bits of ch1.
            	else if(counter == 3)                               	//Else if counter = 3,
                    code = ch1 & 0xF;                               		//then set the code point equal to the last 4 bits of ch1.
		else if(counter == 4)                               	//Else if counter = 3,
			code = ch1 & 0x7;                               	    //then set the code point equal to the last 3 bits of ch1.
		else                                                	//In any other case, the first byte is illegal, set flag = 0.
			flag = 0;
		ch = getchar();                                     	//Read the next byte.

		if((ch & 0xC0) != 0x80)                         	//If each byte in the sequence after the initial byte is not formatted as: 10xxxxxx then sequence is illegal.
			flag = 0;
		while(i<=counter-1 && flag == 1){                   	//While i is less than counter-1(because we exclude the leading byte) and the sequence is legal.
			code = code << 6;                               	    //Shift code 6 positions to the left...
			code = code + (ch & 0x3F);                      	    //and add the last 6 bits of the subsequent byte(10xxxxxx).
			i++;                                            	    //Add one to i.
			if(i<=counter - 1){                               	    //If i is less than counter-1...
				ch = getchar();                             		    //read the next subsequent byte
				if((ch & 0xC0)!= 0x80)                      		    //and perform same check on its format. If not formatted correctly, sequence is illegal and...
					flag = 0;                                 		        //set flag to zero.
				}
			}
		}
		if(flag != 0){                                                                	//If sequence was legal.
			if((code>=0x0000 && code<=0xD7FF) || (code>=0xE000 && code<=0xFFFF)){    	//If the Unicode code point is in [0x0000,0xD7FF] or [0xE000,0xFFFF]
				putchar(code >> 8);                                                 		//Print the first 8 bytes.
				putchar(code & 0xFF);                                               		//Then the remaining 8.
			}
			else if(code>=0x010000 && code<=0x10FFFF){                          	//Else if the Unicode code point is in [0x010000,0x10FFFF]
				shiftTemp = ((code - 0x010000) >> 10);                                  //Set shiftTemp equal to the first 10 bits of (code - 0x010000).
				firstSur = (shiftTemp + 0xD800);                                        //First surrogate pair equals shiftTemp + 0xD800
				secndSur = ((code - 0x010000) & 0x3FF) + 0xDC00;                        //Second surrogate pair equals to the last 10 bits of (code - 0x010000) + 0xDC00
				putchar(firstSur >> 8);                                                 //Print first surrogate, WITHOUT the first  bits.
				putchar(firstSur & 0xFF);                                               //Print last 8 bits of first surrogate.
				putchar(secndSur >> 8);                                                 //Print second surrogate, WITHOUT the first  bits.
				putchar(secndSur & 0xFF);                                               //Print last 8 bits of second surrogate.
			}
		}
        ch1 = getchar();                                                                //Read the leading byte of the next sequence.
	}
	if(flag == 0)                                                                     	//If there was an illegal sequence, output an error message.
		printf("\n\nConversion was terminated. Invalid character in the provided text file.\n");
return 0;
}
