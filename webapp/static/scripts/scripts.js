// Function to generate a random value within a given range
function getRandomValue(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}

// Function to update temperature progress bar (0°C to 50°C)
function updateTemperature(value) {
    const overlay = document.getElementById("progressOverlayTemperature");
    if (!overlay) return;

    let percentage = ((value - 0) / (50 - 0)) * 100; // Normalize to 0-100%
    percentage = Math.max(0, Math.min(100, percentage)); // Ensure within bounds

    overlay.style.width = (100 - percentage) + "%"; // Adjust white overlay

    let element = document.getElementById("infoTemperature");
    element.dataset.valeur = value + "°C";
    element.textContent = element.dataset.valeur;
}

// Function to update humidity progress bar (0% to 100%)
function updateHumidity(value) {
    const overlay = document.getElementById("progressOverlayHumidity");
    if (!overlay) return;

    let percentage = Math.max(0, Math.min(100, value)); // Ensure within bounds

    overlay.style.width = (100 - percentage) + "%"; // Adjust white overlay

    let element = document.getElementById("infoHumidity");
    element.dataset.valeur = value + "%";
    element.textContent = element.dataset.valeur;
}

// Function to update pressure progress bar (900hPa to 1100hPa)
function updatePressure(value) {
    const overlay = document.getElementById("progressOverlayPressure");
    if (!overlay) return;

    let percentage = ((value - 900) / (1100 - 900)) * 100; // Normalize between 900-1100hPa
    percentage = Math.max(0, Math.min(100, percentage)); // Ensure within bounds

    overlay.style.width = (100 - percentage) + "%"; // Adjust white overlay

    let element = document.getElementById("infoPressure");
    element.dataset.valeur = value + "hPa";
    element.textContent = element.dataset.valeur;
}

// Initialize charts with Chart.js (temperature, humidity, pressure)
document.addEventListener("DOMContentLoaded", function () {
    // Référence aux éléments HTML pour les graphiques
    const temperatureCanvas = document.getElementById('temperatureChart');
    const humidityCanvas = document.getElementById('humidityChart');
    const pressureCanvas = document.getElementById('pressureChart');

    if (!temperatureCanvas || !humidityCanvas || !pressureCanvas) {
        // The canvas elements aren't available on this page
        return;
    }

    const charts = Object.values(Chart.instances);
    if (charts.length > 0) return;

    // Initialisation des graphiques avec Chart.js
    var temperatureChart = new Chart(temperatureCanvas, {
        type: 'line',
        data: {
            labels: [],  // Étiquettes des données (ici, c'est le timestamp)
            datasets: [{
                label: 'Temperature (°C)',
                data: [],  // Données de température
                borderColor: 'red',  // Couleur de la ligne
                fill: true  // Remplir la zone sous la courbe
            }]
        },
        options: {
            responsive: true,
            scales: {
                x: {
                    ticks: { color: 'white' }
                },
                y: {
                    ticks: { color: 'white' }
                }
            }
        }
    });

    var humidityChart = new Chart(humidityCanvas, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Humidity (%)',
                data: [],
                borderColor: 'blue',
                fill: true
            }]
        },
        options: {
            responsive: true,
            scales: {
                x: { ticks: { color: 'white' } },
                y: { ticks: { color: 'white' } }
            }
        }
    });

    var pressureChart = new Chart(pressureCanvas, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Pressure (hPa)',
                data: [],
                borderColor: 'green',
                fill: true
            }]
        },
        options: {
            responsive: true,
            scales: {
                x: { ticks: { color: 'white' } },
                y: { ticks: { color: 'white' } }
            }
        }
    });

    function registerNewData(entry) {
        // Mettre à jour les barres de progression
        updateTemperature(entry.temperature);
        updateHumidity(entry.humidity);
        updatePressure(entry.pressure);

        // Mettre à jour les données des graphiques
        temperatureChart.data.labels.push(entry.timestamp); // Ajouter au début
        temperatureChart.data.datasets[0].data.push(entry.temperature); // Ajouter au début

        // console.log(humidityChart.data.datasets[0].data);
        humidityChart.data.labels.push(entry.timestamp); // Ajouter au début
        humidityChart.data.datasets[0].data.push(entry.humidity); // Ajouter au début

        pressureChart.data.labels.push(entry.timestamp); // Ajouter au début
        pressureChart.data.datasets[0].data.push(entry.pressure); // Ajouter au début

        temperatureChart.update();
        humidityChart.update();
        pressureChart.update();

        // Garder uniquement les 10 dernières données
        if (temperatureChart.data.labels.length > 10) {
            temperatureChart.data.labels.shift();  // Supprimer à la fin
            temperatureChart.data.datasets[0].data.shift();  // Supprimer à la fin

            humidityChart.data.labels.shift();  // Supprimer à la fin
            humidityChart.data.datasets[0].data.shift();  // Supprimer à la fin

            pressureChart.data.labels.shift();  // Supprimer à la fin
            pressureChart.data.datasets[0].data.shift();  // Supprimer à la fin

            temperatureChart.update();
            humidityChart.update();
            pressureChart.update();
        }
    }

    // Fonction pour récupérer les données du serveur Django et mettre à jour les graphiques
    function fetchSensorData() {
        fetch('/getSensorData/')
            .then(response => response.json())
            .then(data => {
                const timestamps = temperatureChart.data.labels;
                const latestUpdate = new Date(timestamps[timestamps.length - 1]);

                let newEntries = 0;

                for (let i = data.length-1; i >= 0; i--) {
                    if (new Date(data[i].timestamp) <= latestUpdate) {
                        break;
                    }
                    newEntries++;
                }

                for (let i = data.length-newEntries; i < data.length; i++) {
                    registerNewData(data[i]);
                }

                // console.log(`New entries: ${newEntries}`);

                // Faking entries if there are no new ones
                // if (newEntries === 0) {
                //     const lastEntry = data[data.length - 1];
                //     console.log(lastEntry);
                //     const fakeEntry = {
                //         temperature: lastEntry.temperature * getRandomValue(80, 120) / 100,
                //         humidity: lastEntry.humidity * getRandomValue(80, 120) / 100,
                //         pressure: lastEntry.pressure * getRandomValue(80, 120) / 100,
                //         timestamp: new Date(new Date(lastEntry.timestamp).getTime() + 5000).toISOString()
                //     }

                //     registerNewData(fakeEntry);
                // }
            })
            .catch(error => console.error('Error fetching sensor data:', error));
    }

    // Appeler la fonction pour récupérer les données toutes les 2 secondes
    fetchSensorData();
    setInterval(fetchSensorData, 1000);

    // document.addEventListener("keydown", function (event) {
    //     if (event.key === "n") {
    //         fetchSensorData();
    //     }
    // });
});

