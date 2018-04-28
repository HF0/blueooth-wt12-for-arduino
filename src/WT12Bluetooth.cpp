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
#include "WT12Bluetooth.h"


/** \Brief Set to true if you want the library to output debug information on Serial 0
* 
*/
#define WT12DEBUG false

#if WT12DEBUG
#define DEBUG(msg) Serial.print(msg)
#else
#define DEBUG(msg)
#endif

// initialize constants
const char WT12Bluetooth::WT12_CLOSE_CONNECTION_COMMAND[] = "CLOSE 0";
const char WT12Bluetooth::WT12_RING_BEGGINING_COMMAND[] = "RING ";
const char WT12Bluetooth::WT12_SET_DEFAULT_CONFIG_COMMAND[] = "SET CONTROL CONFIG 1100";
const char WT12Bluetooth::WT12_SET_PAGEMODE4_COMMAND[] = "SET BT PAGEMODE 4";
const char WT12Bluetooth::WT12_RESET_COMMAND[] = "RESET";
const char WT12Bluetooth::WT12_CHANGEMODE_COMMAND[] = "+++";
const char WT12Bluetooth::WT12_CALL_SPRINT_COMMAND[] = "CALL %s 1101 RFCOMM";

const char WT12Bluetooth::WT12_END_LINE[] = "\r\n";
const char WT12Bluetooth::WT12_READY_ANSWER[] = "READY.\r\n";
const char WT12Bluetooth::WT12_SET_CHECK_NAME_COMMAND[] = "SET BT NAME";
const char WT12Bluetooth::WT12_CALL_OK_BEGGINING_COMMAND[] = "CONNECT ";
const char WT12Bluetooth::WT12_CALL_END_BEGGINING_COMMAND[] = "NO CARRIER ";

WT12Bluetooth::WT12Bluetooth(HardwareSerial *serial, unsigned long baudrate){
	this->btserial = serial;
	this->baudrate = baudrate;
	this->commandMode = true;
	this->bufferSize = 0;
}

bool WT12Bluetooth::setName(const char name[]){
	if( name == NULL || strlen(name) > 16){
		return false;
	}
	sprintf(buffer, "%s %s", WT12_SET_CHECK_NAME_COMMAND, name);
	btserial->println(buffer);
	discardUntilNewLineOrTimeout();
	return true;
}

bool WT12Bluetooth::connect(char btMacAddr[]){
	if( isConnected()){
		return true;
	}
	// hex dot separated mac addr
	if( btMacAddr == NULL || strlen(btMacAddr) > 17 ){
		return false;
	}

	sprintf(buffer, WT12_CALL_SPRINT_COMMAND, btMacAddr);
	btserial->println(buffer);
	int answer = bufferUntilOrTimeout(5000l, 
		WT12_CALL_OK_BEGGINING_COMMAND, WT12_CALL_END_BEGGINING_COMMAND);
	if( answer != 0){
		discardUntilNewLineOrTimeout();
	}
	commandMode = answer == 0;
	return isConnected();
}

bool WT12Bluetooth::isConnected(){
	return !commandMode;
}


int WT12Bluetooth::receiveData(char buffer[], int maxBytes){
	if( commandMode ){
		return 0;
	}
	bufferSize = btserial->available();
	bufferSize = min( bufferSize, maxBytes);
	if( bufferSize > 0 ){
		btserial->readBytes(buffer, bufferSize);
	}
	return bufferSize;
}

bool WT12Bluetooth::reset(){
	bool resetOk = sendCommandAndWaitForAnswerOrTimeout(WT12_RESET_COMMAND, 1000l, WT12_READY_ANSWER);
	return resetOk;
}

void WT12Bluetooth::changeMode(){
	delay(1010);
	btserial->print(WT12_CHANGEMODE_COMMAND);
	delay(1010);
}

bool WT12Bluetooth::sendAsString(int data){
	if( commandMode){
		return false;
	}
	btserial->print(data);
	return true;
}

bool WT12Bluetooth::closeConnection(){
	if( !commandMode){
	}
	changeMode();
	btserial->println(WT12_CLOSE_CONNECTION_COMMAND);
	// we assume everything went fine
	int answer = bufferUntilOrTimeout(2000l, WT12_CALL_END_BEGGINING_COMMAND, "SYNTAX ERROR\r\n");
	if( answer == 1){
		discardUntilNewLineOrTimeout();
	}
	commandMode = true;
	return true;
}

void WT12Bluetooth::discardUntilNewLineOrTimeout(){
	bufferUntilOrTimeout(2000l, WT12_END_LINE);
}

