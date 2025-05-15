from django.db import models

class SensorData(models.Model):
    temperature = models.FloatField()
    humidity = models.FloatField()
    pressure = models.FloatField()
    RSSI = models.FloatField()
    SNR = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)  # Assurez-vous d'utiliser auto_now_add si vous voulez que le timestamp soit ajouté automatiquement

    def __str__(self):
        return f'{self.timestamp} - {self.temperature}°C'

