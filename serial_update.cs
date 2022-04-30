// See https://aka.ms/new-console-template for more information
using System;
using System.IO;
using System.IO.Ports;
using System.Threading;

namespace AVR_serial_update
{
    public class update_app
    {
        public static void Main(string[] args) //firmware file provided in CLA: eg .\app.exe filename.bin
        {
            FileStream _firmware = new FileStream(args[0], FileMode.Open, FileAccess.Read, FileShare.Read); //open update file
            BinaryReader br = new BinaryReader(_firmware);  //Need a binary stream to read the data since its a binary file
            SerialPort _serialPort = new SerialPort();

            _serialPort.PortName = "COM3";  //initialize the serial port
            _serialPort.BaudRate = 9600;
            _serialPort.Parity = Parity.None;
            _serialPort.StopBits = StopBits.One;
            _serialPort.DataBits = 8;
            _serialPort.Handshake = Handshake.None;


            byte[] _code = new byte[_firmware.Length];  //buffer to store the contents of the firmware binary
            byte[] _size = new byte[2]; //buffer to store the size of the firmware update (in bytes)
            int length = _code.Length;

            int LSB = length & 0xFF;
            int MSB = length & (0xFF << 8);

            _size[0] = (byte)LSB;
            _size[1] = (byte)MSB;

            br.Read(_code, 0, _code.Length);    //read the firmware update, store contents in _code buffer
            _firmware.Close();


            /*UPLOAD FORMAT:
             * The AVR bootloader is expecting to receive the firmware update
             * in the following manner:
             * 2 bytes --> size of file
             * N bytes --> file bytes
             */
            _serialPort.Open();
            _serialPort.Write(_size, 0, 2); //Send firmware update size first
            _serialPort.Close();

            _serialPort.Open();
            _serialPort.Write(_code, 0, _code.Length); //Send firmware file contents
            Thread.Sleep(1000); //give the serial port enough time to transfer the whole file
            _serialPort.Close(); //close the port
        }
    }
}