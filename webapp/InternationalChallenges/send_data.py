import requests
import random

url = "http://your-django-server-ip:8000/api/sensor/"
data = {
    "temperature": round(random.uniform(20.0, 30.0), 2),
    "humidity": round(random.uniform(40.0, 60.0), 2)
}

response = requests.post(url, json=data)

if response.status_code == 201:
    print("Data sent successfully!")
else:
    print("Failed to send data:", response.text)
