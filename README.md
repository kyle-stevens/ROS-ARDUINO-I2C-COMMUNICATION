# ROS-ARDUINO-I2C-COMMUNICATION
Code for both robotside and arduino side i2c protocols using ros multiarrrays to transfer motor values

#HOW_THE_METHODOLGY_WORKS
Since the i2c stream from linux to arduino sends values by byte, the method for sending an array of motor values was to convert it to a hexadecimal format to make all integers 4 bit, and then appended them to a char string, so that the arduino can later seperate the values with a delimiter and using the null ending as a marker for where each array ends.
