# ESP8266 LED Control

В этом проекте используется Wi-Fi модуль ESP8266 и адресная светодиодная лента WS2812B. Для питания модуля используется MicroUSB, подключение осуществляется следующим образом: плюс к Vin, минус к GND, а провод для передачи данных к GPIO2.

## Функционал
Проект включает в себя следующий функционал:
1. Включение ленты.
2. Выключение ленты.
3. Режим красного цвета.
4. Режим зеленого цвета.
5. Режим синего цвета.
6. Режим радуги.

## Тестирование проекта

Чтобы протестировать проект, выполните следующие шаги:

1. Клонируйте репозиторий проекта:

   ```
   git clone git@github.com:VorobyovVV/Led_controller.git
   ```

2. Запустите скетч в среде разработки.

3. Подключитесь к Wi-Fi сети, созданной ESP8266. По умолчанию, сеть может иметь название вроде "ESP8266".

4. Перейдите по адресу http://192.168.4.1 в вашем веб-браузере.

5. В интерфейсе выберите Wi-Fi сеть, к которой вы хотите подключить модуль.

6. Введите SSID и пароль от вашей Wi-Fi сети.

7. Подключитесь к вашей сети.

8. Перейдите по IP-адресу, полученному ESP, в вашем веб-браузере.

9. Теперь вы можете дистанционно управлять светодиодной лентой.

![интерфейс для управления лентой](URL-адрес изображения)
