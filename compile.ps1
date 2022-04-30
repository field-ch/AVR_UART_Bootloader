Set-ExecutionPolicy RemoteSigned

csc /target:exe /out:filetransfer.exe serial_update.cs
.\filetransfer.exe test_blink.bin