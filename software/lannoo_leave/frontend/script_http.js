var backend = "http://172.16.240.30:3000"

var color_input = document.getElementById("RGB");

color_input.addEventListener("input", function(){
    var color_value = color_input.value;
    
    var JsonData = JSON.stringify({"color": color_value});
    post(JsonData, ""); 
  }, false);

function select_mode(selected) {
//   JSON format: effect: 1; mode: on; etc
var JsonData = JSON.stringify({"mode": this.id});
post(JsonData, ""); 
}

function select_effect(selected) {
var JsonData = JSON.stringify({"effect": this.id});
post(JsonData, ""); 
}

function onload() {
    var mode = document.getElementsByClassName("select_mode");
    var effects = document.getElementsByClassName("select_effect");
    
    for (let index = 0; index < mode.length; index++) {
        mode[index].onclick = select_mode;
    }

    for (let index = 0; index < effects.length; index++) {
        effects[index].onclick = select_effect;
    }
}

function post(data, url) {

    let xhr = new XMLHttpRequest();
    xhr.open("POST", backend + '/' + url);

    xhr.setRequestHeader("Accept", "application/json");
    xhr.setRequestHeader("Content-Type", "application/json");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
        console.log(xhr.responseText);
    }};

    xhr.send(data);
    
}