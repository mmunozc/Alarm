#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Keypad.h>
#include <LiquidCrystal.h>                 //Agrega la libreria del LCD
LiquidCrystal lcd(48, 49, 45, 44, 43, 42); //Inicializa la libreria y Define los pines digitales para el LCD
RTC_DS1307 RTC;

// INICIO CONFIGURACION DEL TECLADO
const byte filas = 4;    //define las 4 filas del teclado
const byte columnas = 3; //define las 3 columnas del teclado
char teclas[filas][columnas] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte pinesfilas[filas] = {21, 20, 19, 18};                                               //defines los pines digitales de las cuatro fila
byte pinescolumnas[columnas] = {17, 16, 15};                                             //defines los pines digitales de las tres columnas
Keypad teclado = Keypad(makeKeymap(teclas), pinesfilas, pinescolumnas, filas, columnas); //lectura del teclado
// FIN CONFIGURACION DEL TECLADO

// VARIABLES
int buz = 22;
int mw_max_value = 999;
int maxHoras = 24;
int maxMin = 60;
int Hor, Min, Sec;
int tmp, Inc, hor, mIn, add = 11;
int count = 0;
int b1 = 22; //ALARMA
int b2 = 23; //TEMPORIZADOR
int b3 = 24; //START

int fch = 11;
int Dia, dia;
int Mes, mes;
int Ano;

int off = 0;
bool timer = false;
char k;

