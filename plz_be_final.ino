// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "Sweat Cookies"
#define REMOTEXY_WIFI_PASSWORD ""
#define REMOTEXY_SERVER_PORT 6377



uint8_t RemoteXY_CONF[] =   // 49 bytes
  { 255,1,0,31,0,42,0,16,112,0,69,0,14,9,10,10,13,67,5,48,
  8,41,10,2,26,21,68,18,16,31,73,25,8,36,135,1,0,30,7,12,
  12,2,31,82,69,83,69,84,0 };
  

struct {

    // input variables
  uint8_t button_1; // =1 if button pressed, else =0 

    // output variables
  int16_t sound_1; 
  char text_1[21];  
  float onlineGraph_1_var1;
  float onlineGraph_1_var2;

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#include <dht.h>
#include <Adafruit_MLX90614.h>
dht DHT;
#define DHT11_PIN 11

int delta_t =0;
int delta_h =0;
int old_t=999;
int old_h=999;
int count_t=0;
float sum_t=0;
int threshold =20;

int count_h=0;
float sum_h=0;
int flag=0;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
void setup() 
{
  RemoteXY_Init (); 

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  int chk = DHT.read11(DHT11_PIN);
  


  //comment starts here
  if (DHT.humidity != -999)
  {
  count_h+=1;
  sum_h+=DHT.humidity;

  count_t+=1;
  sum_t+=mlx.readObjectTempC();
  };
  
  
  
  
  if (count_h==threshold && flag==0){
    Serial.println("Average_humidity");
    Serial.println(sum_h/threshold);
    Serial.println("Old_humidity");
    Serial.println(old_h);

 
    
    
    if((old_h - (sum_h/threshold) < 4)){
      Serial.println("Average_humidity increasing");
      Serial.println("================================================");
      Serial.println("================================================");
    }
    else{
      Serial.println("Average_humidity decreasing!!");
      Serial.println("================================================");
      Serial.println("================================================");
    }

    
    
    
    Serial.println("Average_temperature");
    Serial.println(sum_t/threshold);
    Serial.println("Old_temp");
    Serial.println(old_t);
    if((old_t - (sum_t/threshold)  < 2)){
      Serial.println("Average_temperature increasing");
      Serial.println("================================================");
      Serial.println("================================================");
    }
    else{
      Serial.println("Average_temperature decreasing!!");
      Serial.println("================================================");
      Serial.println("================================================");
    }
    if((old_t  - (sum_t/threshold) < 2) && (old_h - (sum_h/threshold) > 4)){
      strncpy(RemoteXY.text_1, "DEHYD",21);
      RemoteXY.sound_1 = 2021;
      flag=1;
      Serial.print("DEHYD");
      Serial.println("================================================");
      Serial.println("================================================");
    } 
    else{
      strncpy(RemoteXY.text_1,"HYD",21);
//      RemoteXY.sound_1 =0;
      Serial.println("Hydrated");
      Serial.println("================================================");
      Serial.println("================================================");
      }
      old_h = sum_h/threshold;
      old_t = sum_t/threshold;
    
      count_t=0;
      sum_t=0;
      count_h=0;
      sum_h=0;
    }
   if (RemoteXY.button_1 == 1 && flag==1){
      RemoteXY.sound_1 =0;
      strncpy(RemoteXY.text_1,"",21);
      old_t=999;
      old_h=999;
      flag=0;
      count_t=0;
      sum_t=0;
      count_h=0;
      sum_h=0;
    } 

  if(DHT.temperature!=-999)
  {
//  delta_t = DHT.temperature - old_t;
//  old_t = DHT.temperature;
//  Serial.print("Temperature= ");
//  Serial.println(delta_t);
  //delta_h = DHT.humidity - old_h;
  // old_h = DHT.humidity;
  RemoteXY.onlineGraph_1_var2 = DHT.humidity;
  Serial.print("Humidity= ");
  Serial.print(DHT.humidity);
//  Serial.println(delta_h);
  Serial.print("Temp= ");
  Serial.print(mlx.readObjectTempC());
  }

  RemoteXY.onlineGraph_1_var1 = mlx.readObjectTempC();

}
