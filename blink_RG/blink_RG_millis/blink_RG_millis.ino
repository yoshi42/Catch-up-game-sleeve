//Наплічник, що мигає червоним або зеленим кольором, переключається при торканні до сенсору//

#include <Adafruit_NeoPixel.h>
#define PIN 11 // номер порта к которому подключен модуль
#define count_led 12 // количество светодиодов 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800); //first number change does distance between colors

#define sensor A0
int red = 3;  //выводы на тестовые светодиоды
int green = 2;

bool trigger_flag = 0;
bool color_flag = 0;

bool RG_select = 0;

int brightness = 100;

unsigned long time0 = 0;

void setup()
{
	pixels.begin();
  	pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"
	pinMode(sensor, INPUT_PULLUP);
	pinMode(red, OUTPUT);
	pinMode(green, OUTPUT);
	digitalWrite(red, LOW);
	digitalWrite(green, HIGH);
	Serial.begin(9600);
}
void loop()
{
	rg();

	if(RG_select == 0)
	{
		neo_green();
	}

	if(RG_select == 1)
	{
		neo_red();
	}
}

void neo_green()
{	
	for (int i = 0; i < count_led; i++)
	{	
		while((millis() - time0) <= 5)
		{
			pixels.setPixelColor(i, pixels.Color(0,brightness,0)); // r,g,b
			pixels.setPixelColor(i+1, pixels.Color(0,brightness,0)); // r,g,b
			pixels.setPixelColor(i+2, pixels.Color(0,brightness,0)); // r,g,b
			pixels.show();
		}
			time0 = millis();
		
		while((millis() - time0) <= 5)
		{
			pixels.setPixelColor(i-2, pixels.Color(0,0,0)); // r,g,b
			pixels.setPixelColor(i-1, pixels.Color(0,0,0)); // r,g,b
			pixels.setPixelColor(i, pixels.Color(0,0,0)); // r,g,b
			pixels.show();
		}
			time0 = millis();
	}
}

void neo_red()
{
	for (int i = 0; i < count_led; i++)
	{	
		while((millis() - time0) <= 3)
		{
			pixels.setPixelColor(i, pixels.Color(brightness,0,0)); // r,g,b
			pixels.setPixelColor(i+1, pixels.Color(brightness,0,0)); // r,g,b
			pixels.setPixelColor(i+2, pixels.Color(brightness,0,0)); // r,g,b
			pixels.show();
		}
			time0 = millis();

		while((millis() - time0) <= 3)
		{
			pixels.setPixelColor(i-2, pixels.Color(0,0,0));
			pixels.setPixelColor(i-1, pixels.Color(0,0,0));
			pixels.setPixelColor(i, pixels.Color(0,0,0));
			pixels.show();
		}
			time0 = millis();
	}
}

void rg() //тригер переключения с зеленого на красный - нажал - зеленый, отпустил-нажал - красный, отпустил-нажал - зеленый
{
  int a = 0;
  a = digitalRead(sensor);
  //delay(10);
  //Serial.println(a);
	if(a == HIGH && trigger_flag == 0 && color_flag == 0)
	{
		RG_select = 0;
		trigger_flag = 1;
	}

	if(a == LOW && trigger_flag == 1 && color_flag == 0)
	{
		color_flag = 1;
		trigger_flag = 0;
	}

	if(a == HIGH && trigger_flag == 0 && color_flag == 1)
	{
		RG_select = 1;
		trigger_flag = 1;
	}

	if(a == LOW && trigger_flag == 1 && color_flag == 1)
	{
		color_flag = 0;
		trigger_flag = 0;
	}
}
