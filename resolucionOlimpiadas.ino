#include <Keypad.h>

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------FUNCIONES---------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

void encenderLED(int codigo);
void apagarLED();
void reservarLugares(int cantidad, int i, int j);
void apagarPulsadores();
void controlAsiento(int posicion);
void alarma();

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-------------------CONFIGURACIÓN KEYPAD--------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11, 12, 13, 14};
byte colPins[COLS] = {15, 16, 17, 18};

Keypad teclado = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

const int cant = 3;

int lugares[cant][cant] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
},
familia[cant][cant] = {
  {0,0,0},
  {0,0,0},
  {0,0,0}
};

bool bCod = false;
bool bFam = false, bHacer = true;

void setup() {
  // configuramos como salidas las salidas a utilizar para los LED's
  for(int i=2; i<11; i++){
    pinMode(i, OUTPUT);
  }
  
  //Configuración de los pulsadores
  for(int i=19; i<22; i++){
    pinMode(i, OUTPUT);
  }

  for(int i=22; i<25; i++){
    pinMode(i, INPUT);
  }
  
  apagarLED();
  apagarPulsadores();
  
  Serial.begin(9600);
}

void loop() {
  char letra = teclado.getKey();
  int lugDisp = 9;
  //La estructura del código será ABBCC
  //A: Cantidad de Familiares (Entre 1 y 9)
  //BB: codigo identificatorio de la Familia (Entre 00 y 99)
  //C: Codigo de Identificacion Personal (Entre 0 y 9)
  String codigo;
  if (letra){
    codigo += letra;
    while(letra!='*'){
      letra = teclado.getKey();
      
      if (letra && letra!='*'){
        codigo+= letra;
      }
    }
    Serial.println(codigo);
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        bCod = false;
        bFam = false;
        
        if(i==0 && j==0){
          if(lugares[i][j] == 0){
            lugares[i][j] = codigo.toInt(); //Asigno el código a la matriz lugares
            familia[i][j] = ((int) lugares[i][j]/10) - (int) ((int)(lugares[i][j]/1000))*100; //Guardo el código de familia en la matriz
            
            int cantLugares = ((int)(lugares[i][j]/1000))-1;
            Serial.println("\n");
            Serial.print("Familia ");
            controlAsiento(11);
            Serial.println("\n");
            Serial.print("Salimos ");
            //CANTIDAD DE LUGARES
            reservarLugares(cantLugares, i, j);
    
            //LUGARES QUE NO SE PUEDEN OCUPAR
            for(int x=0; x<3; x++){
              for(int y=0; y<3; y++){
                if(lugares[x][y] != 0 && lugares[x][y] != 2){
                  if(lugares[x+1][y] != 1 && x+1<3){
                    lugares[x+1][y] = familia[x+1][y] = 2;
                  }                  
                  if(lugares[x][y+1] != 1 && y+1 < 3){
                    lugares[x][y+1] = familia[x][y+1] = 2;  
                  }
                }
              }
            }
            codigo = ".";
            letra = '.';
            bHacer = false;
            break;
          }
        }else{
          if(bHacer){
            int lug = codigo.toInt();
            
            for(int x=0; x<3; x++){
              for(int y=0; y<3; y++){
                if(lugares[x][y] == lug){
                  Serial.println("Codigo ya ingresado");
                  bCod = false;
                }else{
                  bCod = true;
                }
                /*if(familia[x][y] != ((int) lug/10) - (int) ((int)(lug/1000))*100){
                  bCod = false;
                }else{
                  Serial.println("Familia");
                  bCod = true;
                }*/
              }
            }
  
            if(bCod){
              if(!bFam){
                if(lugares[i][j] == 0){
                  lugares[i][j] = codigo.toInt(); //Asigno el código a la matriz lugares
                  familia[i][j] = ((int) lugares[i][j]/10) - (int) ((int)(lugares[i][j]/1000))*100; //Guardo el código de familia en la matriz
    
                  controlAsiento((i+1)*10 + (j+1));
                  
                  int cantLugares = ((int)(lugares[i][j]/1000))-1;
                  Serial.println("Reservando");
                  Serial.println(i);
                  Serial.println(j);
                  Serial.println(cantLugares);
                  reservarLugares(cantLugares, i, j);
                  Serial.println("Lo hice?");
                  //LUGARES QUE NO SE PUEDEN OCUPAR
                  for(int x=0; x<3; x++){
                    for(int y=0; y<3; y++){
                      if(lugares[x][y] != 0 && lugares[x][y] != 2){
                        if(lugares[x+1][y] != 1 && x+1<3){
                          lugares[x+1][y] = familia[x+1][y] = 2;
                        }                  
                        if(lugares[x][y+1] != 1 && y+1 < 3){
                          lugares[x][y+1] = familia[x][y+1] = 2;  
                        }
                      }
                    }
                  }
            
                  codigo = " ";
                  letra = ' ';
                  bHacer = false;
                  break;
                }
              }
            }
          }
        }
      }
      delay(10);
    }
    
    bHacer=true;
    
    for(int x=0; x<3; x++){
      for(int y=0; y<3; y++){
        if(lugares[x][y] != 0){
          lugDisp -= 1;
        }
      }
    }
    Serial.print("\n");Serial.print("\n");
      
    for (int i=0;i<3;i++)
    {
      for(int j=0;j<3;j++)
      {
        Serial.print(lugares[i][j]);
        Serial.print("-");
      }
      Serial.println("\n");
    }
    Serial.println("\n");Serial.println("\n");
    Serial.println("La cantidad de lugares disponibles es: ");
    Serial.print(lugDisp);
    Serial.println("\n");Serial.println("\n");
    for (int i=0;i<3;i++){
      for(int j=0;j<3;j++){
        Serial.print(familia[i][j]);
        Serial.print("-");
      }
     Serial.println("\n");
    }
  }
}

