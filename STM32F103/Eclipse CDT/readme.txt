These codes were written using the Eclipse Embedded CDT. Since it has semihosting and needs other tools for debugging (OpenOCD) but use xPacks and ARM GCC the code is almost identical that bare metal written in Keil uVision, but, this tool start the devices at maximum clock frequency; Keil does the opposite (default: minimum freq, e.g. 8MHz). 

Only source files are included. 
