powershell usage:
```powershell
 $code='print("Hello from PowerShell!")';$pipe=New-Object System.IO.Pipes.NamedPipeClientStream('.','LatzufPipe','Out');$pipe.Connect();$bytes=[System.Text.Encoding]::UTF8.GetBytes($code);$length=[BitConverter]::GetBytes($bytes.Length);$pipe.Write($length,0,4);$pipe.Write($bytes,0,$bytes.Length);$pipe.Close()
```
