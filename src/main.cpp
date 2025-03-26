#include "IPAddress.h"
#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

const int buttons[] = {D3, D4, D5};
const int leds[] = {D0, D1, D2};
DHT dht(D6, DHT11);

float temperature = 0;
float humidity = 0;
unsigned long previousMillis = 0;

const char *ssid = "ESP8266";
const char *pswd = "12345678";

AsyncWebServer server(3000);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
  <head>
    <title>ESP8266 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <meta charset="UTF-8" />
    
    <style>
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
        font-family: Arial, sans-serif;
      }

      body {
        min-height: 100dvh;
        background-color: oklch(0.145 0 0);
        color: oklch(0.985 0 0);
        -webkit-font-smoothing: antialiased;
        -moz-osx-font-smoothing: grayscale;
      }

      .container {
        padding: 1rem;
        max-width: 28rem;
        margin: 0 auto;
      }

      h1 {
        font-size: 2.25rem;
        font-weight: 700;
        text-align: center;
        letter-spacing: -0.025em;
        scroll-margin: 5rem;
      }

      h2 {
        font-size: 1.5rem;
        font-weight: 500;
        display: flex;
        align-items: center;
        gap: 0.5rem;
      }

      .sensor, .controller {
        display: flex;
        flex-direction: column;
        gap: 1rem;
        margin-bottom: 2rem;
      }

      .sensor div {
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: space-between;
        gap: 1rem;
      }

      .sensor div p {
        font-size: 1rem;
        font-weight: 500;
        display: flex;
        align-items: center;
        gap: 0.5rem;
      }

      .controller button {
        display: inline-flex;
        flex-shrink: 0;
        align-items: center;
        justify-content: center;
        gap: 0.5rem;
        font-size: 1rem;
        font-weight: 500;
        white-space: nowrap;
        border-radius: 0.375rem;
        outline: none;
        background-color: oklch(0.985 0 0);
        color: oklch(0.205 0 0);
        height: 3rem;
        padding: 0.5rem 1rem;
      }
    </style>
  </head>

  <body>
    <main class="container">
      <h1>ESP8266 Web Server</h1>

      <section class="sensor">
        <h2>Sensor</h2>
        <div>
          <p>Temperature: <span id="temperature">0</span>°C</p>
          <p>Humidity: <span id="humidity">0</span>%</p>
        </div>
      </section>


      <section class="controller">
        <h2>Controller</h2>
        <button data-led="4">Toggle LED 1</button>
        <button data-led="5">Toggle LED 2</button>
        <button data-led="16">Toggle LED 3</button>
      </section>
    </main>

    <script>
      document.querySelectorAll("button").forEach((button) => {
        button.addEventListener("click", async () => {
          await fetch("/toggle", {
            method: "POST",
            headers: {
              "Content-Type": "application/x-www-form-urlencoded",
            },
            body: `pin=${button.dataset.led}`,
          });
        });
      });

      setInterval(async () => {
        const temperature = document.getElementById("temperature");
        const humidity = document.getElementById("humidity");

        const responseTemperature = await fetch("/temperature");
        const responseHumidity = await fetch("/humidity");

        temperature.textContent = await responseTemperature.text();
        humidity.textContent = await responseHumidity.text();
      }, 5000);
    </script>
  </body>
</html>

)rawliteral";

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(buttons[i], INPUT);
    pinMode(leds[i], OUTPUT);
  }
  dht.begin();

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, pswd);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/toggle", HTTP_POST, [](AsyncWebServerRequest *request) {
    String pin = request->arg("pin");
    if (pin.toInt() < 0 || pin.toInt() > 16) {
      request->send(400, "text/plain", "Invalid pin");
      return;
    }
    digitalWrite(pin.toInt(), !digitalRead(pin.toInt()));
    request->redirect("/");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(temperature));
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(humidity));
  });

  server.begin();
}

void loop() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttons[i]) == LOW) {
      while (digitalRead(buttons[i]) == LOW)
        ;
      digitalWrite(leds[i], !digitalRead(leds[i]));
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 5000) {
    previousMillis = currentMillis;

    float newTemperature = dht.readTemperature(false);
    float newHumidity = dht.readHumidity();

    Serial.print("Temperature: ");
    Serial.print(newTemperature);
    Serial.print("°C, Humidity: ");
    Serial.print(newHumidity);
    Serial.println("%");

    if (isnan(newTemperature) || isnan(newHumidity)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      temperature = newTemperature;
      humidity = newHumidity;
    }
  }
}
