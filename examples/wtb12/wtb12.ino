#include "WT12Bluetooth.h"

#define DST_BT_MAC_ADDR "BBD38C70E0DA"

#define ARDUINO_UNO true

#if ARDUINO_UNO
#define BTSERIAL Serial
#define SERIALPRINT(msg)
#define SERIALPRINTLN(msg)
#define SERIALWRITE(v)
#else
#define BTSERIAL Serial1
#define SERIALPRINT(msg) Serial.print(msg)
#define SERIALPRINTLN(msg) Serial.println(msg)
#define SERIALWRITE(v) Serial.write(v)
#endif

WT12Bluetooth bt(&BTSERIAL);
const int bufferMaxSize = 100;
char buffer[bufferMaxSize];

void setup()
{
#if !ARDUINO_UNO
	Serial.begin(115200);
	delay(1000);
#endif
	SERIALPRINTLN("Setup");

	boolean btInitialized = bt.on();
	SERIALPRINTLN(btInitialized ? "BT ON" : "BT error when turning on");

	bt.setName("TestName");

}

void loop()
{
	
	while( !bt.isConnected() ){
		SERIALPRINTLN("Waiting for connection");
		bt.waitForConnection(5000l);
	}
	SERIALPRINTLN("Connected");
	while( bt.isConnected()){
		int bytesReceived;
		do{
			bytesReceived = bt.receiveData(buffer, 1);
		}while( bytesReceived != 1);
		char order = buffer[0];
		
		switch(order){
			case 'c':
			for( int i=2; i>=1; i--){
				bt.sendString("Closing connection in ");
				bt.sendAsString(i);
				bt.sendString("\r\n");
				delay(1000l);
			}
			bt.closeConnection();
			
			SERIALPRINTLN("Connection from this side...");
			if( bt.connect(DST_BT_MAC_ADDR)){
				SERIALPRINTLN("Connection ok");
				
				bt.sendString("Hello\r\n");

				bt.closeConnection();
				SERIALPRINTLN("Connection closed");
			}else{
				SERIALPRINTLN("Connection not possible");
			}
			break;

			default:
			SERIALWRITE(order);
			break;
		};
	}
}
