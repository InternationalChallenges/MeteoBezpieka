// UNUSED B)

// const express = require('express');
// const sqlite3 = require('sqlite3').verbose();
// const app = express();
// const path = require('path');
// const port = 3000;

// // Connect to SQLite database
// const db = new sqlite3.Database('./InternationalChallenges/webapp/db.sqlite3', (err) => {
//     if (err) {
//       console.error('Database connection error:', err);
//     } else {
//       console.log('Connected to SQLite database');
//     }
//   });
  

// // Serve static files (like your HTML, CSS, JS)
// app.use(express.static(path.join(__dirname, 'public')));

// // Endpoint to fetch sensor data
// app.get('/getSensorData', (req, res) => {
//     const query = 'SELECT temperature, humidity, pressure, RSSI, SNR, timestamp FROM InternationalChallenges_sensordata ORDER BY timestamp DESC LIMIT 10';
//     db.all(query, [], (err, rows) => {
//         if (err) {
//             res.status(500).json({ error: err.message });
//             return;
//         }
//         res.json(rows); // Send data back as JSON
//     });
// });

// // Function to generate random values
// function getRandomValue(min, max) {
//     return Math.floor(Math.random() * (max - min + 1) + min);
// }

// // Function to insert random data into the database for testing
// function insertRandomData() {
//     const temperature = getRandomValue(0, 50);
//     const humidity = getRandomValue(20, 100);
//     const pressure = getRandomValue(950, 1050);
//     const RSSI = getRandomValue(-100, -30);  // Example RSSI range
//     const SNR = getRandomValue(0, 50);      // Example SNR range
//     const timestamp = new Date().toISOString();  // Current timestamp in ISO format

//     // Insert random data into the database
//     const query = `INSERT INTO InternationalChallenges_sensordata (temperature, humidity, pressure, RSSI, SNR, timestamp) 
//                     VALUES (?, ?, ?, ?, ?, ?)`;
    
//     db.run(query, [temperature, humidity, pressure, RSSI, SNR, timestamp], function(err) {
//         if (err) {
//             console.error('Error inserting random data:', err.message);
//         } else {
//             console.log('Inserted random data into database:', { temperature, humidity, pressure, RSSI, SNR, timestamp });
//         }
//     });
// }

// // Uncomment the line below to insert random data every 5 seconds for testing
// //setInterval(insertRandomData, 5000);

// // Start the server
// app.listen(port, () => {
//     console.log(`Server running at http://localhost:${port}`);
// });
