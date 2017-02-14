@echo off

echo.
echo ##############################
echo ##                          ##
echo ##  Setting up COM port...  ##
echo ##                          ##
echo ##############################
echo.
echo Available COM ports:

powershell [System.IO.Ports.SerialPort]::getportnames() ; $port= new-Object System.IO.Ports.SerialPort COM23,115200,None,8,one ; $port.open() ; $port.Write(1) ; $port.Close();

pause