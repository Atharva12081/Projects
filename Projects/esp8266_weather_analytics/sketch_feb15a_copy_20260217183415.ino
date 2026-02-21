#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <time.h>
// ------------------- Wi-Fi Settings -------------------
const char* ssid = "Mayur@301-2G";
const char* password = "Mayur@301";
// ------------------- DHT11 Settings -------------------
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// ------------------- Web Server -------------------
ESP8266WebServer server(80);
// ------------------- Data Storage -------------------
String csvLog = "Date,Time,Temperature (°C),Humidity (%)\n";
String tableRows = ""; // Latest readings prepended
// ------------------- Time Setup -------------------
void setupTime() {
// UTC+5:30 offset (India Standard Time)
configTime(19800, 0, "pool.ntp.org", "time.nist.gov");
Serial.println("Waiting for time...");
while (!time(nullptr)) {
delay(500);
Serial.print(".");
}
Serial.println("\n✅ Time synchronized");
}
// ------------------- Get Formatted Date & Time -------------------
void getDateTime(String &dateStr, String &timeStr) {
time_t now = time(nullptr);
struct tm *ptm = localtime(&now); // localtime includes UTC+5:30
char dateBuffer[11]; // DD-MM-YYYY
sprintf(dateBuffer, "%02d-%02d-%04d", ptm->tm_mday, ptm->tm_mon + 1,
ptm->tm_year + 1900);
dateStr = String(dateBuffer);
char timeBuffer[9]; // HH:MM:SS
sprintf(timeBuffer, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min,
ptm->tm_sec);
timeStr = String(timeBuffer);
}
// ------------------- Serve Dashboard -------------------
void handleRoot() {
// HTML page with JS to fetch data every 2 seconds
String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>NodeMCU DHT11 Dashboard</title>
<style>
body { font-family: Arial; text-align: center; margin: 20px; }
h2 { color: #333; }
table { width: 80%; margin: 20px auto; border-collapse: collapse; }
th, td { border: 1px solid #ddd; padding: 8px; text-align: center; }
th { background: #4CAF50; color: white; }
tr:nth-child(even) { background: #f2f2f2; }
button { margin: 10px; padding: 10px 20px; background: #4CAF50; color:
white; border: none; cursor: pointer; }
button:hover { background: #45a049; }
</style>
</head>
<body>
<h2>NodeMCU Temperature & Humidity Dashboard</h2>
<button onclick="downloadCSV()">Download CSV</button>
<table id="dataTable">
<tr><th>Date</th><th>Time</th><th>Temperature (°C)</th><th>Humidity
(%)</th></tr>
</table>
<script>
async function fetchData() {
const response = await fetch('/data');
const data = await response.json();
const table = document.getElementById('dataTable');
const row = table.insertRow(1); // Insert at top
row.insertCell(0).innerText = data.date;
row.insertCell(1).innerText = data.time;
row.insertCell(2).innerText = data.temperature;
row.insertCell(3).innerText = data.humidity;
}
// Fetch new data every 2 seconds
setInterval(fetchData, 2000);
// CSV download
function downloadCSV() {
fetch('/download').then(res => res.text()).then(csv => {
const blob = new Blob([csv], { type: 'text/csv' });
const url = window.URL.createObjectURL(blob);
const a = document.createElement('a');
a.href = url;
a.download = 'sensor_data.csv';
a.click();
window.URL.revokeObjectURL(url);
});
}
// Load initial data once
fetchData();
</script>
</body>
</html>
)rawliteral";
server.send(200, "text/html", html);
}
// ------------------- Serve JSON data for JS -------------------
void handleData() {
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();
String date, timeStr;
getDateTime(date, timeStr);
// Save to CSV
csvLog += date + "," + timeStr + "," + String(temperature) + "," + String(humidity)
+ "\n";
String json = "{\"date\":\"" + date + "\",\"time\":\"" + timeStr +
"\",\"temperature\":\"" + String(temperature) + "\",\"humidity\":\"" +
String(humidity) + "\"}";
server.send(200, "application/json", json);
}
// ------------------- Serve CSV -------------------
void handleDownload() {
server.send(200, "text/csv", csvLog);
}
// ------------------- Setup -------------------
void setup() {
Serial.begin(115200);
dht.begin();
// Connect to Wi-Fi
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\n✅ WiFi connected!");
Serial.print("🌍 NodeMCU IP Address: ");
Serial.println(WiFi.localIP());
// Initialize time
setupTime();
// Server routes
server.on("/", handleRoot);
server.on("/data", handleData);
server.on("/download", handleDownload);
server.begin();
Serial.println("✅ Web server started");
}
// ------------------- Loop -------------------
void loop() {
server.handleClient();
}

