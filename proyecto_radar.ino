#include <Servo.h> 

const int trigPin = 10;
const int echoPin = 9;

long duration;
long distance;


Servo myServo; 
//de ida
float angle1[141];//se guardan los angulos enteros de 0-140 grados
float dist1[141];//se guardan las distancias de esos angulos

//de regreso
float angle2[141];//se guardan los angulos enteros de 140-0 grados
float dist2[141];//se guardan las distancias de esos angulos

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);
  myServo.attach(12); 
}
void loop() {
  //LECTURA RADAR IDA
  for(int i=0;i<=140;i++)
  {  
    myServo.write(i);
    angle1[i]=i;
    delay(50);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(trigPin, LOW);
  
    
    //d = t/59;             //escalamos el tiempo a una distancia en cm



    
    //digitalWrite(trigPin, LOW); 
    //delayMicroseconds(2);
    //digitalWrite(trigPin, HIGH); 
    //delayMicroseconds(10);
  
    //digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance=duration/59;
    //distance= duration*0.034/2;
    dist1[i]=distance;
    
    //Serial.print("ang 1:"); 
    //Serial.print(angle1[i]); 
    //Serial.print(" ,  "); 
    //Serial.print("dist 1:"); 
    //Serial.print(dist1[i]);
    //Serial.print(" ;   ");
    //Serial.println();  
  }
  //LECTURA RADAR REGRESO
  for(int i=140;i>=0;i--)
  {  
    myServo.write(i);
    angle2[i]=i;
    delay(50);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(trigPin, LOW);
  
    
    //d = t/59;             //escalamos el tiempo a una distancia en cm



    
    //digitalWrite(trigPin, LOW); 
    //delayMicroseconds(2);
    //digitalWrite(trigPin, HIGH); 
    //delayMicroseconds(10);
  
    //digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance=duration/59;
    //distance= duration*0.034/2;
    dist2[i]=distance;
    
    //Serial.print("ang 2:"); 
    //Serial.print(angle2[i]); 
    //Serial.print(" ,  "); 
    //Serial.print("dist 2:"); 
    //Serial.print(dist2[i]);
    //Serial.print(" ;   ");
    //Serial.println();  
  }
  //SE COMPARAN VECTORES dist1 y dist 2
  int i=0,j=140,aux1=0,aux2=0,variacion=2,diferencia=0,band=0;
    while(i<=180&&band==0)
    {
      aux1=dist1[i];//se guarda de izquierda a derecha
      aux2=dist2[j];//se guarda de derecha a izquierda
      diferencia=aux1-aux2;//esto tiene que estar entre 2 o -2 porque es la variacion
      //primero se verifica que dieron mas o menos los mismos valores de ida que de regreso
      if(diferencia<=variacion&&diferencia>=-variacion)
      {
        //no hay variacion entre los vectores de ida y de regreso 
        //asi que analizamos el vector dist1 (puede ser el que sea porque ambos vectores son iguales)
        band=0;
      }
      else
      {
        //puede significar que se presentó ruido o que cambiaron los vectores de ida y de regreso
        //esto quiere decir que se cambio de posicion algun objeto 
        //se imprime que hubo un cambio y se analiza el dist 2 porque son los valores que cambiaron
        band=1;
      }
      i++;
      j--;
    }
    //SE COMPARA DATOS DEL VECTOR dist1
    int actual=0,siguiente=0,comodin=0,total=7,resta=0,cont=0,suma=0,dist_prom=0,inicio=0,fin=0,ang_prom=0,radianes=0,x=0,y=0;
    if(band==0)//se analiza el vector dist1 
    {
      for(i=0;i<=139;i++)
      {
        //esto es para checar en donde estan los objetos (que distancia tienen y en que rango de angulos aprox)
        actual=dist1[i];
        siguiente=dist1[i+1];
        resta=siguiente-actual;
        //si la variacion esta entre de 2 o -2 ok chido es un obj pero ademas este caso se debe de repetir mas de 7 veces
        if(resta<=variacion&&resta>=-variacion)
        {
          //entra aqui si es el ultimo caso
          if(i==139)
          {
            cont=cont+2;
            suma=suma+actual+siguiente;
          }
          else
          {
            //registra el primer angulo donde se encontro el parecido entre distancias
            if(cont==0)
            {
              inicio=i;
            }
            cont=cont+1;
            suma=suma+actual;
            fin=i+1;
          }
        }
        else
        {
          //la variacion se vuelve muy grande pero antes hay que contar si el contador llego al total
          //si no llego al total entonces eso significa que no es un objeto
          if(cont>=total)
          {
            //encontramos un objeto wuuuu
            //se calcula la distancia promedio
            dist_prom=suma/cont;
            ang_prom=(inicio+fin)/2;
            radianes=ang_prom*3.1416/180;
            //x=dist_prom*cos(ang_prom);
            //y=dist_prom*sin(ang_prom);
            x=dist_prom*cos(radianes);
            y=dist_prom*sin(radianes);
            
            if(dist_prom>=60)
            {
              //esto no es un objeto
            }
            else
            {
              if(ang_prom>90)
              {
                x=x*-1;
              }
              Serial.print("--------------------------");
              Serial.println();
              Serial.print("La distancia del objeto es: ");
              Serial.print(dist_prom);
              Serial.print("  cm ");
              Serial.println();
              Serial.print("El angulo donde esta el objeto es: ");
              Serial.print(ang_prom);
              Serial.print(" °");
              Serial.println();
              Serial.print("La coordenada en x: ");
              Serial.print(x);
              Serial.println();
              Serial.print("La coordenada en y: ");
              Serial.print(y);
              Serial.println();
              Serial.println();
            }
            
          }
          else
          {
            //aqui no habia nada
          }
          cont=0;
          suma=0;
        }
      }
    }
    //SE COMPARA DATOS DEL VECTOR dist2
    else 
    {
      for(i=0;i<=139;i++)
      {
        //esto es para checar en donde estan los objetos (que distancia tienen y en que rango de angulos aprox)
        actual=dist2[i];
        siguiente=dist2[i+1];
        resta=siguiente-actual;
        //si la variacion esta entre de 2 o -2 ok chido es un obj pero ademas este caso se debe de repetir mas de 7 veces
        if(resta<=variacion&&resta>=-variacion)
        {
          //entra aqui si es el ultimo caso
          if(i==139)
          {
            cont=cont+2;
            suma=suma+actual+siguiente;
          }
          else
          {
            //registra el primer angulo donde se encontro el parecido entre distancias
            if(cont==0)
            {
              inicio=i;
            }
            cont=cont+1;
            suma=suma+actual;
            fin=i+1;
          }
        }
        else
        {
          //la variacion se vuelve muy grande pero antes hay que contar si el contador llego al total
          //si no llego al total entonces eso significa que no es un objeto
          if(cont>=total)
          {
            //encontramos un objeto wuuuu
            dist_prom=suma/cont;
            ang_prom=(inicio+fin)/2;
            radianes=ang_prom*3.1416/180;
            //x=dist_prom*cos(ang_prom);
            //y=dist_prom*sin(ang_prom);
            x=dist_prom*cos(radianes);
            y=dist_prom*sin(radianes);
           
            if(dist_prom>=60)
            {
              //esto no es un objeto
            }
            else
            {
              if(ang_prom>90)
              {
                x=x*-1;
              }
              Serial.print("--------------------------");
              Serial.println();
              Serial.print("*La distancia del objeto es: ");
              Serial.print(dist_prom);
              Serial.print("  cm ");
              Serial.println();
              Serial.print("*El angulo donde esta el objeto es: ");
              Serial.print(ang_prom);
              Serial.print(" °");
              Serial.println();
              Serial.print("*La coordenada en x: ");
              Serial.print(x);
              Serial.println();
              Serial.print("*La coordenada en y: ");
              Serial.print(y);
              Serial.println();
              Serial.println();
            }
          }
          else
          {
            //aqui no habia nada
          }
          cont=0;
          suma=0;
        }
      }
      
    }
    band=0;
    Serial.print("----------------------------------------------------------------------------------");
    Serial.println();
delay(1000);  
  
  
}
