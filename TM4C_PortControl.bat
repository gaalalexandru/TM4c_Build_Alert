@echo off

echo.
echo #########################
echo ##                     ##
echo ##  Allerting TM4C...  ##
echo ##                     ##
echo #########################
echo.

REM Manually enter TM4c device COM port number
set com=COM23
set arg1=%1
if "%arg1%"=="1" echo Job failed & echo Available COM ports: & powershell [System.IO.Ports.SerialPort]::getportnames() ; $port= new-Object System.IO.Ports.SerialPort %com%,115200,None,8,one ; [Byte] $command = 0x01 ; $port.open() ; $port.Write($command,0,1) ; $port.Close();
if "%arg1%"=="2" echo Job unstable & echo Available COM ports: &  powershell [System.IO.Ports.SerialPort]::getportnames() ; $port= new-Object System.IO.Ports.SerialPort %com%,115200,None,8,one ; [Byte] $command = 0x02 ; $port.open() ; $port.Write($command,0,1) ; $port.Close();
if "%arg1%"=="3" echo Job successefull & echo Available COM ports: &  powershell [System.IO.Ports.SerialPort]::getportnames() ; $port= new-Object System.IO.Ports.SerialPort %com%,115200,None,8,one ; [Byte] $command = 0x03 ; $port.open() ; $port.Write($command,0,1) ; $port.Close();
REM exit