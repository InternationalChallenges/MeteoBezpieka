import serial
import re
import SensorData
import dbContext
import datetime

class Reader:
    
    def __init__(self):
        self.ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
        
    def read(self) -> SensorData.SensorData:
        print("Dans def read")
        if self.ser.is_open:
            try:
                print("Dans le try")
                while(True):
                    print("Dans le while")
                    data: list = self.ser.readlines()
                    if data:
                        print(data[0].split())   
                        data0 = data[0].split()
                        print(data0[0])
                        if data0[0] == b'Received':
                            print("dans le if final")
                            data0.pop(0)
                            data0.pop(0)		
                            print(data0)
                            encoded_data=[d.decode("utf-8", errors="ignore") for d in data0]
                            print(encoded_data)
                            parsed_data = self.parse_list_to_sensor_data(encoded_data)
                            print(parsed_data)
                            self.ser.reset_input_buffer()
                            return parsed_data
            except Exception as e:
                print("Error reading from serial port: ", e)
                return ""
        else:
            print("Serial port is not open!")
            return ""
    
    def parse_list_to_sensor_data(self, data: list[str]) -> SensorData:
        temperature = re.sub(r"[^\d.-]", "", data[1])
        humidity = re.sub(r"[^\d.-]", "", data[4])
        pressure = re.sub(r"[^\d.-]", "", data[7])
        rssi = re.sub(r"[^\d.-]", "", str(0))
        snr = re.sub(r"[^\d.-]", "", str(0))
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        return SensorData.SensorData(temperature, humidity, pressure, rssi, snr, timestamp)
     
    def close(self):
        self.ser.close()
        
        
if __name__ == "__main__":
    # Instances creation
    print("Starting UART reader...")
    reader = Reader()
    print("After Reader")
    dbConn = dbContext.dbContext()
    print("After DB")    
    # Reading data from serial and saving it to db
 
    while True:
       print("Dans la boucle")
       data: SensorData.SensorData = reader.read()
       print("After SensorData")
       dbConn.SaveData(data)
       print("after db connect")
