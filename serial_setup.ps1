[System.IO.Ports.SerialPort]::getportnames()
powershell $port=new-Object System.IO.Ports.SerialPort COM23,115200,None,8,one
powershell $port.Close()
powershell $port.Open()