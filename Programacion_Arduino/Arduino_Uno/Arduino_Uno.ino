int setPin = 2;
int timedout = 100;
int fanOutput = 3;
int contador = 0, pulsos = 0;
boolean estadoActual=false,estadoAnterior = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(setPin,INPUT);
  pinMode(A0,INPUT);
  pinMode(fanOutput,OUTPUT);
  Serial.begin(9600);
  //Serial.println("ALL OK");
}

void loop() {
  if(!digitalRead(setPin)){
    //Serial.print("he entrado al select");
    //Serial.print("  numero: ");
    //Serial.println(pulsos);
    if(pulsos<=5 && pulsos!=0){
      fanFunction(pulsos);
    }else if(pulsos>6){
      pulsos = 0;
      //Serial.print("Numero de pulsos excedido: ");
      //Serial.println(pulsos);
      delay(1000);
    }else if(pulsos == 6){
      fanFunction(pulsos-6);
      //Serial.println("Apagando");
    }
  }else{
 
   int tiempoActual = millis();
   while(digitalRead(setPin));
   pulsos = millis()-tiempoActual;
   //Serial.print("EL TIEMPO FUE: ");
   //Serial.println(pulsos);
   if(pulsos>0 && pulsos<20){
    pulsos = 1;
   }else if(pulsos>19 && pulsos<40){
    pulsos = 2;
   }else if(pulsos>29 && pulsos<40){
    pulsos = 3;
   }else if(pulsos>39 && pulsos<50){
    pulsos = 4;
   }else if(pulsos>49 && pulsos<60){
    pulsos = 5;
   }else if(pulsos>59 && pulsos<70){
    pulsos = 6;
   }else{
    //Serial.println("ERRORR DE LECTURA");
   }
   //Serial.print("FIN DE LECTURA : ");
   //Serial.println(pulsos);
  
  // put your main code here, to run repeatedly:
  }
}

void fanFunction(int ondas){
  Serial.println("FanFunction");
  int onda2 = ondas;
  int estado = 1024,contador = 0;
  int timedOut = 30;//miliseconds timedout
  int actualTime = millis();
  int timeToDie = millis()+160;
  contador = 0;
  if(onda2==0){
    digitalWrite(fanOutput, LOW);
  }
  else{

  //espero al primer cruze por cero
  digitalWrite(fanOutput, HIGH);
  ////Serial.println(timeToDie);
  ////Serial.println(millis());
  ////Serial.println(ondas);
  ////Serial.println(contador);
    while(contador<=ondas && millis()<=timeToDie && ondas!=0){
      //do not nothuing
      estado = analogRead(A0);
      //Serial.print("ESTADO: ");
      //Serial.println(estado);
      if(estado<400 && contador>0){
        contador++;
        //Serial.print("Cruce Numero: ");
        //Serial.println(contador); 
      }else if(estado<400 && contador == 0){
        // primer cruce y commienzo a contar
        digitalWrite(fanOutput, LOW);
        contador++;
        Serial.print("Cruce Numero: ");
        Serial.println(contador);   
      }
      ////Serial.println(estado);

    }digitalWrite(fanOutput, HIGH);
    if(contador>ondas){
      //do not nothing
    }else if(ondas!=0){
      //Serial.println("error del sistema, no pude contar el cruze por cero");
    }


  }

}

