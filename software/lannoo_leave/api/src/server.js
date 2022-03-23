const express = require('express')
const app = express()
const port = 3000
const cors = require('cors')
const bodyParser = require('body-parser')

app.use(cors())
app.use(bodyParser.json())
var jsonParser = bodyParser.json()

app.get('/', (req, res) => {
  res.send('Welcome to API for Lannoo leaf')
})

app.get('/import', (req, res) => {
  res.send('Welcome to API for Lannoo leaf')
})

app.post('/', jsonParser, function (req, res) {
  console.log(JSON.stringify(req.body))
  res.send('Your data: ' + JSON.stringify(req.body))
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
