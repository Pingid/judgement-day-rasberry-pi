const PiCamera = require('pi-camera')
const fs = require('fs')
const { promisify } = require('util')
const axios = require('axios')
const argv = require('yargs').argv
const { spawn } = require('child_process');

const pictureFile = `${__dirname}/test.jpg`
const ENDPOINT = 'https://csdn6q1rd0.execute-api.eu-west-2.amazonaws.com/dev/'

const myCamera = new PiCamera({ mode: 'photo', output: pictureFile, nopreview: true, });

const sleep = (n, data = null) => new Promise((resolve, reject) => { setTimeout(() => resolve(data), n) })
const base64Encode = file => promisify(fs.readFile)(file).then(bitmap => new Buffer(bitmap).toString('base64'))

const takeAndUploadPicture = (uploadEndpoint) => myCamera.snap()
  .then(() => base64Encode(pictureFile))
  .then(base64 => axios.post(`${ENDPOINT}${uploadEndpoint}`, { image: base64 }))
  .then(() => console.log('PICTURE'))

const main = () => Promise.resolve()
  .then(() => console.log('tick'))
  .then(() => axios.get(`${ENDPOINT}state`))
  .then(({ data }) => data.cameras.running ? takeAndUploadPicture(data.cameras.uploadEndpoint).then(() => data) : data)
  .then(data => sleep(data.cameras.delay || 10000).then(main))
  .catch(error => { console.log(error); return sleep(data.cameras.delay || 10000).then(main) })

  const spawnProm = (command, args) => new Promise((resolve, reject) => {
    const ls = spawn(command, args);
    let ret = '';
    ls.stdout.on('data', (data) => { ret += data });
    ls.stderr.on('data', (data) => { ret += data });
    ls.on('close', (code) => { resolve(ret.trim()) });
  })

const startup = () => Promise.resolve()
  .then(() => axios.get(`${ENDPOINT}state`))
  .then(({ data }) => {
    
    return spawnProm('hostname', ['-I'])
      .then(ip => {
        console.log(data)
        console.log({ ...data, cameras: { ...data.cameras, address: { ...(data.cameras.address | {}), [argv.name]: ip }}})
      })
  })
  
startup()
main()