bool WT12Bluetooth::send(char data[], int dataSize){
	if( commandMode){
		return false;
	}
	btserial->write(data, dataSize);
	return true;
}

bool WT12Bluetooth::sendString(char data[]){
	return send(data, strlen(data));
}

bool WT12Bluetooth::waitForConnection(unsigned long timeoutMs){
	if( !commandMode){
		return true;
	}

	unsigned long startMs = millis();
	unsigned long elapsedMs = 0;
	bufferSize = 0;
	boolean done = false;
	int answer;
	while( !done && elapsedMs < timeoutMs){
		unsigned long remaining = timeoutMs - elapsedMs;
		answer = bufferUntilOrTimeout(remaining, 
			WT12_RING_BEGGINING_COMMAND, WT12_END_LINE);
		// 0 timeout
		// 1 connection ok
		// 2 new line found
		done = answer == 0 || answer == 1;
		if( answer == 1){
			// discard line
			discardUntilNewLineOrTimeout();
		} 
		elapsedMs = millis() - startMs;
	}
	commandMode = answer != 1;
	return isConnected();
}

void WT12Bluetooth::deleteFromBufferUntil(const char delimiter[], int delimiterSizeBytes){
	int delimiterStartIndex = find(buffer, bufferSize, delimiter, delimiterSizeBytes);
	if( delimiterStartIndex != -1){
		int posAfterEndLine = delimiterStartIndex+delimiterSizeBytes;
		bufferSize -= posAfterEndLine;
		memcpy(buffer, &buffer[posAfterEndLine], bufferSize);
	}
}

int WT12Bluetooth::find(const char string[], int stringSize, const char substring[], int substringSize){
	bool found = false;
	for( int bufferStartIndex=0; !found && bufferStartIndex<= stringSize - substringSize; bufferStartIndex++){
		found = strncmp(&string[bufferStartIndex], substring, substringSize) == 0;
		if( found ){
			return bufferStartIndex;
		}
	}
	return -1;
}

int WT12Bluetooth::bufferUntilOrTimeout(unsigned long timeoutMs, const char answer1[], const char answer2[]){
	return bufferUntilOrTimeout(timeoutMs, answer1, answer2, NULL);
}

int WT12Bluetooth::bufferUntilOrTimeout(unsigned long timeoutMs, 
	const char answer1[], const char answer2[], const char answer3[]){

	unsigned long startMs = millis();
	unsigned long elapsedMs = 0;
	int bufferIndex = 0;
	int foundAnswerIndex = 0;
	while( foundAnswerIndex == 0 && elapsedMs < timeoutMs ){
		if( bufferIndex < WT12_MAX_BUFFER_SIZE && btserial->available() > 0){
			this->buffer[bufferIndex] = btserial->read();
			bufferIndex++;

			bool answer1Found = find(buffer, bufferIndex, answer1, strlen(answer1)) != -1;
			if( answer1Found){
				foundAnswerIndex = 1;
			}else if( answer2 != NULL){
				bool answer2Found = find(buffer, bufferIndex, answer2, strlen(answer2)) != -1;
				if( answer2Found ){
					foundAnswerIndex = 2;
				}
			}else if( answer3 != NULL){
				bool answer3Found = find(buffer, bufferIndex, answer3, strlen(answer3)) != -1;
				if( answer3Found){
					foundAnswerIndex = 3;
				}
			}
		}
		elapsedMs = millis() - startMs;
	}

	#if WT12DEBUG
	DEBUG("[");
	for(int i=0; i < bufferIndex; i++){
		DEBUG(buffer[i]);
	}
	DEBUG("]\r\n");
	#endif

	return foundAnswerIndex;
}

bool WT12Bluetooth::bufferUntilOrTimeout(unsigned long timeoutMs, const char answer[]){
	return bufferUntilOrTimeout(timeoutMs, answer, NULL, NULL) == 1;
}

bool WT12Bluetooth::sendCommandAndWaitForAnswerOrTimeout(
const char command[], unsigned long timeoutMs, const char answer[]){

	btserial->println(command);
	bool found = bufferUntilOrTimeout(timeoutMs, answer);
	return found;
}

bool WT12Bluetooth::on(){
	btserial->begin(this->baudrate);
	closeConnection();
	// set configuration
	sendCommandAndWaitForAnswerOrTimeout(WT12_SET_PAGEMODE4_COMMAND, 1000l, WT12_END_LINE);
	sendCommandAndWaitForAnswerOrTimeout(WT12_SET_DEFAULT_CONFIG_COMMAND, 1000l, WT12_END_LINE);
	return reset();
}

bool WT12Bluetooth::off(){
	if( btserial != NULL){
		btserial->end();
	}
	return true;
}
