These codes were written using the Eclipse Embedded CDT. It has semihosting and needs other tools for debugging (OpenOCD), but use xPacks and ARM GCC therefore code is almost identical that bare metal one written in Keil uVision. This tool starts the devices at their maximum clock frequency; Keil does the opposite (default: minimum freq, e.g. 8MHz). 

Only source files are included. 
