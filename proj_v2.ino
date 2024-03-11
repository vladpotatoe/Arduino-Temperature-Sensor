const int MAX_SAMPLES = 100; // Maximum number of samples for moving average

float temperatureSamples[MAX_SAMPLES]; // Array to store temperature samples

int sampleIndex = 0; // Index for the current sample
// Define pin connections for the 7-segment displays
const int displayPins[8] = {7, 6, 5, 4, 3, 2, 1, 0};

// Define pin connections for the MOSFETs controlling the displays
const int mosfetPins[3] = {10, 9, 8};

const int buttonPin = A1;
bool isCelsius = true;

// Define the binary patterns for digits 0 to 9
const byte numbers[10] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110  // 9
};

void setup() {

  // Set display pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(displayPins[i], OUTPUT);
  }
  
  // Set MOSFET pins as outputs
  for (int i = 0; i < 3; i++) {
    pinMode(mosfetPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT);
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    isCelsius = !isCelsius; // Toggle the temperature unit
    
  }

  float temperatureCelsius = readTemperature();

  if (!isCelsius) {
    temperatureCelsius = celsiusToFahrenheit(temperatureCelsius);
  }

  addTemperatureSample(temperatureCelsius); // Add the new temperature sample
  
  // Calculate the average temperature
  float averageTemperature = calculateAverageTemperature();
  
  // Convert Celsius to integer representation
  int temperature = int(averageTemperature);
  
  // Extract digits from the temperature
  int decimal = int((averageTemperature - temperature) * 10); // Extract the first decimal
  int roundedDecimal;
  if (decimal > 2.5 && decimal < 7.5){
    roundedDecimal = 5;
  } else {
    roundedDecimal = 0;
  }
  
  int tens = (temperature / 10) % 10;
  int ones = temperature % 10;
  
  // Display the temperature on the 7-segment displays
  displayNumber(tens, 1); // Display tens on the first display
  displayNumber(ones, 2); // Display ones on the second display
  displayNumber(roundedDecimal, 3); // Display decimal on the third display
}

// Function to read temperature
float readTemperature() {
  int A5Read = analogRead(A5);
  float voltage = ((A5Read / 1023.0) * 5) * 1000; // Convert to voltage
  float temperatureCelsius = (voltage - 2109) / (-10.9); // Convert to temperature in Celsius
  
 
  return temperatureCelsius;
  
}

// Function to display a number on the specified 7-segment display
void displayNumber(int digit, int displayIndex) {
  // Turn on the specified MOSFET and display the specified digit
  // Turn off the MOSFET for the other displays
  for (int disp = 0; disp < 3; disp++) {
    digitalWrite(mosfetPins[disp], (disp == (displayIndex - 1)) ? HIGH : LOW);
  }

  // Display the specified digit on the specified display
  displayDigit(digit);
  delay(5); // Adjust delay for visibility
}

// Function to display a digit on the 7-segment display
void displayDigit(int digit) {
  // Set the segments for the specified digit
  for (int i = 0; i < 8; i++) {
    digitalWrite(displayPins[i], bitRead(numbers[digit], i));
  }
}

// Initialize the array with zeros
void initSamples() {
  for (int i = 0; i < MAX_SAMPLES; i++) {
    temperatureSamples[i] = 0.0;
  }
}

// Add a new temperature sample to the array
void addTemperatureSample(float temperature) {
  temperatureSamples[sampleIndex] = temperature;
  sampleIndex = (sampleIndex + 1) % MAX_SAMPLES; // Move to the next sample index
}

// Calculate the average of the temperature samples
float calculateAverageTemperature() {
  float total = 0.0;
  for (int i = 0; i < MAX_SAMPLES; i++) {
    total += temperatureSamples[i];
  }
  return total / MAX_SAMPLES;
}

// Function to convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
  return celsius * 9 / 5 + 32;
}

