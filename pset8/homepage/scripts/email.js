/*  Formspree (https://formspree.io) was used to handle the backend for a quick solution.
    Attempted to use Node JS but was ubale to get it working for this project.
    I will revist Node JS and NodeMailer module at a later time.
    I wrote the sections to add the status banner to page after submission. */

window.addEventListener('DOMContentLoaded', () => {
    // get the form elements defined in your form HTML above
    
    var form = document.getElementById("email-form");
    var button = document.getElementById("submit");
    var status = document.getElementById("status-banner");

    // Success and Error functions for after the form is submitted
    
    function success() {
      form.reset();
        status.classList.add('alert');
        status.classList.add('alert-success');
        status.setAttribute('role', 'alert');
        status.innerHTML = "Success! Thanks!";
    }

    function error() {
        status.classList.add('alert');
        status.classList.add('alert-warning');
        status.setAttribute('role', 'alert');
        status.innerHTML = "Oops! There was a problem.";
    }

    // handle the form submission event

    form.addEventListener("submit", function(ev) {
      ev.preventDefault();
      var data = new FormData(form);
      ajax(form.method, form.action, data, success, error);
    });
  });
  
  // helper function for sending an AJAX request

  function ajax(method, url, data, success, error) {
    var xhr = new XMLHttpRequest();
    xhr.open(method, url);
    xhr.setRequestHeader("Accept", "application/json");
    xhr.onreadystatechange = function() {
      if (xhr.readyState !== XMLHttpRequest.DONE) return;
      if (xhr.status === 200) {
        success(xhr.response, xhr.responseType);
      } else {
        error(xhr.status, xhr.response, xhr.responseType);
      }
    };
    xhr.send(data);
};