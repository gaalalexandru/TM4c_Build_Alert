@echo off
echo.
echo ##############################
echo ##                          ##
echo ##  Setting up COM port...  ##
echo ##                          ##
echo ##############################
echo.
echo Available COM ports:
powershell [System.IO.Ports.SerialPort]::getportnames()
powershell $TM4c_Port= new-Object System.IO.Ports.SerialPort COM23,115200,None,8,one
powershell $TM4c_Port.open()
REM $TM4c_Port.WriteLine(0)
pause