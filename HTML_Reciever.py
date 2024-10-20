import requests
import matplotlib.pyplot as plt
import time
import threading

# Replace with link in serial monitor
url = "http://192.168.50.212/"

# Variables to store the data
temperature = []
humidity = []
heat_index = []

# Function to fetch data from the Arduino web server
def fetch_data():
   while True:
       try:
           # Send a GET request to the Arduino web server
           response = requests.get(url)
           
           # Parse the JSON data
           data = response.json()
           
           # Append the data to the lists
           temperature.append(data["temperature"])
           humidity.append(data["humidity"])
           heat_index.append(data["heatIndex"])
           
           # Wait for 2 seconds before fetching the next data
           time.sleep(2)
       except:
           print("Error fetching data")

# Start the data fetching thread
data_thread = threading.Thread(target=fetch_data)
data_thread.start()

# Plot the data
plt.figure(figsize=(12, 6))

# Temperature plot
plt.subplot(1, 3, 1)
plt.plot(temperature)
plt.title("Temperature")
plt.xlabel("Time")
plt.ylabel("Temperature (°C)")

# Humidity plot
plt.subplot(1, 3, 2)
plt.plot(humidity)
plt.title("Humidity")
plt.xlabel("Time")
plt.ylabel("Humidity (%)")

# Heat index plot
plt.subplot(1, 3, 3)
plt.plot(heat_index)
plt.title("Heat Index")
plt.xlabel("Time")
plt.ylabel("Heat Index (°C)")

plt.tight_layout()
plt.show()
