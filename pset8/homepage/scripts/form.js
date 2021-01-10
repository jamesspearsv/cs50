const nodemailer = require('nodemailer');

window.addEventListener('DOMContentLoaded', () =>{
    let name = document.querySelector('#name').value;
    let email = document.querySelector('email').value;
    let message = document.querySelector('#message').value;

    let transporter = nodemailer.createTransport({
        service: 'gmail',
        auth: {
          user: 'youremail@gmail.com',
          pass: 'yourpassword'
        }
      });

    
});