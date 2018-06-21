/*
MIT License

Copyright (c) 2018 HF0

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef WT12BLUETOOTH_H_
#define WT12BLUETOOTH_H_

/**
* Maximum size in bytes of the internal buffer used to receive serial information
*/
#define WT12_MAX_BUFFER_SIZE 150

#include <Arduino.h>

/**
* Arduino library for WT12 Bluetooth module
*/
class WT12Bluetooth {

	public:

	/** \brief Constructor
	*
	* Creates an instance of the library but does not actually opens a connection with the module
	* on() must be called before using the instance
	* \param serial. Reference to the serial port where the module has been connected
	* \param baudrate. (Optional) Serial baud rate. 115200 if not specified.
	*/
	WT12Bluetooth(HardwareSerial *serial, unsigned long baudrate = 115200);

	/** \brief 	Turns the module on
	*
	* It opens the serial connection to the module, it sets some configuration parameters and resets it
	*/
	bool on();
	
	/** \brief 	Turns the module off
	*
	* Closes the serial connection to the module
	* \returns true if the operation went well, false otherwise
	*/
	bool off();

	/** \brief Resets the module
	* \returns true if the operation went well, false otherwise
	*/
	bool reset();
	
	/** \brief True if in data mode (a connection is established)
	* \returns true if the operation went well, false otherwise
	*/
	bool isConnected();

	/** \brief Sends data using the current active connection
	* \param data. Array of bytes to be sent
	* \param dataSize. Size of the data to be sent in bytes
	* \returns true if the operation went well (a connection is active), false otherwise
	*/
	bool send(const char data[], int dataSize);
	
	/** \brief Sends a single value (as String) using the current active connection
	* \returns true if the operation went well (a connection is active), false otherwise
	*/
	bool sendAsString(int data);
	
	/** \brief Sends a string using the current active connection
	* \param data. Data to be sent. All the data up to the \0 character will be sent (excluding \0)
	* \returns true if the operation went well (a connection is active), false otherwise
	*/
	bool sendString(const char data[]);
	
	/** \brief Opens a connection to the specified bluetooth mac addr
	* \param btMacAddr. String with the mac address of the destination module. e.g. "BB:D3:8C:70:E0:DA" or "BBD38C70E0DA"
	* \returns true if the operation went well, false otherwise
	*/
	bool connect(const char btMacAddr[]);

	/** \brief Waits for a connection to be initiated from a remote module
	*
	* If a connection is already active this method does nothing
	* \param timeoutMs. If a connection is not activated withtin the specified timeout in ms the method returns
	* \returns true if the operation went well, false if timeout
	*/
	bool waitForConnection(unsigned long timeoutMs);
	
	/** \brief Receives data from the active connection
	*
	* If a connection is not active this method does nothing. 
	* This method does not block. It only returns the data that is available in the module at that moment.
	* \param buffer. Buffer where the data will be received
	* \param maxBytes. Maximum number of bytes to be received
	* \returns the number of bytes that have been received
	*/
	int receiveData(char buffer[], int maxBytes);
		
	/** \brief Closes the current connection
	*
	* If there is no active connection, the method does nothing
	* \returns true if the operation went well, false otherwise
	*/
	bool closeConnection();

	/** \brief Sets the name of the module
	*
	* A maximum length of 16 bytes is allowed
	* \returns true if the operation went well, false otherwise
	*/
	bool setName(const char name[]);
	
	private:
	
	void changeMode();

	void deleteFromBufferUntil(const char delimiter[], int delimiterSizeBytes);
	void discardUntilNewLineOrTimeout();

	bool bufferUntilOrTimeout(unsigned long timeoutMs, const char answer[]);
	int bufferUntilOrTimeout(unsigned long timeoutMs, const char answer1[], const char answer2[]);
	int bufferUntilOrTimeout(unsigned long timeoutMs, const char answer1[], const char answer2[], const char answer3[]);

	bool sendCommandAndWaitForAnswerOrTimeout(const char command[], unsigned long timeoutMs, const char answer[]);

	int find(const char string[], int stringSize, const char substring[], int substringSize);
	
	HardwareSerial* btserial;
	unsigned long baudrate;
	char buffer[WT12_MAX_BUFFER_SIZE];
	int bufferSize;
	bool commandMode;

	// Commands
	static const char WT12_CLOSE_CONNECTION_COMMAND[];
	static const char WT12_RING_BEGGINING_COMMAND[];
	static const char WT12_SET_DEFAULT_CONFIG_COMMAND[];
	static const char WT12_SET_PAGEMODE4_COMMAND[];
	static const char WT12_RESET_COMMAND[];
	static const char WT12_CHANGEMODE_COMMAND[];
	static const char WT12_SET_CHECK_NAME_COMMAND[];
	static const char WT12_CALL_SPRINT_COMMAND[];
	static const char WT12_CALL_OK_BEGGINING_COMMAND[];
	static const char WT12_CALL_END_BEGGINING_COMMAND[];
	
	static const char WT12_END_LINE[];
	static const char WT12_READY_ANSWER[];
};


#endif /* WT12BLUETOOTH_H_ */