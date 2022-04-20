import express from 'express';
// import { Lannooleaf, Color, Graph } from 'lannooleaf_spi_controller';
import bodyParser from 'body-parser';
import cors from 'cors'
const app = express()
const port = 3000

app.use(cors())
app.use(bodyParser.json())
var jsonParser = bodyParser.json()

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

