const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')

//> ROBOT ONLINE


class API {
  constructor(path) {
    this.port = new SerialPort(path, { baudRate: 9600 })
    this.parser = new Readline()
    this.port.pipe(this.parser)

    this.parser.on('data', line => console.log(`> ${line}`))
  }

  changeColour(r, g, b) {
    this.send(`<c,${r},${g},${b}>`, 10)
  }

  flash() {
    this.send(`<f>`, 10)
  }

  send(str, n) {
    const callback = () => {
      this.port.write(str)
      if(n > 0) {
        console.log("sending")
        this.send(str, n-1)
      }
    }
    setTimeout(callback, 200)
  }
}

const api = new API('/dev/cu.usbmodem14101')

setTimeout(() => api.changeColour(0,0,255), 4000)
setTimeout(() => api.flash(), 8000)

module.exports = API