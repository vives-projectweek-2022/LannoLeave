const LannooLeaf = require('./LannooLeaf.js')
const Color = require('./Color.js')
const express = require('express')
const app = express()
const port = 3000
const cors = require('cors')
const bodyParser = require('body-parser')
// const lannoLeaf = new LannooLeaf();

app.use(cors())
app.use(bodyParser.json())
var jsonParser = bodyParser.json()

// async function main() {

//     // lannoLeaf.set_unit_all(0xff, 0x00, new Color(50, 0, 0, 50));

//     lannoLeaf.set_all_all(new Color(50, 0, 0, 50));
//     await new Promise(r => setTimeout(r, 1000));

//     lannoLeaf.set_all_all(new Color(0, 50, 0, 50));
//     await new Promise(r => setTimeout(r, 1000));

//     lannoLeaf.set_all_all(new Color(0, 0, 50, 50));
//     await new Promise(r => setTimeout(r, 1000));
// };




app.get('/', (req, res) => {
  res.send('Welcome to API for Lannoo leaf')
})

app.get('/spi', (req, res) => {
  main();
  res.send('SPI-DEMO!')
})

app.post('/', jsonParser, function (req, res) {
  console.log(JSON.stringify(req.body))
  // res.send('Poepsnor!')
  res.send('Your data: ' + JSON.stringify(req.body))
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