void encenderLED(int codigo){
  int fila1 = 8, fila2 = 9, fila3 = 10;
  
  switch(codigo){
      case 11:
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;
        
      case 12:
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;

      case 13:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;
        
      case 21:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;
        
      case 22:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;

      case 23:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;

      case 31:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(2, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;
        
      case 32:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;

      case 33:
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila1, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(fila2, HIGH);    
        delay(10);
        apagarLED();
        digitalWrite(3, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(fila3, HIGH);
        delay(10);
        apagarLED();
        break;
    }
}

void apagarLED(){
  for(int i=2; i<11; i++){
    pinMode(i, LOW);
  }
}

void reservarLugares(int cantidad, int i, int j){
  int cont = 1;
  
  for (int x=0;x<3;x++){
    for(int y=0;y<3;y++){
      if(cont<cantidad+1){
        if(lugares[x][y] == 0){
          lugares[x][y] = 1;
          Serial.print("posición: ");
          Serial.print(i);
          Serial.print(" ");
          Serial.println(j);
          familia[x][y] = ((int) lugares[i][j]/10) - (int) ((int)(lugares[i][j]/1000))*100;
          cont++;
        }        
      }else{
        break;
      }
    }
  }
}

void apagarPulsadores(){
  for(int i=19; i<22; i++){
      digitalWrite(i, LOW);
    }
}

void controlAsiento(int posicion){
  bool bandera=false, bandera2 = false;
  while(1){
    
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        encenderLED(posicion);
        Serial.print("posicion: ");
        Serial.print(posicion);
        apagarPulsadores();
        digitalWrite(i+19, HIGH);
        if(digitalRead(j+22) && ((i+1)*10 + (j+1)) == posicion){
          return true;
          break;
        }else{
          if(digitalRead(j+22) && ((i+1)*10 + (j+1)) != posicion){
            bandera = false;
            bandera2 = true;
            alarma();
            break;
          }
        }
        delay(10);
      }
    }
    if(bandera)
      break;
    else
      if(bandera2)
        alarma();
  }
}

void alarma(){
  Serial.println("ALARMA");
}
