const express = require('express');
const app = express();
const port = 3000;

app.use(express.json());

// Middleware to handle CORS preflight requests
app.use((req, res, next) => {
  res.set('Access-Control-Allow-Origin', '*');
  res.set('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.set('Access-Control-Allow-Headers', 'Content-Type');
  if (req.method === 'OPTIONS') {
    return res.sendStatus(204);
  }
  next();
});

// Simulate Arduino's response to GET request
app.get('/status', (_, res) => {
  res.set('Access-Control-Allow-Origin', '*'); // Allow CORS
  res.json({ led: 'on', temp: 23 });
});

// Simulate Arduino responding to a POST command
app.post('/control', (req, res) => {
  res.set('Access-Control-Allow-Origin', '*'); // Allow CORS
  if (!req.body || typeof req.body !== 'object') {
    console.error('Invalid control command received:', req.body);
    return res.status(400).json({ status: 'error', message: 'Invalid input' });
  }
  console.log('Received control command:', req.body);
  res.json({ id: 101, title:  req.body.title});
});

app.listen(port, () => {
  console.log(`Simulated Arduino server running at http://localhost:${port}`);
});



// Prompt: I am creating an app in Flutter, which later is going to connect to an arduino board through IP. 
//         So, I want to simulate it with a server, but I do not know how to?