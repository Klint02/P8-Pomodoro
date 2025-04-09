const express = require('express');
const app = express();
const port = 3000;

app.use(express.json());

// Simulate Arduino's response to GET request
app.get('/status', (req, res) => {
  res.json({ led: 'on', temp: 23 });
});

// Simulate Arduino responding to a POST command
app.post('/control', (req, res) => {
  console.log('Received control command:', req.body);
  res.json({ status: 'ok' });
});

app.listen(port, () => {
  console.log(`Simulated Arduino server running at http://localhost:${port}`);
});



// Prompt: I am creating an app in Flutter, which later is going to connect to an arduino board through IP. 
//         So, I want to simulate it with a server, but I do not know how to?