#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string>
#include <stdio.h>
#include <ros/ros.h>
#include <stdlib.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt16MultiArray.h>



using namespace std;

int addr1 = 0x04;  //primary address //successive i2c devices
int file_i2c; //primary address 
int pwmVals[8];
char *filename = (char*)"/dev/i2c-1";
int i2c_slave_overflow = 32;

void arrayCallback(const std_msgs::UInt16MultiArray::ConstPtr& array);

bool writeStream(int file_i2c, string output) { 
    if (output.length() <= i2c_slave_overflow) {
        if (write(file_i2c, output.c_str(), output.length()) != output.length()) {
            printf("\tFailed to write to the i2c bus.\n");
        }
    }
    else {  
        printf("\t\tinput too long |:| overflow bounds exceeded and message voided\n");
    }
    return true;
}

bool readStream(int file_i2c, char buf[]) {
    int n=32; //this should be the number of expected characters
    if (read(file_i2c, buf, n) != n) {
        printf("\ti2c read transaction |:| failed\n");
    }
    else {
        printf("\t%s\n", buf);
    }
}

string intToHexDig(int value) {
        string cHex;//this block can be written as a case statement, but im lazy
        if (value == 15) {cHex = 'f';}
        else if (value == 14) {cHex = 'e';}
        else if (value == 13) {cHex = 'd';}
        else if (value == 12) {cHex = 'c';}
        else if (value == 11) {cHex = 'b';}
        else if (value == 10) {cHex = 'a';}
        else if (value == 9)  {cHex = '9';}
        else if (value == 8)  {cHex = '8';}
        else if (value == 7)  {cHex = '7';}
        else if (value == 6)  {cHex = '6';}
        else if (value == 5)  {cHex = '5';}
        else if (value == 4)  {cHex = '4';}
        else if (value == 3)  {cHex = '3';}
        else if (value == 2)  {cHex = '2';}
        else if (value == 1)  {cHex = '1';}
        else if (value == 0)  {cHex = '0';}
        return cHex;
}

string convertTo3BitHex(int decimal) {
    string converted;
    int firstChar = (int)(decimal/ (16 * 16));
    int firstRemainder = (int)(decimal % (16 * 16));
    int secondChar = (int)(firstRemainder / (16));
    int secondRemainder = (int)(firstRemainder % (16));
    int thirdChar = secondRemainder;
    string cfirstChar, csecondChar, cthirdChar;
    cfirstChar = intToHexDig(firstChar);
    csecondChar = intToHexDig(secondChar);
    cthirdChar = intToHexDig(thirdChar);

    converted = converted + cfirstChar;
    converted = converted + csecondChar;
    converted = converted + cthirdChar;
//    cout << converted << endl;
//    cout << "done"<<endl;
    return converted;
}



int main(int argc, char **argv) {
	//Open i2c bus
    
	if((file_i2c = open(filename, O_RDWR)) < 0) {
		printf("\tFailed to open the i2c bus\n");
	}
	if(ioctl(file_i2c, I2C_SLAVE, addr1) < 0) {
		printf("\tFailed to acquire bus access and/or talk to slave.\n");
	}

	
	ros::init(argc, argv, "pwmSubscriber");
	ros::NodeHandle n;
	ros::Subscriber subPWM = n.subscribe("array",1,arrayCallback);
    while(ros::ok()){	
        ros::spinOnce();
    }
	return 0;
}



void arrayCallback(const std_msgs::UInt16MultiArray::ConstPtr& array){
	int i = 0;
	for(std::vector<short unsigned int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it){
		pwmVals[i] = *it;
		i++;
	}
    cout << pwmVals <<endl;
	char hexVals[32];

	string sinput1;
	for (int i =0; i<8; i++){
		int valueOf = pwmVals[i];
		sinput1 = sinput1 + convertTo3BitHex(valueOf);
		if(i<7){sinput1 = sinput1 + "~";}
	}
    cout << sinput1;
    cout << endl;
	writeStream (file_i2c, sinput1);
    char buf2[32];
	readStream(file_i2c, buf2);
	return;
}
