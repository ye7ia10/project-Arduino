        #include <IRremote.h>
        #define LM35 A4
        #define RED 7
        #define GREEN 6
        #define BUZZ 8
        int trigPin = 9;
        int echoPin = 10;
        int RECV_PIN = 11;
        IRrecv irrecv(RECV_PIN);
        const String Ok ="Ok", power = "Power", temperature = "Temperature", distance = "Distance";//  function key
        bool isDistance = false, isTemperature = false;
        int number = 0;
        int threshTemperature = 40,threshDistance = 15;
        unsigned int Remote[] ={            
                    0x2FDA05F,  //5
                    0x2FD00FF,   //0
                    0x2FD847B,   //Ok
                    0x2FD807F,   //1
                    0x2FD40BF,   //2
                    0x2FDC03F,   //3
                    0x2FD20DF,   //4 
                    0x2FD609F,   //6
                    0x2FDE01F,   //7
                    0x2FD10EF,   //8
                    0x2FD906F,   //9    
                    0x2FD28D7,   //power
                    0x2FD12ED,   //Temperature
                    0x2FD926D   //Distance          
                    };
        
         String RemoteKey[] ={
                    "5",
                    "0",
                    "Ok",
                    "1",
                    "2",
                    "3",
                    "4",
                    "6",
                    "7",
                    "8",
                    "9",
                    "Power",
                    "Temperature",
                    "Distance"
                    };
        
        bool isOn = false;
        long duration;
        int distanceVal;
        decode_results results;
        float temp;
        void setup() {
          Serial.begin(9600);
          Serial.println("Protection System");
          irrecv.enableIRIn();
          pinMode(RED,OUTPUT);
          pinMode(GREEN,OUTPUT);
          pinMode(BUZZ, OUTPUT); 
          pinMode(trigPin,OUTPUT);
          pinMode(echoPin, INPUT);
          pinMode(LM35, INPUT);
        }
        // the loop routine runs over and over again forever:
        void loop() {
          if (irrecv.decode(&results)) {
          ValidateCode(results.value);
          irrecv.resume();
          }

          if (isOn){
          
              // Temprature measure 
          temp = analogRead(A4);
          temp = temp * 0.48826125;
          if (temp >= threshTemperature){
              digitalWrite(RED,HIGH);
              digitalWrite(BUZZ,HIGH);
          }

          //Distance Measure
          digitalWrite(trigPin,LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin,HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin,LOW);
          duration = pulseIn(echoPin, HIGH);
          distanceVal = duration * 0.034 / 2;
         
          if (temp >= threshTemperature){
            Serial.print("Temprature = ");
            Serial.println(temp);
            if (temp <= threshTemperature + 20){
                digitalWrite(BUZZ, HIGH);
                digitalWrite(RED, HIGH);
                delay(200);
                digitalWrite(RED, LOW);
                delay(200);
                digitalWrite(RED, HIGH);
                delay(300);
                digitalWrite(GREEN, HIGH);
            } else if (temp <= threshTemperature + 30){
                digitalWrite(BUZZ, HIGH);
                digitalWrite(RED, HIGH);
                delay(100);
                digitalWrite(RED, LOW);
                delay(100);
                digitalWrite(RED, HIGH);
                delay(400);
                digitalWrite(GREEN, HIGH);
            } else {
                digitalWrite(BUZZ, HIGH);
                digitalWrite(RED, HIGH);
                delay(500);
                digitalWrite(GREEN, HIGH);
            }
            
          } else if(distanceVal > threshDistance){
            digitalWrite(RED, LOW);
            digitalWrite(BUZZ, LOW);
            delay(500);
            digitalWrite(GREEN, LOW);
          }else{
            delay(500);
            digitalWrite(GREEN, LOW);
          }

           Serial.print("Distance = ");
           Serial.println(distanceVal);
          if (distanceVal <= threshDistance){
            digitalWrite(BUZZ, HIGH);
            digitalWrite(RED, HIGH);
          } else if(temp < threshTemperature){
            digitalWrite(BUZZ, LOW);
            digitalWrite(RED, LOW);
          }
          
        
          
          }            
        }


          void ValidateCode(int cd)
          {
            int found=0;
                for(int i=0; i< sizeof(Remote)/sizeof(int); i++)
                {
                  if(Remote[i] ==cd)
                  {
                    Serial.print("Key pressed:");
                    Serial.println(RemoteKey[i]);
                    Action(RemoteKey[i]);// take action          
                    found=1;
                  }
                }  
           
            
          }


           void Action(String value){
             if(value == power){
                  Serial.println("Power");
                  isOn = !isOn;
                  if (!isOn){
                    digitalWrite(RED, LOW);
                    digitalWrite(GREEN, LOW); 
                    digitalWrite(BUZZ, LOW);
                   }
              }else if(isOn){
                if(value == Ok){
                  Serial.println("Ok");
                  if(isTemperature){
                    threshTemperature = number;
                    number = 0;
                    isTemperature = false;
                  }else if(isDistance){
                    threshDistance = number;
                    number = 0;
                    isDistance = false;
                  }
                  number = 0;
                  Serial.print("temprarure = ");
                  Serial.println(threshTemperature);
                  Serial.print("Distance = ");
                  Serial.println(threshDistance);
                  
                }
                else if(value == temperature){
                  Serial.println("Temperature");
                  isTemperature = true;
                  isDistance = false;
                }else if(value == distance){
                  Serial.println("Distance");
                  isDistance = true;
                  isTemperature = false;
                }else{
                  //Number
                  int num = value[0] - '0';
                  number = number * 10 + num;
                  Serial.println(number);
                }
              } 
           
          }
