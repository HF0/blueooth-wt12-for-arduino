# Bluetooth WT12 library for Arduino

![WT12 module](./bluegigawt12.jpg)

A Simple arduino library for the WT12 bluetooth module by Bluegiga

# Dependencies 

To generate the documentation 

* doxygen [Link](https://github.com/doxygen/doxygen)
* moxygen. [Link](https://github.com/sourcey/moxygen)
     * (npm install moxygen -g to install it)

Run the .bat file provided to generate a markdown file with the documentation

# Connection

* Connect TX/RX and VCC/GND from the module to the Arduino

# Limitations

* The module is configured so that only one connection is allowed. Unexpected disconnections are not handled.

# Links

* An alternative comprehensive library for WT modules [Link](https://github.com/jrowberg/iwrap)


# Documentation

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`WT12DEBUG`](#_w_t12_bluetooth_8cpp_1a3943a793f4c36f686487c03958699b14)            | Set to true if you want the library to output debug information on Serial 0
`define `[`WT12_MAX_BUFFER_SIZE`](#_w_t12_bluetooth_8h_1aa7987de2d117dedffe88fcfd35b11a90)            | Maximum size in bytes of the internal buffer used to receive serial information
`class `[`WT12Bluetooth`](#class_w_t12_bluetooth) | Arduino library for WT12 Bluetooth module

## Members

#### `define `[`WT12DEBUG`](#_w_t12_bluetooth_8cpp_1a3943a793f4c36f686487c03958699b14) 

Set to true if you want the library to output debug information on Serial 0

#### `define `[`WT12_MAX_BUFFER_SIZE`](#_w_t12_bluetooth_8h_1aa7987de2d117dedffe88fcfd35b11a90) 

Maximum size in bytes of the internal buffer used to receive serial information

# class `WT12Bluetooth` 

Arduino library for WT12 Bluetooth module

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`WT12Bluetooth`](#class_w_t12_bluetooth_1aa319a90b78927c15891cb5e8fa48975f)`(HardwareSerial * serial,unsigned long baudrate)` | Constructor.
`public bool `[`on`](#class_w_t12_bluetooth_1afdea0270d7bbba9903453e4014658e81)`()` | Turns the module on.
`public bool `[`off`](#class_w_t12_bluetooth_1a5eb47b55802dce6b55332219b588f15f)`()` | Turns the module off.
`public bool `[`reset`](#class_w_t12_bluetooth_1ad868be8dad1f7df8920f376b51de24da)`()` | Resets the module.
`public bool `[`isConnected`](#class_w_t12_bluetooth_1a11143379bb672685e7bf9b148e164f2a)`()` | True if in data mode (a connection is established)
`public bool `[`send`](#class_w_t12_bluetooth_1a05f234a3af85fcc7cabbf448b052308c)`(char data,int dataSize)` | Sends data using the current active connection.
`public bool `[`sendAsString`](#class_w_t12_bluetooth_1ac590116df1a28a5f33642effc368857a)`(int data)` | Sends a single value (as String) using the current active connection.
`public bool `[`sendString`](#class_w_t12_bluetooth_1a7fdb3aa8511887ccc15346c26aa2d11d)`(char data)` | Sends a string using the current active connection.
`public bool `[`connect`](#class_w_t12_bluetooth_1ab52437a799e48a608da4f0a4065a23ca)`(char btMacAddr)` | Opens a connection to the specified bluetooth mac addr.
`public bool `[`waitForConnection`](#class_w_t12_bluetooth_1abd6e211cd531dcf7c4f6b2d31d4f3cb3)`(unsigned long timeoutMs)` | Waits for a connection to be initiated from a remote module.
`public int `[`receiveData`](#class_w_t12_bluetooth_1ae6a39bad1ad70cdc16969286e20ab638)`(char buffer,int maxBytes)` | Receives data from the active connection.
`public bool `[`closeConnection`](#class_w_t12_bluetooth_1a42cded905d08cdb69f4782ef45cfb074)`()` | Closes the current connection.
`public bool `[`setName`](#class_w_t12_bluetooth_1af12df69b54d33a90a9624706ca17a388)`(const char name)` | Sets the name of the module.

## Members

#### `public  `[`WT12Bluetooth`](#class_w_t12_bluetooth_1aa319a90b78927c15891cb5e8fa48975f)`(HardwareSerial * serial,unsigned long baudrate)` 

Constructor.

Creates an instance of the library but does not actually opens a connection with the module [on()](#class_w_t12_bluetooth_1afdea0270d7bbba9903453e4014658e81) must be called before using the instance 
#### Parameters
* `serial.` Reference to the serial port where the module has been connected 

* `baudrate.` (Optional) Serial baud rate. 115200 if not specified.

#### `public bool `[`on`](#class_w_t12_bluetooth_1afdea0270d7bbba9903453e4014658e81)`()` 

Turns the module on.

It opens the serial connection to the module, it sets some configuration parameters and resets it

#### `public bool `[`off`](#class_w_t12_bluetooth_1a5eb47b55802dce6b55332219b588f15f)`()` 

Turns the module off.

Closes the serial connection to the module 
#### Returns
true if the operation went well, false otherwise

#### `public bool `[`reset`](#class_w_t12_bluetooth_1ad868be8dad1f7df8920f376b51de24da)`()` 

Resets the module.

#### Returns
true if the operation went well, false otherwise

#### `public bool `[`isConnected`](#class_w_t12_bluetooth_1a11143379bb672685e7bf9b148e164f2a)`()` 

True if in data mode (a connection is established)

#### Returns
true if the operation went well, false otherwise

#### `public bool `[`send`](#class_w_t12_bluetooth_1a05f234a3af85fcc7cabbf448b052308c)`(char data,int dataSize)` 

Sends data using the current active connection.

#### Parameters
* `data.` Array of bytes to be sent 

* `dataSize.` Size of the data to be sent in bytes 

#### Returns
true if the operation went well (a connection is active), false otherwise

#### `public bool `[`sendAsString`](#class_w_t12_bluetooth_1ac590116df1a28a5f33642effc368857a)`(int data)` 

Sends a single value (as String) using the current active connection.

#### Returns
true if the operation went well (a connection is active), false otherwise

#### `public bool `[`sendString`](#class_w_t12_bluetooth_1a7fdb3aa8511887ccc15346c26aa2d11d)`(char data)` 

Sends a string using the current active connection.

#### Parameters
* `data.` Data to be sent. All the data up to the \0 character will be sent (excluding \0) 

#### Returns
true if the operation went well (a connection is active), false otherwise

#### `public bool `[`connect`](#class_w_t12_bluetooth_1ab52437a799e48a608da4f0a4065a23ca)`(char btMacAddr)` 

Opens a connection to the specified bluetooth mac addr.

#### Parameters
* `btMacAddr.` String with the mac address of the destination module. e.g. "BB:D3:8C:70:E0:DA" or "BBD38C70E0DA" 

#### Returns
true if the operation went well, false otherwise

#### `public bool `[`waitForConnection`](#class_w_t12_bluetooth_1abd6e211cd531dcf7c4f6b2d31d4f3cb3)`(unsigned long timeoutMs)` 

Waits for a connection to be initiated from a remote module.

If a connection is already active this method does nothing 
#### Parameters
* `timeoutMs.` If a connection is not activated withtin the specified timeout in ms the method returns 

#### Returns
true if the operation went well, false if timeout

#### `public int `[`receiveData`](#class_w_t12_bluetooth_1ae6a39bad1ad70cdc16969286e20ab638)`(char buffer,int maxBytes)` 

Receives data from the active connection.

If a connection is not active this method does nothing. This method does not block. It only returns the data that is available in the module at that moment. 
#### Parameters
* `buffer.` Buffer where the data will be received 

* `maxBytes.` Maximum number of bytes to be received 

#### Returns
the number of bytes that have been received

#### `public bool `[`closeConnection`](#class_w_t12_bluetooth_1a42cded905d08cdb69f4782ef45cfb074)`()` 

Closes the current connection.

If there is no active connection, the method does nothing 
#### Returns
true if the operation went well, false otherwise

#### `public bool `[`setName`](#class_w_t12_bluetooth_1af12df69b54d33a90a9624706ca17a388)`(const char name)` 

Sets the name of the module.

A maximum length of 16 bytes is allowed 
#### Returns
true if the operation went well, false otherwise