void setup()
{
  Wire.begin();
  RTC.begin();

  pinMode(buz, OUTPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(47, OUTPUT);
  digitalWrite(47, 0);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("MODULO RTCDS1307");
  lcd.setCursor(0, 1);
  lcd.print("  Reloj Alarma  ");
  delay(2000);
  lcd.clear();

  if (!RTC.isrunning())
  {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop()
{
  DateTime now = RTC.now();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tiempo:");
  lcd.setCursor(7, 0);
  Hor = now.hour(), DEC;
  if (Hor <= 9)
  {
    lcd.print("0");
    lcd.print(Hor = now.hour(), DEC);
  }
  else
    lcd.print(Hor = now.hour(), DEC);
  lcd.print(":");
  Min = now.minute(), DEC;
  if (Min <= 9)
  {
    lcd.print("0");
    lcd.print(Min = now.minute(), DEC);
  }
  else
    lcd.print(Min = now.minute(), DEC);

  lcd.print(":");
  Sec = now.second(), DEC;
  if (Sec <= 9)
  {
    lcd.print("0");
    lcd.print(Sec = now.second(), DEC);
  }
  else
    lcd.print(Sec = now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Fecha:");
  lcd.print(Dia = now.day(), DEC);
  lcd.print("/");
  lcd.print(Mes = now.month(), DEC);
  lcd.print("/");
  lcd.print(Ano = now.year(), DEC);
  TimeCheck();
  delay(200);

  if (digitalRead(b2) == HIGH)
  {
    lcd.clear();
    delay(500);
    temporizador();
    lcd.clear();
  }

  if (digitalRead(b1) == HIGH)
  {
    lcd.clear();
    delay(500);
    current();
    time();
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarma Encendida");
    delay(2000);
  }
}

void Buz()
{
  if (digitalRead(b3) == HIGH)
    off = 0;
  if (off == 1)
  {
    digitalWrite(buz, HIGH);
    delay(500);

    digitalWrite(buz, LOW);
    delay(500);
  }
}

void TimeCheck()
{
  int tem[17];
  for (int i = 11; i < 17; i++)
  {
    tem[i] = EEPROM.read(i);
  }
  if (Hor == tem[11] && Min == tem[12] && off == 1)
  {
    add = 11;
    Buz();
    Buz();
    lcd.clear();
    lcd.print("alarma...........");
    lcd.setCursor(0, 1);
    lcd.print("...........alarma");
    Buz();
    Buz();
  }
}

void time()
{
  int tmp = 1, mins = 0, hors = 0, secs = 0;
  while (tmp == 1)
  {
    off = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Configura Alarma ");

    lcd.setCursor(0, 1);
    if (Hor <= 9)
      lcd.print("0");
    lcd.print(Hor);
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print(":");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    delay(200);

    k = teclado.getKey();
    if (k > 48 && k <= 57)
    {
      Hor = k - 48;
    }

    if (digitalRead(b3) == HIGH)
    {
      hor = Hor;
      EEPROM.write(add++, hor);
      tmp = 2;
      delay(500);
      while (digitalRead(b3) == HIGH)
      {
      };
    }
  }

  while (tmp == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print(Hor);
    lcd.print(":");
    if (Min <= 9)
      lcd.print("0");
    lcd.print(Min);
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print(Hor);
    lcd.print(":");
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Sec);
    lcd.print("  ");
    delay(200);

    k = teclado.getKey();
    if (k > 48 && k <= 57)
    {
      Min = k - 48;
    }

    if (digitalRead(b3) == HIGH)
    {
      mIn = Min;
      EEPROM.write(add++, mIn);
      tmp = 3;
      delay(500);
      while (digitalRead(b3) == HIGH)
      {
      };
    }
  }

  while (tmp == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Configura Fecha ");

    lcd.setCursor(0, 1);
    if (Dia <= 9)
      lcd.print("0");
    lcd.print(Dia);
    lcd.print("/");
    lcd.print(Mes);
    lcd.print("/");
    lcd.print(Ano);
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print("/");
    lcd.print(Mes);
    lcd.print("/");
    lcd.print(Ano);
    delay(200);

    k = teclado.getKey();
    if (k > 48 && k <= 57)
    {
      Dia = k - 48;
    }

    if (digitalRead(b3) == HIGH)
    {
      dia = Dia;
      EEPROM.write(fch++, dia);
      tmp = 4;
      delay(500);
      while (digitalRead(b3) == HIGH)
      {
      };
    }
  }

  while (tmp == 4){
    lcd.setCursor(0, 1);
    lcd.print(Dia);
    lcd.print("/");
    if (Mes <= 9)
      lcd.print("0");
    lcd.print(Mes);
    lcd.print("/");
    lcd.print(Ano);
    lcd.print("  ");
    delay(200);
    lcd.setCursor(0, 1);
    lcd.print(Dia);
    lcd.print(":");
    lcd.print("  ");
    lcd.print(":");
    lcd.print(Ano);
    lcd.print("  ");
    delay(200);

    k = teclado.getKey();
    if (k > 48 && k <= 57)
    {
      Mes = k - 48;
    }

    if (digitalRead(b3) == HIGH)
    {
      mes = Mes;
      EEPROM.write(fch++, mes);
      tmp = 0;
      delay(500);
      while (digitalRead(b3) == HIGH)
      {
      };
    }
  }
  off = 1;
  delay(10);
}

void current()
{
  lcd.setCursor(0, 1);
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(":");
  lcd.print(Sec);
}

void temporizador()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("INGRESE TIEMPO:");
  while (!timer)
  {
    k = '\0';
    do
    {
      k = teclado.getKey();
      detectButton();
      lcd.setCursor(2, 1);
      lcd.print(count);
    } while (k == '\0' && !timer);
    switch (k)
    {
    case '*':
      break;

    case '#':
      break;

    case '\0':
      break;

    default:
      count = (count * 10) + (k - 48);
      if (count > mw_max_value)
      {
        count = mw_max_value;
      }
      break;
    }
  }
  while (count > 0)
  {
    lcd.setCursor(1, 0);
    lcd.print("SEG. RESTANTES");
    lcd.setCursor(6, 1);
    lcd.print("s");
    lcd.setCursor(2, 1);
    lcd.print(count);

    long t_inicial = millis();
    long t_actual = millis();
    while (t_actual - t_inicial < 1000)
    {
      detectButton();
      t_actual = millis();
    }
    count--;
    if (count <= 0)
    {
      count = 0;
      timer = false;
    }
    lcd.clear();
  }
}

void detectButton()
{
  if (digitalRead(b3) == HIGH)
  {
    timer = true;
  }
}
