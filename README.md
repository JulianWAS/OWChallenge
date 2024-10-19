# Julian Alden-Salter
# OW-Scent Challenge.

# Technical:

## Scent delivery device description
The scent delivery device has 6 channels. To open a channel and release a scent, a corresponding air control valve needs to be opened and a diaphragm pump started.

To open a valve, the GPIO pin on a microcontroller needs to be set to high. To close a valve, the GPIO pin needs to be set to low.
The valve GPIO pin mapping is as follows:

| Channel | GPIO |
|---------|------|
| 1       | 46   |
| 2       | 3    |
| 3       | 8    |
| 4       | 18   |
| 5       | 17   |
| 6       | 16   |


The pump is controlled by GPIO pin 11. To start the pump, the GPIO pin needs to be set high. To stop the pump, the GPIO pin needs to be set low.

## Communication
Serial communication is used to send and receive messages to and from the device.

# Requirements
Develop code for an embedded system that will enable the controlled release of scents (for a specific channel). Commands should be sent via serial communication and should specify the channel to open. The channel will stay open for 60 seconds and then close again.


# Design
I have chosen C as this allows the most control over code size and generation although C++ may eventually be a better choice. I have used C in a very C++ like manner 
to implement a predominatly object oriented approach mainly to allow clarity and ease of expansion (a device with multiple pumps, extra channels etc.)

Although not explicitly specified I have assumed that channels can be 'mixed' 

I have tried to focus on the safety aspects with robust error reporting and checking to ensure operations have taken place. Where I've run out of time I have tried to 
comment to this effect (e.g. not allowing the pump to run if no channels are open, etc.)

## Channel Object
Each channel has it's own object containing data and functionality

Init with gpio, name, UUID, fill level, usage counters, other info / metadata? Also handles setup from previously stored state after power down.

Set channel state on / off

Set channel name to showcase setup possibility

## Added: Object for a refill cart with refill level and usage data data
This was added on the fly and should really be it's own object

## Object for pump
Init with gpio, default time, other info / metadata?

Set pump state on / off

Set pump name

## Channel manager
Handles storage of and general interaction with channels

Initialisation of the manager will initialise the channels

## Pump Manager
Although not strictly necessary (there being a single pump), a manager allows the same design philosophy to be maintained and pumps to be added to future
devices without rewriting the entire firmware.

## Serial Parser
The serial parser is the heart of the system. Instructions originate from the host system and are passed to the device via a serial interface.

A simple binary TLV packet is implemented however a human readable version would also be possible for direct manual testing if necessary

Limited commands are provided however the code is structured such that this is easily expandable.

## Run Cycle
This starts the pumps, each pump handles it's own timer with callback shutoff mechanism which only initiates if the pump start was successful.
	
# Testing
This would be the proposed areas to focus testing. Given the time constraints tests have not been written but code has been designed to allow unit testing to be added. 

This is not the way I would do it in production code - Tests should be written according to the specification as this often gives insight into specification issues. 
Once this is done the code to pass these tests can be started. This is a long winded process though so in a veryy agile environment may not be the best way forward.

## Unit testing
Object testing of all functions

Channel manager testing

Pump manager testing

Serial parser testing

## Functional testing.
Logic probe testing of hardware for channel open / close for each channel.

Logic probe testing of pump operation.

Logic probe testing of pump operation duration.

# Changes and improvements
Moving the refill cartrige to it's own object / file

Adding a human readable serial control scheme (at command like)

Making the serial control scheme definable via a yamml / json, csv like file, create a generator script with python or similar to generate c/h files that match this schema

Ensure testing is automatable using something similar to the robot framework. This can then be added to the CI pipeline to test commits to the repo and inform 
developers of any issues.

Possibly convert to C++ although care should be taken if the MCU chosen is particularly low power.

If the device is portable / battery operated power saving measures will become very important. I have alluded to this within the code coments. 



